PROG := syobon

GAMEDATA ?= .

CXX ?= g++
CXXFLAGS ?= -O2 -pipe

SRCS := DxLib.cpp loadg.cpp main.cpp
OBJS := $(SRCS:%.cpp=%.o)

CXXFLAGS += $(shell pkg-config sdl --cflags)
LIBS := $(shell pkg-config sdl --libs) -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL_gfx


all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) -DGAMEDATA=\"$(GAMEDATA)\" -c $<

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(PROG) *~
