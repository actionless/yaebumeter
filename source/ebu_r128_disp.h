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


#ifndef __EBU_R128_DISP_H
#define	__EBU_R128_DISP_H


#include <clxclient.h>
#include "guiclass.h"



class Ebu_r128_disp : public X_window, public X_callback
{
public:

    Ebu_r128_disp (X_window *parent, X_callback *callb, int xp, int yp, XftColor *bg);
    ~Ebu_r128_disp (void);
    Ebu_r128_disp (const Ebu_r128_disp&);
    Ebu_r128_disp& operator=(const Ebu_r128_disp&);

    static int  init (X_display *disp, const char *shared);
    static void fini (X_display *disp);

    void set_scale (bool abs, bool ext);
    void set_level (float vm, float pm, float vs, float ps);
    void set_integ (float v_i);
    void set_range (float v_0, float v_1);
    void set_peak (float v_p);

    enum { PAUSE, START, RESET };

private:

    enum
    {
        IDIV = 12,
	IMAR = 20,
	X0   = IMAR,
	X1   = X0 + 27 * IDIV + 1,
	IMGS = X1 + IMAR,
	RMAR = 220,
        XS   = X1 + RMAR,
	Y0   = 3,
	Y1   = 15,
	Y2   = 23,
	H2   = 8,
	Y3   = 35,
	H3   = 4,
	Y4   = 42,
	H4   = 4,
	Y5   = 52,
        YS   = 75
    };

    enum 
    {
	BMODEM,
        BMODES,
	BSCALE09,
	BSCALE18,
	BSCALELU,
	BSCALEFS,
        BPAUSE,
        BSTART,
        BRESET,
        BPEAK1,
        BPEAK2
    };

    void handle_callb (int type, X_window *W, XEvent *E);
    void handle_event (XEvent *E);
    void expose (XExposeEvent *E);

    void disp_level (void);
    void disp_integ (void);
    void disp_range (void);
    void plot_level (int z0, int z1, int y, int dy);
    void plot_range (int z0, int z1, int y, int dy);

    float db2pix (float v) { return _pixref + _db2pix * (v + 23.0f); }

    static int loadimg (X_display *disp, const char *sdir, const char *file, XImage **imag);

    X_callback *_callb;

    XImage  *_scale;
    XImage  *_ticks;
    XImage  *_imag0;
    XImage  *_imag1;

    Pbutt1  *_bmodeM;
    Pbutt1  *_bmodeS;
    Pbutt1  *_bscale09;
    Pbutt1  *_bscale18;
    Pbutt1  *_bscaleLU;
    Pbutt1  *_bscaleFS;
    Pbutt1  *_bpause;
    Pbutt1  *_bstart;
    Pbutt1  *_breset;
    Pbutt1  *_bpeak1;

    float    _vm;
    float    _pm;
    float    _vs;
    float    _ps;
    float    _vi;
    float    _v0;
    float    _v1;

    float    _db2pix;
    int      _pixref;
    int      _count;
    bool     _abs;
    int      _kr;
    int      _kp;
    int      _dp;
    int      _ki;
    int      _k0;
    int      _k1;
	 
    X_textip  *_t_unit;
    X_textip  *_t_integ;
    X_textip  *_t_range;

    static XImage  *_ihscale09r;
    static XImage  *_ihscale09a;
    static XImage  *_ihscale18r;
    static XImage  *_ihscale18a;
    static XImage  *_ihticks;
    static XImage  *_ihmeter09;
    static XImage  *_ihmeter18;
    static XImage  *_ihmeterbg;
    static XImage  *_ibscale09;
    static XImage  *_ibscale18;
    static XImage  *_ibscaleLU;
    static XImage  *_ibscaleFS;
    static XImage  *_ibmodeM;
    static XImage  *_ibmodeS;
    static XImage  *_ibpause;
    static XImage  *_ibstart;
    static XImage  *_ibreset;
    static XImage  *_ibpeak1;
};


#endif
