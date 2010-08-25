#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

void StartSoundLibrary();

void AddSoundBufferForLoad(char * bufferpath);
int LoadSoundBuffers();

void SoundLibrary_PlaySound(unsigned int num);
#endif // SOUND_H_INCLUDED
