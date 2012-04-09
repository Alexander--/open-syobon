//This file is a reverse engineered "DxLib.h" to the extent that
//Syobon Action uses it. Minor functions are just replaced with SDL
//counterparts.

#include "config.h"

#define USE_FMOD 0 /* XXX: works better in SDL. fmod doesn't seem to mix several sounds */

#include <SDL.h>
#include <SDL_rotozoom.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include "joyconfig.h"

#define TRUE 1
#define FALSE 0
#define byte unsigned char

void ScreenFlip(void);
//#define ScreenFlip() SDL_Flip(screen)
#define GetNowCount() SDL_GetTicks()

//UNIMPLEMENTED - macro substitution
#define SetFontThickness(f);
#define StopSoundMem(s);
#define ChangeVolumeSoundMem(s, v);

//Sound
#if USE_FMOD
#include "fmod.h"
extern FMOD_SYSTEM *fmod_system;
#define DX_PLAYTYPE_BACK 0 /* XXX: not used */
FMOD_SOUND * LoadSoundMem(const char *soundfile);
FMOD_SOUND * LoadMusicMem(const char *soundfile);
void PlaySoundMem(FMOD_SOUND *sound);
#define CheckSoundMem(s) !s
#else
#define DX_PLAYTYPE_LOOP -1
#define DX_PLAYTYPE_BACK 0
#define PlaySoundMem(s, l) Mix_PlayChannel(-1, s, l)
#define LoadSoundMem(s) Mix_LoadWAV(s)
#define LoadMusicMem(m) Mix_LoadMUS(m)
#define CheckSoundMem(s) !s
#endif

int DxLib_Init(int windowmode);

//Main screen
extern SDL_Surface *screen;

//Fonts
#define FONT_MAX 64
extern TTF_Font *font[FONT_MAX];

//Strings & fonts
#define DX_FONTTYPE_NORMAL 0
#define DX_FONTTYPE_EDGE 1
void SetFontSize(byte size);
void ChangeFontType(byte type);
void DrawString(int a, int b, const char *x, Uint32 c);
void DrawFormatString(int a, int b, Uint32 color, const char *str, ...);

//Key Aliases
#define KEY_INPUT_ESCAPE SDLK_ESCAPE
#define KEY_INPUT_LEFT SDLK_LEFT
#define KEY_INPUT_RIGHT SDLK_RIGHT
#define KEY_INPUT_DOWN SDLK_DOWN
#define KEY_INPUT_UP SDLK_UP
#define KEY_INPUT_F1 SDLK_F1
#define KEY_INPUT_O SDLK_o
#define KEY_INPUT_K SDLK_k
#define KEY_INPUT_Z SDLK_z
#define KEY_INPUT_RETURN SDLK_RETURN
#define KEY_INPUT_SPACE SDLK_SPACE
#define KEY_INPUT_1 SDLK_1
#define KEY_INPUT_2 SDLK_2
#define KEY_INPUT_3 SDLK_3
#define KEY_INPUT_4 SDLK_4
#define KEY_INPUT_5 SDLK_5
#define KEY_INPUT_6 SDLK_6
#define KEY_INPUT_7 SDLK_7
#define KEY_INPUT_8 SDLK_8
#define KEY_INPUT_9 SDLK_9
#define KEY_INPUT_0 SDLK_0

extern SDL_Joystick* joystick;

void UpdateKeys();
byte ProcessMessage();
byte CheckHitKey(int key);
byte WaitKey();

#define GetColor(r, g, b) SDL_MapRGB(screen->format, r, g, b)

//Uint32 GetColor(byte r, byte g, byte b);

#define DrawGraph(a, b, mx, z) DrawGraphZ(a, b, mx)
void DrawGraphZ(int a, int b, SDL_Surface * mx);

#define DrawTurnGraph(a, b, mx, z) DrawTurnGraphZ(a, b, mx)
void DrawTurnGraphZ(int a, int b, SDL_Surface * mx);

//#define DrawVertTurnGraph(x, y, e, a, mx, z) DrawRotaGraphZ(x, y, a, mx)
void DrawVertTurnGraph(int a, int b, SDL_Surface * mx);

SDL_Surface *DerivationGraph(int srcx, int srcy, int width, int height,
			     SDL_Surface * src);

//Noticably different than the original
SDL_Surface *LoadGraph(const char *filename);
