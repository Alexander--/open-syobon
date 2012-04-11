ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -DGAMEDATA=\"$(pkgdatadir)\"
AM_CXXFLAGS = $(SDL_CFLAGS)
LDADD = $(SDL_LIBS)

bin_PROGRAMS = syobon
syobon_SOURCES = main.cpp DxLib.cpp loadg.cpp
nobase_pkgdata_DATA = $(PACKAGE).png BGM/* SE/* res/*.PNG

if USE_FONTCONFIG
nobase_pkgdata_DATA += res/sazanami-gothic.ttf
LDADD += -lfontconfig
endif

if DESKTOP
pixmapdir = $(datarootdir)/pixmaps
pixmap_DATA = $(PACKAGE).png
entrydir = $(datarootdir)/applications
entry_DATA = $(PACKAGE).desktop
endif
