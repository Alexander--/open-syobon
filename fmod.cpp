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
