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


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "styles.h"
#include "global.h"
#include "mainwin.h"



Mainwin::Mainwin (X_rootwin *parent, X_resman *xres, int xp, int yp, Jclient *jclient, float win_scale) :
	scale (win_scale),
    A_thread ("Main"),
    X_window (parent, xp, yp, XSIZE*win_scale, YSIZE*win_scale, XftColors [C_MAIN_BG]->pixel),
    _stop (false),
    _xres (xres),
    _jclient (jclient)
{
    X_hints     H;
    char        s [1024];
	int x_scaled = (int)(roundf(XSIZE*scale));
	int y_scaled = (int)(roundf(YSIZE*scale));

    _atom = XInternAtom (dpy (), "WM_DELETE_WINDOW", True);
    XSetWMProtocols (dpy (), win (), &_atom, 1);
    _atom = XInternAtom (dpy (), "WM_PROTOCOLS", True);

    sprintf (s, "%s - %s  [%s]", PROGNAME, VERSION, jclient->jname ());
    x_set_title (s);
    H.position (xp, yp);
    H.minsize (x_scaled, y_scaled);
    H.maxsize (x_scaled, y_scaled);
    H.rname (xres->rname ());
    H.rclas (xres->rclas ());
    x_apply (&H); 

    _ebudisp = new Ebu_r128_disp (this, this, 0, 0, XftColors [C_DISP_BG], scale);
    _ebudisp->x_map ();

    x_add_events (ExposureMask); 
    x_map (); 
    set_time (0);
    inc_time (100000);
}

 
Mainwin::~Mainwin (void)
{
}

 
int Mainwin::process (void)
{
    int e;

    if (_stop) handle_stop ();

    e = get_event_timed ();
    switch (e)
    {
    case EV_TIME:
        handle_time ();
	break;
    }
    return e;
}


void Mainwin::handle_event (XEvent *E)
{
    switch (E->type)
    {
    case Expose:
	expose ((XExposeEvent *) E);
	break;  
 
    case ClientMessage:
        clmesg ((XClientMessageEvent *) E);
        break;
    }
}


void Mainwin::expose (XExposeEvent *E)
{
    if (E->count) return;
    redraw ();
}


void Mainwin::clmesg (XClientMessageEvent *E)
{
    if (E->message_type == _atom) _stop = true;
}



void Mainwin::handle_time (void)
{
    const Ebu_r128_proc *P = _jclient->ebu_r128 ();

    _ebudisp->set_level (P->loudness_M (), P->maxloudn_M (), P->loudness_S (), P->maxloudn_S ());
    _ebudisp->set_integ (P->integrated ());
    _ebudisp->set_range (P->range_min (), P->range_max ());
    _ebudisp->set_peak (_jclient->peakproc ()->readpeak ());
    inc_time (100000);
    XFlush (dpy ());
}


void Mainwin::handle_stop (void)
{
    put_event (EV_EXIT, 1);
}


void Mainwin::handle_callb (int type, X_window *W, XEvent *E)
{
    switch (type)
    {
    case Ebu_r128_disp::PAUSE:
	_jclient->ebu_r128 ()->integr_pause ();
	break;
    case Ebu_r128_disp::START:
	_jclient->ebu_r128 ()->integr_start ();
	break;
    case Ebu_r128_disp::RESET:
	_jclient->ebu_r128 ()->integr_reset ();
	break;
    }
}


void Mainwin::redraw (void)
{
    XPutImage (
		dpy (), win (), dgc (), redzita,
		0, 0,
		(int)roundf((XSIZE - 35) * scale), 0,
		(int)roundf(35*scale), (int)roundf(75*scale)
	);
}


