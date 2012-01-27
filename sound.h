#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

//SoundLibrary_PlaySound(SLIDESHOW_START);
enum SOUND_NAMES
{
   LOADED_PICTURE=0,
   UNABLE_TO_MOVE,
   SLIDESHOW_START,
   SLIDESHOW_STOP,
   ZOOM,
   UNZOOM
};



void StartSoundLibrary();

void AddSoundBufferForLoad(char * bufferpath);
int LoadSoundBuffers();

void SoundLibrary_PlaySound(unsigned int num);
void SoundLibrary_PlaySoundPos(unsigned int num,float x,float y,float z);
#endif // SOUND_H_INCLUDED
