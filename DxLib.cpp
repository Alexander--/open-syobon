#include "DxLib.h"

#if USE_FMOD
FMOD_SYSTEM *fmod_system;

FMOD_SOUND * LoadSoundMem(const char *soundfile)
{
	FMOD_SOUND *snd; 
	FMOD_System_CreateSound(fmod_system, soundfile, FMOD_DEFAULT, 0, &snd);
	return (snd);
}

FMOD_SOUND * LoadMusicMem(const char *soundfile)
{
	FMOD_SOUND *snd;
	FMOD_System_CreateStream(fmod_system, soundfile, FMOD_DEFAULT, 0, &snd);
	return (snd);
}

void PlaySoundMem(FMOD_SOUND *sound)
{
	FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, sound, false, 0);
}
#endif

SDL_Joystick* joystick;

//Main screen
SDL_Surface *screen;
SDL_Surface *scale_screen;

bool keysHeld[SDLK_LAST];
int DxLib_Init(int windowmode)
{
    /*setlocale (LC_ALL, "");
        #bindtextdomain (PACKAGE, LOCALEDIR);
        #textdomain (PACKAGE);
*/

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
	printf("Unable to init SDL: %s\n", SDL_GetError());
	return -1;
    }

	SDL_Surface *icon = IMG_Load(GAMEDATA"/"PACKAGE".png");
        if (icon) {
                SDL_WM_SetIcon(icon, NULL);
                SDL_FreeSurface(icon);
        }


	int flags = SDL_HWSURFACE | SDL_DOUBLEBUF;

	if (!windowmode)
	{
		flags |= SDL_FULLSCREEN;
	}

    if (!(screen = SDL_SetVideoMode(480 /*(int)fmax/100 */ , 420 /*(int)fymax/100 */ , 32, flags))) 
	{
		SDL_Quit();
		return -1;
    }

	/*
	 * Verify if we're running fullscreen.
	 */
	int scaling = false;

	if (!windowmode)
	{
#if 0
		const SDL_VideoInfo *vinfo = SDL_GetVideoInfo();

		if (vinfo)
		{
			if (vinfo->current_w == 480 && vinfo->current_h == 420)
			{
				/*
				 * Windows Vista provides 480x420,
				 * Windows 7 does not (on my setup at least).
				 * So we have to scale manually.
				 */
				scaling = false;
				/*
				 * XXX: well, this way of checking doesn't work somehow
				 */
			}
		}
#endif
		scaling = false; /* XXX: set to true when we manage to get SDL to switch to fullscreen on windows 7 */
	}

	if (scaling)
	{
		SDL_Rect** modes;
		int i;

		/*
		 * Reallocate again but a software surface as we
		 * don't need double buffering.
		 */
		SDL_FreeSurface(screen);
		screen = SDL_SetVideoMode(480, 420, 32, 0); /* XXX */

		/* Get available fullscreen/hardware modes */
		modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);

		/* Check if there are any modes available */
		if (modes)
		{
			int scale_w, scale_h;

			/* Check if our resolution is restricted */
			if (modes == (SDL_Rect**)-1) {
				printf("all modes available\n");
				/* all modes are available.. this shouldn't happen */
			}
			else
			{
				//printf("Available Modes\n");
				for (i = 0; modes[i]; i++)
				{
					//printf("  %d x %d\n", modes[i]->w, modes[i]->h);
					if (modes[i]->w == 640 && modes[i]->h == 480)
					{
						scale_w = 640;
						scale_h = 480;
						break;
					}
					else if (modes[i]->w == 800 && modes[i]->h == 600)
					{
						scale_w = 800;
						scale_h = 600;
						break;
					}
					else
					{
						/* XXX: error out */
					}
				}
			}

			if (scale_screen = SDL_SetVideoMode(scale_w, scale_h, 32, SDL_HWSURFACE | SDL_FULLSCREEN))
			{
				//printf("scaled to %u x %u\n", scale_w, scale_h);
			}
			/* XXX */
		}
	}

	//SDL_WM_SetCaption("Syobon Action (しょぼんのアクション)", NULL);
	SDL_WM_SetCaption("Syobon Action", NULL);
	SDL_ShowCursor(SDL_DISABLE);

	//Initialize font
	if (TTF_Init() == -1) {
		printf("Unable to init SDL_ttf: %s\n", TTF_GetError());
		return -1;
	}

	if (font == NULL) {
		printf("Unable to load font: %s\n", TTF_GetError());
		return -1;
	}
#if USE_FMOD
	if (FMOD_System_Create(&fmod_system) != FMOD_OK)
	{
		printf("Unable to initialize sound.");
		return (-1);
	}
	if (FMOD_System_Init(fmod_system, 32, 0, 0) != FMOD_OK) /* 32 software channels */
	{
		printf("Unable to initialize FMOD.");
		return (-1);
	}
#else
    //Audio Rate, Audio Format, Audio Channels, Audio Buffers
#define AUDIO_CHANNELS 2
    if (Mix_OpenAudio(22050, AUDIO_S16SYS, AUDIO_CHANNELS, 1024)) {
	  printf("Unable to init SDL_mixer: %s\n", Mix_GetError());
	  return -1;
    }
#endif
    //Try to get a joystick
    joystick = SDL_JoystickOpen(0);

    for (int i = 0; i < SDLK_LAST; i++)
	keysHeld[i] = false;
    for (int i = 0; i < FONT_MAX; i++)
	font[i] = NULL;
    srand(time(NULL));

    return 0;
}



//Fonts
byte fontsize = 0;
TTF_Font *font[FONT_MAX];

//Strings
void SetFontSize(byte size)
{
    fontsize = size;
    if (font[size] == NULL) {
#ifdef USE_FONTCONFIG
	FcPattern *pat, *match;
	FcResult result;
	char* file;

	pat = FcPatternCreate();
	FcPatternAddString(pat, FC_FAMILY, (unsigned char*)"Sazanami Gothic");
	FcConfigSubstitute(NULL, pat, FcMatchPattern);
	FcDefaultSubstitute(pat);
	match = FcFontMatch(NULL, pat, &result);

	FcPatternGetString(match, FC_FILE, 0, (FcChar8 **) &file);
	font[size] = TTF_OpenFont(file, size);

	FcPatternDestroy(match);
	FcPatternDestroy(pat);
#else
	font[size] = TTF_OpenFont(GAMEDATA"/res/sazanami-gothic.ttf", size);
#endif
	if (font[size] == NULL) {
	    printf("Unable to load font: %s\n", TTF_GetError());
	    exit(1);
	}
    }
}

byte fontType = DX_FONTTYPE_NORMAL;
void ChangeFontType(byte type)
{
    fontType = type;
}

void DrawString(int a, int b, const char *x, Uint32 c)
{
    SDL_Color color = { c >> 16, c >> 8, c };
    SDL_Surface *rendered = TTF_RenderUTF8_Solid(font[fontsize], x, color);
    if (fontType == DX_FONTTYPE_EDGE) {
	SDL_Color blk = { 0, 0, 0 };
	SDL_Surface *shadow = TTF_RenderUTF8_Solid(font[fontsize], x, blk);
	DrawGraphZ(a - 1, b - 1, shadow);
	DrawGraphZ(a, b - 1, shadow);
	DrawGraphZ(a + 1, b - 1, shadow);
	DrawGraphZ(a - 1, b, shadow);
	DrawGraphZ(a + 1, b, shadow);
	DrawGraphZ(a - 1, b + 1, shadow);
	DrawGraphZ(a, b + 1, shadow);
	DrawGraphZ(a + 1, b + 1, shadow);
	SDL_FreeSurface(shadow);
    }
    DrawGraphZ(a, b, rendered);
    SDL_FreeSurface(rendered);
}

void DrawFormatString(int a, int b, Uint32 color, const char *str, ...)
{
    va_list args;
    char *newstr = new char[strlen(str) + 16];
    va_start(args, str);
    vsprintf(newstr, str, args);
    va_end(args);
    DrawString(a, b, newstr, color);
    delete newstr;
}

//void DrawFormatString(int a, int b, int c

//Key Aliases
#define KEY_INPUT_ESCAPE SDLK_ESCAPE

bool ex = false;

void UpdateKeys()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_KEYDOWN:
	    keysHeld[event.key.keysym.sym] = true;
	    break;
	case SDL_KEYUP:
	    keysHeld[event.key.keysym.sym] = false;
	    break;
	case SDL_JOYAXISMOTION:
	    if(event.jaxis.which == 0)
	    {
		if(event.jaxis.axis == JOYSTICK_XAXIS)
		{
		    if(event.jaxis.value < -4096) keysHeld[SDLK_LEFT] = true;
		    else if(event.jaxis.value > 4096) keysHeld[SDLK_RIGHT] = true;
		    else {
			keysHeld[SDLK_LEFT] = false;
			keysHeld[SDLK_RIGHT] = false;
		    }
		}
		else if(event.jaxis.axis == JOYSTICK_YAXIS)
		{
		    if(event.jaxis.value < -4096) keysHeld[SDLK_UP] = true;
		    else if(event.jaxis.value > 4096) keysHeld[SDLK_DOWN] = true;
		    else {
			keysHeld[SDLK_UP] = false;
			keysHeld[SDLK_DOWN] = false;
		    }
		}
	    }
	    break;

	case SDL_JOYBUTTONDOWN:
		if (event.jbutton.which == 0)
		{
			if (event.jbutton.button == 0 ||
				event.jbutton.button == 1 ||
				event.jbutton.button == 2
				)
			{
				keysHeld[SDLK_UP] = true;
			}
		}
		break;

	case SDL_JOYBUTTONUP:
		if (event.jbutton.which == 0)
		{
			if (event.jbutton.button == 0 ||
				event.jbutton.button == 1 ||
				event.jbutton.button == 2
				)
			{
				keysHeld[SDLK_UP] = false;
			}
		}
		break;

	case SDL_QUIT:
	    ex = true;
	    break;
	}
    }
}

byte ProcessMessage()
{
    return ex;
}

byte CheckHitKey(int key)
{
    return keysHeld[key];
}

byte WaitKey()
{
    SDL_Event event;
    while (true) {
	while (SDL_PollEvent(&event))
	    if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN)
		return event.key.keysym.sym;
    }
}

/*Uint32 GetColor(byte r, byte g, byte b)
{
    return r << 8 * 3 | g << 8 * 2 | b << 8 | 0xFF;
}*/

void DrawGraphZ(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a;
        offset.y = b;
        SDL_BlitSurface(mx, NULL, screen, &offset);
    }
}

void DrawTurnGraphZ(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
		SDL_Surface *flipped;
        SDL_Rect offset;
        offset.x = a;
        offset.y = b;

		/*
		 * zoomSurface() is bugged, try to copy manually [zapek]
		 */
		if (mx->format->BitsPerPixel == 32)
		{
			int lockmx = false;
			int lockflipped = false;

			flipped = SDL_CreateRGBSurface(mx->flags, mx->w, mx->h, 
				mx->format->BitsPerPixel,
				mx->format->Rmask,
				mx->format->Gmask,
				mx->format->Bmask,
				mx->format->Amask);

			if (SDL_MUSTLOCK(mx))
			{
				lockmx = true;
			}
			if (SDL_MUSTLOCK(flipped))
			{
				lockflipped = true;
			}

			if (lockmx)
			{
				SDL_LockSurface(mx);
			}

			if (lockflipped)
			{
				SDL_LockSurface(flipped);
			}

			Uint32 *src = (Uint32 *)mx->pixels;
			Uint32 *dst = (Uint32 *)flipped->pixels;

			int i, j;

			for (i = 0; i < mx->h; i++)
			{
				for (j = 0; j < mx->w; j++)
				{
					dst[i * mx->w + j] = src[(i + 1) * mx->w - j - 1];
				}
			}
			
			if (lockflipped)
			{
				SDL_UnlockSurface(flipped);
			}
			if (lockmx)
			{
				SDL_UnlockSurface(mx);
			}
		}
		else
		{
			flipped = zoomSurface(mx, -1, 1, 0); /* XXX: moves one pixel bottom */
		}
		SDL_SetColorKey(flipped, SDL_SRCCOLORKEY,
                SDL_MapRGB(flipped->format, 9 * 16 + 9, 255, 255));
        SDL_BlitSurface(flipped, NULL, screen, &offset);
        SDL_FreeSurface(flipped);
    }
}

void DrawVertTurnGraph(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a - mx->w / 2;
        offset.y = b - mx->h / 2;

        SDL_Surface *flipped = rotozoomSurface(mx, 180, 1, 0);
        SDL_SetColorKey(flipped, SDL_SRCCOLORKEY,
                SDL_MapRGB(flipped->format, 9 * 16 + 9, 255, 255));
        SDL_BlitSurface(flipped, NULL, screen, &offset);
        SDL_FreeSurface(flipped);
    }
}

SDL_Surface *DerivationGraph(int srcx, int srcy, int width, int height,
			     SDL_Surface * src)
{
    SDL_Surface *img =
	SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
			     screen->format->BitsPerPixel,
			     src->format->Rmask, src->format->Bmask,
			     src->format->Gmask, src->format->Amask);

    SDL_Rect offset;
    offset.x = srcx;
    offset.y = srcy;
    offset.w = width;
    offset.h = height;

    SDL_BlitSurface(src, &offset, img, NULL);
    SDL_SetColorKey(img, SDL_SRCCOLORKEY,
		    SDL_MapRGB(img->format, 9 * 16 + 9, 255, 255));
    return img;
}

//Noticably different than the original
SDL_Surface *LoadGraph(const char *filename)
{
    SDL_Surface *image = IMG_Load(filename);

    if (image) {
	return image;
    } else {
	fprintf(stderr, "Error: Unable to load %s\n", filename);
	exit(1);
    }
}

void ScreenFlip()
{
	if (scale_screen)
	{
		float scaleFactor;
		SDL_Rect offset;

		if ((float(screen->w) / screen->h) > (float(scale_screen->w) / scale_screen->h))
		{
			// letterbox; compute vertical offset
			scaleFactor = float(scale_screen->w) / screen->w;
			offset.x = 0;
			offset.y = int((float(scale_screen->h) - (scaleFactor * screen->h)) / 2 + 0.5f);
		}
		else
		{
			// pillarbox; compute horizontal offset
			scaleFactor = float(scale_screen->h) / screen->h;
			offset.x = int((float(scale_screen->w) - (scaleFactor * screen->w)) / 2 + 0.5f);
			offset.y = 0;
		}

		SDL_Surface * temp = zoomSurface(screen, scaleFactor, scaleFactor, 1);
		SDL_BlitSurface(temp, NULL, scale_screen, &offset);
		SDL_Flip(scale_screen);
	}
	else
	{
		SDL_Flip(screen);
	}
}
