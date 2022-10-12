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


#include <stdlib.h>
#include <stdio.h>
#include <clthreads.h>
#include <sys/mman.h>
#include <signal.h>
#include <string>
#include "styles.h"
#include "jclient.h"
#include "mainwin.h"
#include <math.h>


#define NOPTS 5
#define CP (char *)


XrmOptionDescRec options [NOPTS] =
{
    {CP"-h",   CP".help",     XrmoptionNoArg,   CP"true" },
    {CP"-s",   CP".server",   XrmoptionSepArg,  0        },
    {CP"-g",   CP".geometry", XrmoptionSepArg,  0        },
    {CP"-cl",  CP".connectl", XrmoptionSepArg,  0        },
    {CP"-cr",  CP".connectr", XrmoptionSepArg,  0        }
};


static Jclient  *jclient = 0;
static Mainwin  *mainwin = 0;


static void help (void)
{
    fprintf (stderr, "\n%s-%s\n\n", PROGNAME, VERSION);
    fprintf (stderr, "  (C) 2010-2011 Fons Adriaensen  <fons@linuxaudio.org>\n\n");
    fprintf (stderr, "Options:\n");
    fprintf (stderr, "  -h               Display this text\n");
    fprintf (stderr, "  -name <name>     Jack client name\n");
    fprintf (stderr, "  -s <server>      Jack server name\n");
    fprintf (stderr, "  -g <geometery>   Window position\n");
    fprintf (stderr, "  -cl <jack_name>  Connect to Left channel\n");
    fprintf (stderr, "  -cr <jack_name>  Connect to Right channel\n");
    exit (1);
}


static void sigint_handler (int)
{
    signal (SIGINT, SIG_IGN);
    mainwin->stop ();
}


const char* GetEnv( const char* tag, const char* def=nullptr ) noexcept {
  const char* ret = std::getenv(tag);
  return ret ? ret : def;
}


const float DetectDisplayScale () {
	// @TODO: implement actual dpi detection using X dpy
	//        instead of using 1.0 as a fallback:
	float scale = 1.0;

	const char * scale_env = GetEnv("EBUMETER_SCALE");
	printf("Going to detect DPI from EBUMETER_SCALE env var...\n");
	if (scale_env) {
		scale = std::stof(scale_env);
		printf("%s\n", scale_env);
	}
	printf("DPI detection finished: %f\n", scale);
	return scale;
}


int main (int ac, char *av [])
{
	float scale = DetectDisplayScale();
    X_resman       xresman;
    X_display     *display;
    X_handler     *handler;
    X_rootwin     *rootwin;
    int            ev, xp, yp, xs, ys;

    xresman.init (&ac, av, CP PROGNAME, options, NOPTS);
    if (xresman.getb (".help", 0)) help ();

    display = new X_display (xresman.get (".display", 0));
    if (display->dpy () == 0)
    {
	fprintf (stderr, "Can't open display.\n");
        delete display;
	return 1;
    }

    xp = (int)roundf(yp = (100 * scale));
    xs = (int)roundf((Mainwin::XSIZE + 2) * scale);
    ys = (int)roundf((Mainwin::YSIZE + 22) * scale);
    xresman.geometry (".geometry", display->xsize (), display->ysize (), 1, xp, yp, xs, ys);
    if (styles_init (display, &xresman, SHARED, scale))
    {
		delete display;
		return 1;
    }

    jclient = new Jclient (
		xresman.rname (),
		xresman.get (".server", 0),
		xresman.get (".connectl", 0),
		xresman.get (".connectr", 0)
	);
    rootwin = new X_rootwin (display);
    mainwin = new Mainwin (rootwin, &xresman, xp, yp, jclient, scale);
    rootwin->handle_event ();
    handler = new X_handler (display, mainwin, EV_X11);
    handler->next_event ();
    XFlush (display->dpy ());

    ITC_ctrl::connect (jclient, EV_EXIT, mainwin, EV_EXIT);

    if (mlockall (MCL_CURRENT | MCL_FUTURE)) fprintf (stderr, "Warning: memory lock failed.\n");
    signal (SIGINT, sigint_handler); 

    do
    {
	ev = mainwin->process ();
	if (ev == EV_X11)
	{
	    rootwin->handle_event ();
	    handler->next_event ();
	}
    }
    while (ev != EV_EXIT);	

    mainwin->x_unmap ();
    styles_fini (display);
    delete jclient;
    delete handler;
    delete rootwin;
    delete display;
   
    return 0;
}



