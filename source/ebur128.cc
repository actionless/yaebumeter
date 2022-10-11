// -------------------------------------------------------------------------
//
//    Copyright (C) 2010-2011 Fons Adriaensen <fons@linuxaudio.org>
//    
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// -------------------------------------------------------------------------


#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <math.h>
#include "ebu_r128_proc.h"
#include "peak_proc.h"
#include "audiofile.h"


static void help (void)
{
    fprintf (stderr, "\nebur128 %s\n", VERSION);
    fprintf (stderr, "(C) 2010-2011 Fons Adriaensen  <fons@linuxaudio.org>\n");
    fprintf (stderr, "Measure integrated loudness, loudness range, and peak level\n");
    fprintf (stderr, "of an audio file according to EBU recommendation R-128.\n");
    fprintf (stderr, "Usage: ebur128 <options> <input file>.\n");
    fprintf (stderr, "Options:\n");
    fprintf (stderr, "  --help   Display this text.\n");
    fprintf (stderr, "  --lufs   Use absolute units.\n");
    fprintf (stderr, "  --full   Print internal values as well.\n");
    fprintf (stderr, "  --prob   Write cumulative probability file.\n");
    exit (1);
}


enum { HELP, LUFS, FULL, PROB };


static struct option options [] = 
{
    { "help",  0, 0, HELP  },
    { "lufs",  0, 0, LUFS  },
    { "full",  0, 0, FULL  },
    { "prob",  0, 0, PROB  },
    { 0, 0, 0, 0 }
};


static bool lufs = false;
static bool full = false;
static bool prob = false;


static void procoptions (int ac, char *av [])
{
    int k;

    while ((k = getopt_long (ac, av, "", options, 0)) != -1)
    {
	switch (k)
	{
        case '?':
	case HELP:
	    help ();
	    break;
	case LUFS:
	    lufs = true;
	    break;
	case FULL:
	    full = true;
	    break;
	case PROB:
	    prob = true;
	    break;
 	}
    }
}


int main (int ac, char *av [])
{
    Audiofile      Ainp;
    Ebu_r128_proc  R128proc;
    Peak_proc      Peakproc;
    int            i, k, nm, ns, nchan, bsize;
    float          fsamp, km, ks, v;
    float          *p, *inpb, *data [2];
    const int      *hm, *hs;
    FILE           *F;

    procoptions (ac, av);
    if (ac - optind < 1)
    {
        fprintf (stderr, "Missing arguments, try --help.\n");
	return 1;
    }
    if (ac - optind > 1 )
    {
        fprintf (stderr, "Too many arguments, try --help.\n");
	return 1;
    }

    if (Ainp.open_read (av [optind]))
    {
	fprintf (stderr, "Can't open input file '%s'.\n", av [optind]);
	return 1;
    }

    nchan = Ainp.chan ();
    fsamp = Ainp.rate ();
    if (nchan > 2)
    {
	fprintf (stderr, "Input file must be mono or stereo.\n");
	Ainp.close ();
	return 1;
    }

    bsize = Ainp.rate () / 5;
    inpb = new float [nchan * bsize];
    if (nchan > 1)
    {
	data [0] = new float [bsize];
	data [1] = new float [bsize];
    }
    else
    {
	data [0] = inpb;
	data [1] = inpb;
    }

    R128proc.init (nchan, fsamp);
    R128proc.integr_start ();
    Peakproc.init (nchan, fsamp);
    while (true)
    {
	k = Ainp.read (inpb, bsize);
	if (k == 0) break;
	if (nchan > 1)
	{
	    p = inpb;
	    for (i = 0; i < k; i++)
	    {
		data [0][i] = *p++;
		data [1][i] = *p++;
	    }
	}
	R128proc.process (k, data);
	Peakproc.process (k, data);
    }
    Peakproc.flush ();
    Ainp.close ();
    if (nchan > 1)
    {
	delete[] data [0];
	delete[] data [1];
    }
    delete[] inpb;

    if (lufs)
    {
        printf ("Integrated loudness:   %6.1lf LUFS\n", R128proc.integrated ());
        printf ("Loudness range:        %6.1lf LU\n", R128proc.range_max () - R128proc.range_min ());
        printf ("Peak level             %6.1lf dB\n", 20 * log10f (Peakproc.readpeak () + 1e-10f));
	if (full)
	{
            printf ("Integrated threshold:  %6.1lf LUFS\n", R128proc.integ_thr ());
            printf ("Range threshold:       %6.1lf LUFS\n", R128proc.range_thr ());
            printf ("Range min:             %6.1lf LUFS\n", R128proc.range_min ());
            printf ("Range max:             %6.1lf LUFS\n", R128proc.range_max ());
            printf ("Momentary max:         %6.1lf LUFS\n", R128proc.maxloudn_M ());
            printf ("Short term max:        %6.1lf LUFS\n", R128proc.maxloudn_S ());
	}
    }
    else
    {
        printf ("Integrated loudness:   %6.1lf LU\n", R128proc.integrated () + 23.0f);
        printf ("Loudness range:        %6.1lf LU\n", R128proc.range_max () - R128proc.range_min ());
        printf ("Peak level             %6.1lf dB\n", 20 * log10f (Peakproc.readpeak () + 1e-10f));
	if (full)
	{
            printf ("Integrated threshold:  %6.1lf LU\n", R128proc.integ_thr () + 23.0f);
            printf ("Range threshold:       %6.1lf LU\n", R128proc.range_thr () + 23.0f);
            printf ("Range min:             %6.1lf LU\n", R128proc.range_min () + 23.0f);
            printf ("Range max:             %6.1lf LU\n", R128proc.range_max () + 23.0f);
            printf ("Momentary max:         %6.1lf LU\n", R128proc.maxloudn_M () + 23.0f);
            printf ("Short term max:        %6.1lf LU\n", R128proc.maxloudn_S () + 23.0f);
	}
    }

    if (prob)
    {
        km = R128proc.hist_M_count ();
        ks = R128proc.hist_S_count ();
	if ((km < 10) || (ks < 10))
	{
	    fprintf (stderr, "Insufficient data for probability file.\n");
	}
	else
	{
	    F = fopen ("ebur128-prob", "w");
	    if (F)
	    {
		hm = R128proc.histogram_M ();
		hs = R128proc.histogram_S ();
		nm = 0;
		ns = 0;
		for (i = 0; i <= 750; i++)
		{
		    nm += hm [i];
		    ns += hs [i];
		    v = 0.1f * (i - 700);
		    if (! lufs) v += 23.0f;
		    fprintf (F, "%5.1lf %8.6lf %8.6lf\n", v, nm / km, ns / ks);
		}
                fclose (F);
	    }
	    else
	    {
		fprintf (stderr, "Can't open probability data file.\n");
	    }
	}
    }

    return 0;
}
