// ----------------------------------------------------------------------
//
//  Copyright (C) 2010 Fons Adriaensen <fons@linuxaudio.org>
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


#ifndef __GUICLASS_H
#define __GUICLASS_H


#include "button.h"


class Pbutt1 : public PushButton
{
public:

    Pbutt1 (X_window   *parent,
            X_callback *cbobj,
	    int         cbind,
            XImage     *image,
            int        xp,
            int        yp,
	    int        xs,
	    int        ys,
		float scale) :
    PushButton (parent, cbobj, cbind, image, xp, yp, xs, ys, scale)
    {
    }

private:

    virtual int handle_press (void);
    virtual int handle_relse (void);
};




class Pbutt2 : public PushButton
{
public:

    Pbutt2 (X_window   *parent,
            X_callback *cbobj,
	    int         cbind,
            XImage     *image,
            int        xp,
            int        yp,
	    int        xs,
	    int        ys,
		float scale) :
    PushButton (parent, cbobj, cbind, image, xp, yp, xs, ys, scale)
    {
    }

private:

    virtual int handle_press (void);
    virtual int handle_relse (void);
};




#endif
