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


#include <X11/X.h>
#include <X11/Xlib.h>
#include <clxclient.h>
#include <math.h>
#include "png2img.h"
#include "styles.h"
#include "ebu_r128_disp.h"


XImage  *Ebu_r128_disp::_ihscale09r = 0;
XImage  *Ebu_r128_disp::_ihscale09a = 0;
XImage  *Ebu_r128_disp::_ihscale18r = 0;
XImage  *Ebu_r128_disp::_ihscale18a = 0;
XImage  *Ebu_r128_disp::_ihticks = 0;
XImage  *Ebu_r128_disp::_ihmeterbg = 0;
XImage  *Ebu_r128_disp::_ihmeter09 = 0;
XImage  *Ebu_r128_disp::_ihmeter18 = 0;
XImage  *Ebu_r128_disp::_ibscale09 = 0;
XImage  *Ebu_r128_disp::_ibscale18 = 0;
XImage  *Ebu_r128_disp::_ibscaleLU = 0;
XImage  *Ebu_r128_disp::_ibscaleFS = 0;
XImage  *Ebu_r128_disp::_ibmodeM = 0;
XImage  *Ebu_r128_disp::_ibmodeS = 0;
XImage  *Ebu_r128_disp::_ibpause = 0;
XImage  *Ebu_r128_disp::_ibstart = 0;
XImage  *Ebu_r128_disp::_ibreset = 0;
XImage  *Ebu_r128_disp::_ibpeak1 = 0;


int Ebu_r128_disp::loadimg (X_display *disp, const char *sdir, const char *file, XImage **imag, float scale)
{
    char   s [1024];
    XImage *p;

    snprintf (s, 1024, "%s/%s", sdir, file);
    p = png2img (s, disp, 0, scale);
    *imag = p;
    return p ? 0 : 1;
}


int Ebu_r128_disp::init (X_display *disp, const char *shared, float scale)
{
    return (   loadimg (disp, shared, "hscale09r.png", &_ihscale09r, scale)
            || loadimg (disp, shared, "hscale09a.png", &_ihscale09a, scale)
            || loadimg (disp, shared, "hscale18r.png", &_ihscale18r, scale)
            || loadimg (disp, shared, "hscale18a.png", &_ihscale18a, scale)
            || loadimg (disp, shared, "hticks.png", &_ihticks, scale)
            || loadimg (disp, shared, "hmeterbg.png", &_ihmeterbg, scale)
            || loadimg (disp, shared, "hmeter09.png", &_ihmeter09, scale)
            || loadimg (disp, shared, "hmeter18.png", &_ihmeter18, scale)
            || loadimg (disp, shared, "bscale09.png", &_ibscale09, scale)
            || loadimg (disp, shared, "bscale18.png", &_ibscale18, scale)
            || loadimg (disp, shared, "bscaleLU.png", &_ibscaleLU, scale)
            || loadimg (disp, shared, "bscaleFS.png", &_ibscaleFS, scale)
            || loadimg (disp, shared, "bmodeM.png", &_ibmodeM, scale)
            || loadimg (disp, shared, "bmodeS.png", &_ibmodeS, scale)
            || loadimg (disp, shared, "bpause.png", &_ibpause, scale)
            || loadimg (disp, shared, "bstart.png", &_ibstart, scale)
            || loadimg (disp, shared, "breset.png", &_ibreset, scale)
            || loadimg (disp, shared, "bpeak1.png", &_ibpeak1, scale));
} 


void Ebu_r128_disp::fini (X_display *disp)
{
    _ihscale09r->data = 0;
    _ihscale09a->data = 0;
    _ihscale18r->data = 0;
    _ihscale18a->data = 0;
    _ihticks->data = 0;
    _ihmeterbg->data = 0;
    _ihmeter09->data = 0;
    _ihmeter18->data = 0;
    _ibscale09->data = 0;
    _ibscale18->data = 0;
    _ibscaleLU->data = 0;
    _ibscaleFS->data = 0;
    _ibmodeM->data = 0;
    _ibmodeS->data = 0;
    _ibpause->data = 0;
    _ibstart->data = 0;
    _ibreset->data = 0;
    _ibpeak1->data = 0;
    XDestroyImage (_ihscale09r);
    XDestroyImage (_ihscale09a);
    XDestroyImage (_ihscale18r);
    XDestroyImage (_ihscale18a);
    XDestroyImage (_ihticks);
    XDestroyImage (_ihmeterbg);
    XDestroyImage (_ihmeter09);
    XDestroyImage (_ihmeter18);
    XDestroyImage (_ibscale09);
    XDestroyImage (_ibscale18);
    XDestroyImage (_ibscaleLU);
    XDestroyImage (_ibscaleFS);
    XDestroyImage (_ibmodeM);
    XDestroyImage (_ibmodeS);
    XDestroyImage (_ibpause);
    XDestroyImage (_ibstart);
    XDestroyImage (_ibreset);
    XDestroyImage (_ibpeak1);
}


void Ebu_r128_disp::scaled_XPutImage(Display* dpy, Drawable win, GC dgc, XImage* _img, int a, int b, int c, int d, int e, int f) {
    XPutImage (dpy, win, dgc, _img, (int)(a*scale), (int)(b*scale),  (int)(c*scale), (int)(d*scale), (int)(e*scale), (int)(f*scale));
}

void Ebu_r128_disp::scaled_XFillRectangle(Display* dpy, Drawable win, GC dgc, int a, int b, int c, int d) {
	XFillRectangle (dpy, win, dgc, (int)(a*scale), (int)(b*scale),  (int)(c*scale), (int)(d*scale));
}

X_textip* Ebu_r128_disp::scaled_X_texttip(Ebu_r128_disp* drawable, X_callback* cb, X_textln_style* style, int a, int b, int c, int d, int e) {
	return new X_textip(drawable, cb, style, (int)(a*scale), (int)(b*scale), (int)(c*scale), (int)(d*scale), (int)(e*scale));
}

Ebu_r128_disp::Ebu_r128_disp (X_window *parent, X_callback *callb, int xp, int yp, XftColor *bg, float win_scale) :
	scale (win_scale),
    X_window (parent, (int)(xp*win_scale), (int)(yp*win_scale), (int)(XS*win_scale), (int)(YS*win_scale), bg->pixel),
    _callb (callb),
    _count (0)
{
    int x, y;

    _t_unit = scaled_X_texttip (this,  0, &tstyle2, X1 + 5, Y2, 60, 24, 7);
    _t_unit->x_map ();
    _t_integ = scaled_X_texttip (this, 0, &tstyle1, X1 - 280, Y5, 120, 18, 31);
    _t_integ->x_map ();
    _t_range = scaled_X_texttip (this, 0, &tstyle1, X1 - 120, Y5, 120, 18, 31);
    _t_range->x_map ();

    x = X1 + 50;
    y = Y5 - 7;
    _bpeak1 = new Pbutt1 (this, this, BPEAK1, _ibpeak1, x, y, 32, 24, scale);
    _bpeak1->x_map();

    x = X1 + 120;
    y = Y0 + 4;
    _bmodeM = new Pbutt1 (this, this, BMODEM, _ibmodeM, x, y, 24, 16, scale);
    _bmodeM->x_map();
    _bmodeM->set_state (2);
    _bmodeS = new Pbutt1 (this, this, BMODES, _ibmodeS, x, y + 17, 24, 16, scale); 
    _bmodeS->x_map();
    x += 36;
    _bscale09 = new Pbutt1 (this, this, BSCALE09, _ibscale09, x, y, 24, 16, scale);
    _bscale09->x_map();
    _bscale18 = new Pbutt1 (this, this, BSCALE18, _ibscale18, x, y + 17, 24, 16, scale); 
    _bscale18->x_map();
    x += 26;
    _bscaleLU = new Pbutt1 (this, this, BSCALELU, _ibscaleLU, x, y, 24, 16, scale);
    _bscaleLU->x_map();
    _bscaleFS = new Pbutt1 (this, this, BSCALEFS, _ibscaleFS, x, y + 17, 24, 16, scale); 
    _bscaleFS->x_map();

    x = X1 + 120;
    y = Y5 - 7;
    _bpause = new Pbutt1 (this, this, BPAUSE, _ibpause, x, y, 24, 24, scale);
    _bpause->x_map();
    _bpause->set_state (2);
    _bstart = new Pbutt1 (this, this, BSTART, _ibstart, x + 25, y, 24, 24, scale);
    _bstart->x_map();
    _breset = new Pbutt1 (this, this, BRESET, _ibreset, x + 60, y, 24, 24, scale);
    _breset->x_map();
 
    _ticks = _ihticks;
    _imag0 = _ihmeterbg;
    _pixref = X0 + 18 * IDIV;
    _vm = -200;
    _pm = -200;
    _vs = -200;
    _ps = -200;
    _vi = -200;
    _v0 = -200;
    _v1 = -200;
    set_scale (false, false);
    x_add_events (ExposureMask);
}


Ebu_r128_disp::~Ebu_r128_disp (void)
{
}


void Ebu_r128_disp::set_scale (bool abs, bool ext)
{
    if (ext)
    {
	_scale = abs ? _ihscale18a : _ihscale18r;
        _imag1 = _ihmeter18;
        //_db2pix = (int)(IDIV / 2.0 * scale);
        _db2pix = (int)(IDIV / 2.0);
        _bscale09->set_state (0);
        _bscale18->set_state (2);
    }
    else
    {
	_scale = abs ? _ihscale09a : _ihscale09r;
        _imag1 = _ihmeter09;
        //_db2pix = (int)(IDIV * scale);
        _db2pix = IDIV;
        _bscale09->set_state (2);
        _bscale18->set_state (0);
    }
    if (abs)
    {
        _t_unit->set_text ("LUFS");
	_bscaleLU->set_state (0);
	_bscaleFS->set_state (2);
    }
    else
    {
        _t_unit->set_text ("LU");
	_bscaleLU->set_state (2);
	_bscaleFS->set_state (0);
    }
    _abs = abs;
    scaled_XPutImage (dpy (), win (), dgc (), _scale,  0, 0,  0, Y0, IMGS, 10); 
    disp_integ ();
}



void Ebu_r128_disp::set_level (float vm, float pm, float vs, float ps)
{
    _vm = vm;
    _pm = pm;
    _vs = vs;
    _ps = ps;
    disp_level ();
}


void Ebu_r128_disp::set_integ (float vi)
{
    _vi = vi;
    disp_integ ();
}


void Ebu_r128_disp::set_range (float v0, float v1)
{
    _v0 = v0;
    _v1 = v1;
    disp_range ();
}


void Ebu_r128_disp::set_peak (float v)
{
    if (v > 1.0f) _bpeak1->set_state (4);
    else if (_bpeak1->state () < 4)
    {
        if (v > 0.891f)
	{
	    _count = 10;
            _bpeak1->set_state (2);
	}
	else if (_count && --_count == 0)
	{
	    _bpeak1->set_state (0);
	} 
    }
}


void Ebu_r128_disp::handle_event (XEvent *E)
{
    switch (E->type)
    {
    case Expose:
	expose ((XExposeEvent *) E);
	break;  
    }
}


void Ebu_r128_disp::handle_callb (int type, X_window *W, XEvent *E)
{
    PushButton *B;
    int         k;

    switch (type)
    {
    case PushButton::RELSE:
	B = (PushButton *) W;
	k = B->cbind ();
	switch (k)
	{
	case BMODEM:
	    _bmodeM->set_state (2);
	    _bmodeS->set_state (0);
	    break;
	case BMODES:
	    _bmodeM->set_state (0);
	    _bmodeS->set_state (2);
	    break;
	case BSCALE09:
	    set_scale (_bscaleFS->state(), false);
	    break;
	case BSCALE18:
	    set_scale (_bscaleFS->state(), true);
	    break;
	case BSCALELU:
	    set_scale (false, _bscale18->state());
	    break;
	case BSCALEFS:
	    set_scale (true, _bscale18->state());
	    break;
	case BPAUSE:
	    _bpause->set_state (2);
	    _bstart->set_state (0);
	    _callb->handle_callb (PAUSE, 0, 0);
	    break;
	case BSTART:
	    _bpause->set_state (0);
	    _bstart->set_state (2);
	    _callb->handle_callb (START, 0, 0);
	    break;
	case BRESET:
	    _callb->handle_callb (RESET, 0, 0);
	    break;
	case BPEAK1:
	    _bpeak1->set_state (0);
	    break;
	}
    }
}


void Ebu_r128_disp::expose (XExposeEvent *E)
{
    if (E->count) return;
    XSetFunction (dpy (), dgc (), GXcopy);
    scaled_XPutImage (dpy (), win (), dgc (), _scale,  0, 0,  0, Y0, IMGS, 12); 
    scaled_XPutImage (dpy (), win (), dgc (), _ticks,  0, 0,  0, Y1, IMGS, 8);
    scaled_XPutImage (dpy (), win (), dgc (), _imag0,  0, 0,  0, Y2, IMGS, H2); 
    scaled_XPutImage (dpy (), win (), dgc (), _imag0,  0, 0,  0, Y3, IMGS, H3); 
    scaled_XPutImage (dpy (), win (), dgc (), _imag0,  0, 0,  0, Y4, IMGS, H4); 
    _kr = (X0 - 1);
    _kp = (X0 - 1);
    _dp = 0;
    _ki = (X0 - 1);
    _k0 = (X0 - 1);
    _k1 = (X0 - 1);
    disp_level ();
    disp_integ ();
    disp_range ();
}


void Ebu_r128_disp::disp_level (void)
{
    int   kr, kp, dp;
    float vr, vp;

    if (_bmodeM->state ())
    {
	vr = _vm;
	vp = _pm;
    }
    else
    {
	vr = _vs;
	vp = _ps;
    }

    kr = (int)(floor (db2pix (vr) + 0.5f));
    kp = (int)(floor (db2pix (vp) + 0.5f));
    if (kr < X0) kr = X0 - 1;
    if (kr > X1) kr = X1;
    kp += 1;
    dp = kp - kr;
    if (dp > 3) dp = 3;
    if (kp > X1 + 3) dp = 0;
    XSetForeground (dpy (), dgc (), XftColors [C_DISP_PK]->pixel);
    if (_dp > 0)
    {
        scaled_XPutImage (dpy (), win (), dgc (), _imag0, _kp - _dp, 0, _kp - _dp, Y2, _dp, H2);
    }
    if (kr > _kr)
    {
        scaled_XPutImage (dpy (), win (), dgc (), _imag1, _kr, 0, _kr, Y2, kr - _kr, H2); 
    }	    
    else if (kr < _kr)
    {
        scaled_XPutImage (dpy (), win (), dgc (), _imag0, kr, 0, kr, Y2, _kr - kr, H2); 
    }	    
    if (dp > 0)
    {
        scaled_XFillRectangle (dpy (), win (), dgc (), kp - dp, Y2, dp, H2 - 1);
    }
    _kr = kr;
    _kp = kp;
    _dp = dp;
}


void Ebu_r128_disp::disp_integ (void)
{
    char s [32];
    int  k;

    if (_vi < -80)
    {
	_t_integ->set_text ("I : --");
	k = X0 - 1;
    }
    else
    {
	if (_abs) sprintf (s, "I : %5.1lf LUFS", _vi);
	else      sprintf (s, "I : %5.1lf LU", _vi + 23);
        _t_integ->set_text (s);
	k = (int)(floor (db2pix (_vi) + 0.5f));
	if (k < X0) k = X0 - 1;
	if (k > X1) k = X1;
    }
    if (k != _ki)
    {
        plot_level  (_ki, k, Y4, H4);
	_ki = k;
    }
} 	     


void Ebu_r128_disp::disp_range (void)
{
    char s [32];
    int  k0, k1;

    if (_v1 < -80.0f)
    {
	_t_range->set_text ("LRA : --");
	k0 = k1 = X0 - 1;
    }
    else
    {
        sprintf (s, "LRA : %5.1lf LU", _v1 - _v0);
        _t_range->set_text (s);
	k0 = (int)(floor (db2pix (_v0)));
	k1 = (int)(ceil  (db2pix (_v1)));
	if (k0 < X0) k0 = X0;
	if (k0 > X1) k0 = X1;
	if (k1 < X0) k1 = X0;
	if (k1 > X1) k1 = X1;
    }
    if ((k0 != _k0) || (k1 != _k1))
    {
        plot_range (k0, k1, Y3, H3);
	_k0 = k0;
	_k1 = k1;
    }
} 	     


void Ebu_r128_disp::plot_level (int z0, int z1, int y, int dy)
{
    XSetFunction (dpy (), dgc (), GXcopy);
    if      (z1 > z0) scaled_XPutImage (dpy (), win (), dgc (), _imag1, z0 + 1, 0, z0 + 1, y, z1 - z0, dy); 
    else if (z1 < z0) scaled_XPutImage (dpy (), win (), dgc (), _imag0, z1 + 1, 0, z1 + 1, y, z0 - z1, dy);
}


void Ebu_r128_disp::plot_range (int z0, int z1, int y, int dy)
{
    XSetFunction (dpy (), dgc (), GXcopy);
    scaled_XPutImage (dpy (), win (), dgc (), _imag0, 0, 0, 0, y, IMGS, dy);
    if (z0 >= X0) scaled_XPutImage (dpy (), win (), dgc (), _imag1, z0, 0, z0, y, z1 - z0 + 1, dy); 
}
