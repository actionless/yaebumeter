// ----------------------------------------------------------------------
//
//  Copyright (C) 2011 Fons Adriaensen <fons@linuxaudio.org>
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
// ----------------------------------------------------------------------


#ifndef __BUTTON_H
#define	__BUTTON_H


#include <clxclient.h>


class PushButton : public X_window
{
public:

    PushButton (X_window    *parent,
                X_callback  *cbobj,
                int          cbind,
		XImage      *image,
	        int xp,
                int yp,
		int xs,
		int ys,
		float scale);

    virtual ~PushButton (void);

    enum { NOP = 100, PRESS, RELSE, DRAG };

    int    cbind (void) { return _cbind; }
    int    state (void) { return _state; }

    virtual void set_state (int s);

    static void init (X_display *disp);
    static void fini (void);

protected:

    X_callback  *_cbobj;
    int          _cbind;
    XImage      *_image;
    int          _state;
    int          _xs;
    int          _ys;

    void render (void);
    void callback (int k) { _cbobj->handle_callb (k, this, 0); }

private:

    void handle_event (XEvent *E);
    void bpress (XButtonEvent *E);
    void brelse (XButtonEvent *E);

    virtual int handle_press (void) = 0;
    virtual int handle_relse (void) = 0;

    static int _keymod;
    static int _button;
	float scale;
};


#endif
