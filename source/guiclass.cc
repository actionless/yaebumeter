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


#include "guiclass.h"
#include <stdio.h>

int Pbutt1::handle_press (void)
{
    _state |= 1;
    return 0;
}

int Pbutt1::handle_relse (void)
{
    _state &= ~1;
    return RELSE;
}


int Pbutt2::handle_press (void)
{
    return PRESS;
}

int Pbutt2::handle_relse (void)
{
    return 0;
}

