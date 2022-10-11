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


#include "styles.h"
#include "png2img.h"
#include "ebu_r128_disp.h"


XftColor      *XftColors [NXFTCOLORS];
XftFont       *XftFonts [NXFTFONTS];

X_textln_style tstyle1;
X_textln_style tstyle2;
XImage        *redzita;


static int loadimg (X_display *disp, const char *sdir, const char *file, XImage **imag)
{
    char   s [1024];
    XImage *p;

    snprintf (s, 1024, "%s/%s", sdir, file);
    p = png2img (s, disp, 0);
    *imag = p;
    return p ? 0 : 1;
}



int styles_init (X_display *disp, X_resman *xrm, const char *shared)
{
    XftColors [C_MAIN_BG] = disp->alloc_xftcolor (0.15f, 0.15f, 0.15f, 1.0f);
    XftColors [C_MAIN_FG] = disp->alloc_xftcolor (1.00f, 1.00f, 1.00f, 1.0f);
    XftColors [C_MAIN_LS] = disp->alloc_xftcolor (0.40f, 0.40f, 0.40f, 1.0f);
    XftColors [C_MAIN_DS] = disp->alloc_xftcolor (0.60f, 0.60f, 0.60f, 1.0f);
    XftColors [C_DISP_BG] = disp->alloc_xftcolor (0.15f, 0.15f, 0.15f, 1.0f);
    XftColors [C_DISP_FG] = disp->alloc_xftcolor (0.80f, 0.80f, 0.80f, 1.0f);
    XftColors [C_DISP_PK] = disp->alloc_xftcolor (1.00f, 1.00f, 1.00f, 1.0f);

    XftFonts [F_DISP1] = disp->alloc_xftfont (xrm->get (".font.disp1", "luxi:bold:pixelsize=13"));
    XftFonts [F_DISP2] = disp->alloc_xftfont (xrm->get (".font.disp2", "luxi:bold:pixelsize=18"));

    tstyle1.font = XftFonts [F_DISP1];
    tstyle1.color.normal.bgnd = XftColors [C_DISP_BG]->pixel;
    tstyle1.color.normal.text = XftColors [C_DISP_FG];

    tstyle2.font = XftFonts [F_DISP2];
    tstyle2.color.normal.bgnd = XftColors [C_DISP_BG]->pixel;
    tstyle2.color.normal.text = XftColors [C_DISP_FG];

    if (loadimg (disp, shared, "redzita.png", &redzita) || Ebu_r128_disp::init (disp, SHARED))
    {
	fprintf (stderr, "Can't load images from '%s'.\n", shared);
        return 1;
    }
    	
    return 0;
}


void styles_fini (X_display *disp)
{
    redzita->data = 0;
    XDestroyImage (redzita);
    Ebu_r128_disp::fini (disp);
}
