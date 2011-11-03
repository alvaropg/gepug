## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(GEPUB_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libgepub.la


libgepub_la_SOURCES = \
	gepub-archive.c \
	gepub-doc.c 	\
	gepub-utils.c

libgepub_la_LDFLAGS = 

libgepub_la_LIBADD = $(GEPUB_LIBS)

include_HEADERS = \
	gepub.h 	\
	gepub-archive.h \
	gepub-doc.h 	\
	gepub-utils.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgepub-0.1.pc

EXTRA_DIST = \
	libgepub-0.1.pc.in
