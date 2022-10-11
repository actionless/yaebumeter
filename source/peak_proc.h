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


#ifndef __PEAK_PROC_H
#define __PEAK_PROC_H


#include <zita-resampler/resampler.h>


class Peak_proc
{
public:

    Peak_proc (void);
    ~Peak_proc (void);

    void  init (int nchan, float fsamp);
    void  reset (void);
    void  process (int nfram, float *input []);
    void  flush (void);

    float readpeak (void)
    {
	_reset = true;
	return _vpeak;
    }

private:

    enum { MAXCHAN = 16, BUFSIZE = 64 };

    void findpeak (void);

    Resampler         _resamp;
    int               _nchan;
    float             _vpeak;
    bool              _reset;
    float            *_rinp;
    float            *_rout;
    float             _zdcf [MAXCHAN];
    float             _wdcf;
};


#endif
