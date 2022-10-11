// ------------------------------------------------------------------------
//
//  Copyright (C) 2010-2011 Fons Adriaensen <fons@linuxaudio.org>
//    
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// ------------------------------------------------------------------------


#include <string.h>
#include "jclient.h"
#include "global.h"


Jclient::Jclient (const char *jname, const char *jserv) :
    A_thread ("Jclient"),
    _jack_client (0),
    _jname (0),
    _state (0)
{
    init_jack (jname, jserv);
}


Jclient::~Jclient (void)
{
    if (_jack_client) close_jack ();
}


void Jclient::init_jack (const char *jname, const char *jserv)
{
    int            opts;
    jack_status_t  stat;

    opts = JackNoStartServer;
    if (jserv) opts |= JackServerName;
    _jack_client = jack_client_open (jname, (jack_options_t) opts, &stat, jserv);
    if (! _jack_client)
    {
        fprintf (stderr, "Can't connect to JACK\n");
        exit (1);
    }

    jack_set_process_callback (_jack_client, jack_static_process, (void *) this);
    jack_on_shutdown (_jack_client, jack_static_shutdown, (void *) this);
    if (jack_activate (_jack_client))
    {
        fprintf(stderr, "Can't activate JACK.\n");
        exit (1);
    }
    _jname = jack_get_client_name (_jack_client);
    _fsamp = jack_get_sample_rate (_jack_client);

    _inpports [0] = jack_port_register (_jack_client, "in.L", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    _inpports [1] = jack_port_register (_jack_client, "in.R", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

    _ebu_r128.init (2, _fsamp);
    _peakproc.init (2, _fsamp);
    _state = 1;
}


void Jclient::close_jack ()
{
    jack_deactivate (_jack_client);
    jack_client_close (_jack_client);
}


void Jclient::jack_static_shutdown (void *arg)
{
    return ((Jclient *) arg)->jack_shutdown ();
}


void Jclient::jack_shutdown (void)
{
    send_event (EV_EXIT, 1);
}


int Jclient::jack_static_process (jack_nframes_t nframes, void *arg)
{
    return ((Jclient *) arg)->jack_process (nframes);
}


int Jclient::jack_process (int frames)
{
    float *ipp [2];

    if (_state == 0) return 0;

    ipp [0] = (float *) jack_port_get_buffer (_inpports [0], frames);
    ipp [1] = (float *) jack_port_get_buffer (_inpports [1], frames);

    _ebu_r128.process (frames, ipp);
    _peakproc.process (frames, ipp);

    return 0;
}


