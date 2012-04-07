PROG := syobon

GAMEDATA ?= /usr/share/$(PROG)

CXX ?= g++
CXXFLAGS ?= -O2 -pipe

SRCS := DxLib.cpp loadg.cpp main.cpp
OBJS := $(SRCS:%.cpp=%.o)

CXXFLAGS += $(shell pkg-config sdl --cflags)
CXXFLAGS += -D_FONTCONFIG -I/usr/include/fontconfig
LIBS := $(shell pkg-config sdl --libs) -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL_gfx -lfontconfig


all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) -DGAMEDATA=\"$(GAMEDATA)\" -c $<

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(PROG) *~

install:
	install -D $(PROG) $(DESTDIR)/usr/bin/$(PROG)
	install -dm 0755 $(DESTDIR)/usr/share/$(PROG)/{BGM,res,SE}
	install -dm 0755 $(DESTDIR)/usr/share/doc/$(PROG)
	install -D -m 0644 BGM/* -t $(DESTDIR)/usr/share/$(PROG)/BGM
	install -D -m 0644 res/* -t $(DESTDIR)/usr/share/$(PROG)/res
	install -D -m 0644 SE/* -t $(DESTDIR)/usr/share/$(PROG)/SE
	install -D -m 0644 icon.ico -t $(DESTDIR)/usr/share/$(PROG)
	install -D -m 0644 {README,AUTHORS,NEWS} -t $(DESTDIR)/usr/share/doc/$(PROG)
	install -D -m 0644 open-syobon.desktop $(DESTDIR)/usr/share/applications/open-syobon.desktop
