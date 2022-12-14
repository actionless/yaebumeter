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


#ifndef __MAINWIN_H
#define	__MAINWIN_H


#include <clxclient.h>
#include "jclient.h"
#include "ebu_r128_disp.h"
#include "global.h"


class Mainwin : public A_thread, public X_window, public X_callback
{
public:

    //enum { XSIZE = 500, YSIZE = 75 };  // no logo
    enum { XSIZE = 526, YSIZE = 75 };

    Mainwin (X_rootwin *parent, X_resman *xres, int xp, int yp, Jclient *jclient, float win_scale);
    ~Mainwin (void);
    Mainwin (const Mainwin&);
    Mainwin& operator=(const Mainwin&);

    void stop (void) { _stop = true; }
    int process (void); 

	//static float scale;
	//float get_scale (void) { return scale; }

private:

    virtual void thr_main (void) {}

    void handle_time (void);
    void handle_stop (void);
    void handle_event (XEvent *);
    void handle_callb (int type, X_window *W, XEvent *E);
    void expose (XExposeEvent *E);
    void clmesg (XClientMessageEvent *E);
    void redraw (void);

    Atom            _atom;
    bool            _stop;
    X_resman       *_xres;
    Jclient        *_jclient;
    Ebu_r128_disp  *_ebudisp;

	float scale;
};


#endif
