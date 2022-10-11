// ------------------------------------------------------------------------
//
//  Copyright (C) 2013-2014 Fons Adriaensen <fons@linuxaudio.org>
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

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "peak_proc.h"


Peak_proc::Peak_proc (void) :
    _nchan (0),
    _vpeak (0),
    _reset (false),
    _rinp (0),
    _rout (0),
    _wdcf (0)
{
}


Peak_proc::~Peak_proc (void)
{
    delete[] _rinp;
    delete[] _rout;
}


void Peak_proc::init (int nchan, float fsamp)
{
    if (_nchan)
    {
        delete[] _rinp;
	delete[] _rout;
	_nchan = 0;
	_rinp = 0;
	_rout = 0;
    }    
    if (nchan > 0)
    {
	int r = (fsamp >= 64e3f) ? 2 : 4;
	_wdcf = 12.57f / (fsamp * r);  // 2 Hz
	_nchan = nchan;
	_rinp = new float [BUFSIZE * nchan];
	_rout = new float [BUFSIZE * nchan];
	_resamp.setup (1, r, nchan, 24, 1.0f);
	reset ();
    }
}


void Peak_proc::reset (void)
{
    if (_nchan > 0)
    {
	memset (_zdcf, 0, MAXCHAN * sizeof (float));
        _resamp.reset ();
        _resamp.inp_data = 0;
        _resamp.inp_count = _resamp.inpsize ();
        _resamp.out_data = 0;
        _resamp.out_count = 9999;
        _resamp.process ();
        _vpeak = 0;
        _reset = false;
    }
}


void Peak_proc::process (int nfram, float *input [])
{
    int     i, j, k;
    float   *ipp [MAXCHAN], *p, *q;

    if (_reset)
    {
	_vpeak = 0;
	_reset = false;
    }
    if (_nchan == 0) return;
    for (j = 0; j < _nchan; j++)
    {
        ipp [j] = input [j];
    }	
    while (nfram)
    {
	k = (nfram > BUFSIZE) ? BUFSIZE : nfram;
        for (j = 0; j < _nchan; j++)
        {
	    p = ipp [j];
  	    q = _rinp + j;
   	    for (i = 0; i < k; i++)
	    {
	        q [i * _nchan] = p [i];
	    }
	    ipp [j] += k;
	}
	nfram -= k;
	_resamp.inp_data = _rinp;
	_resamp.inp_count = k;
	findpeak ();
    }
}


void Peak_proc::flush (void)
{
    if (_nchan == 0) return;
    _resamp.inp_data = 0;
    _resamp.inp_count = _resamp.inpsize ();
    findpeak ();
}


void Peak_proc::findpeak (void)
{
    int    i, j, k;
    float  m, s, w, z;
    float  *p;

    w = _wdcf;
    m = _vpeak;
    while (_resamp.inp_count)
    {
        _resamp.out_data = _rout;
        _resamp.out_count = BUFSIZE;
        _resamp.process ();
	k = BUFSIZE - _resamp.out_count;
        for (j = 0; j < _nchan; j++)
        {
	    p = _rout + j;
   	    z = _zdcf [j];
	    for (i = 0; i < k; i++)
	    {
	        s = p [i * _nchan];
//		printf ("%d %4d %6.1lf\n", j, i, s);
	        z += w * (s - z) + 1e-20f;
	        if (z * s < 0) s -= z;
	        s = fabsf (s);
	        if (s > m) m = s;
	    }
	    _zdcf [j] = z;
	}
    }
    _vpeak = m;	
}


