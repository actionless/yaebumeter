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


#ifndef __STYLES_H
#define __STYLES_H


#include <clxclient.h>


enum
{
    C_MAIN_BG, C_MAIN_FG,
    C_MAIN_LS, C_MAIN_DS,
    C_DISP_BG, C_DISP_FG, C_DISP_PK,
    NXFTCOLORS
};

enum
{
    F_DISP1, F_DISP2,
    NXFTFONTS
};


extern int  styles_init (X_display *disp, X_resman *xrm, const char *shared, float scale);
extern void styles_fini (X_display *disp);

extern XftColor  *XftColors [NXFTCOLORS];
extern XftFont   *XftFonts [NXFTFONTS];

extern X_textln_style tstyle1;
extern X_textln_style tstyle2;
extern XImage *redzita;


#endif
