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


#ifndef __JCLIENT_H
#define __JCLIENT_H


#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <clthreads.h>
#include <jack/jack.h>
#include "global.h"
#include "ebu_r128_proc.h"
#include "peak_proc.h"


class Jclient : public A_thread
{
public:

    Jclient (const char *jname, const char *jserv, const char *connectl, const char *connectr);
    ~Jclient (void);
  
    const char *jname (void) const { return _jname; }
    Ebu_r128_proc* ebu_r128 () const { return (Ebu_r128_proc *) &_ebu_r128; }
    Peak_proc*     peakproc () const { return (Peak_proc *) &_peakproc; }
    
private:

    void  init_jack (const char *jname, const char *jserv, const char *connectl, const char *connectr);
    void  close_jack (void);
    void  jack_shutdown (void);
    int   jack_process (int frames);

    virtual void thr_main (void) {}

    jack_client_t  *_jack_client;
    jack_port_t    *_jack_testip;
    jack_port_t    *_inpports [2];
    const char     *_jname;
    unsigned int    _fsamp;
    int             _state;
    Ebu_r128_proc   _ebu_r128;
    Peak_proc       _peakproc;

  
    static void jack_static_shutdown (void *arg);
    static int  jack_static_process (jack_nframes_t nframes, void *arg);
};


#endif
