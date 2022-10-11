#  --------------------------------------------------------------------------
#
#  Copyright (C) 2010-2018 Fons Adriaensen <fons@linuxaudio.org>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#  --------------------------------------------------------------------------


PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
SHARED ?= $(PREFIX)/share/ebumeter

VERSION = 0.4.2
CPPFLAGS += -MMD -MP -DVERSION=\"$(VERSION)\"
CXXFLAGS += -O2 -Wall -ffast-math -pthread
#CXXFLAGS += -march=native


all:	ebur128 ebumeter


EBUR128_O = ebur128.o ebu_r128_proc.o peak_proc.o audiofile.o dither.o 
ebur128:	LDLIBS += -lsndfile -lzita-resampler
ebur128:	$(EBUR128_O)
	$(CXX) $(LDFLAGS) -o $@ $(EBUR128_O) $(LDLIBS)
$(EBUR128_O):
-include $(EBUR128_O:%.o=%.d)


EBUMETER_O = ebumeter.o jclient.o mainwin.o styles.o ebu_r128_proc.o \
	ebu_r128_disp.o peak_proc.o png2img.o button.o guiclass.o 
ebumeter:	CPPFLAGS += $(shell pkgconf --cflags freetype2)
ebumeter:	CPPFLAGS += -DSHARED=\"$(SHARED)\"
ebumeter:	LDLIBS += -lclthreads -lclxclient -lzita-resampler \
	-lpthread -ljack -lpng -lXft -lpng -lX11 -lrt
ebumeter:	$(EBUMETER_O)
	$(CXX) $(LDFLAGS) -o $@ $(EBUMETER_O) $(LDLIBS)
$(EBUMETER_O):
-include $(EBUMETER_O:%.o=%.d)


install:	all
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(SHARED)
	install -m 755 ebur128 $(DESTDIR)$(BINDIR)
	install -m 755 ebumeter $(DESTDIR)$(BINDIR)
	rm -rf $(DESTDIR)$(SHARED)/*
	install -m 644 ../share/* $(DESTDIR)$(SHARED)


uninstall:
	rm -f  $(DESTDIR)$(BINDIR)/ebumeter
	rm -f  $(DESTDIR)$(BINDIR)/ebur128
	rm -rf $(DESTDIR)$(SHARED)


clean:
	rm -f *~ *.o *.a *.d *.so ebumeter ebur128

