## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(datadir)/bookreader/ui
ui_DATA = bookreader.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(BOOKREADER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = bookreader

bookreader_SOURCES = \
	main.c \
	bookreader.h \
	bookreader.c	

bookreader_LDFLAGS = \
	-Wl,--export-dynamic

bookreader_LDADD = $(BOOKREADER_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/bookreader

