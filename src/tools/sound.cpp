/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "sound.h"
#include "../slideshow.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

// Maximum data buffers we will need.
#define NUM_BUFFERS 200
// Maximum emissions we will need.
#define NUM_SOURCES 16



unsigned int total_loaded_buffers=0;
//Filenames!
char * filenames[NUM_BUFFERS];
// Buffers hold sound data.
ALuint Buffers[NUM_BUFFERS];
// Sources are points of emitting sound.
ALuint Sources[NUM_SOURCES];
// Position of the source sounds.
ALfloat SourcesPos[NUM_SOURCES][3];
// Velocity of the source sounds.
ALfloat SourcesVel[NUM_SOURCES][3];


// Position of the listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
// Velocity of the listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

void AddSoundBufferForLoad(char * bufferpath)
{
  if (total_loaded_buffers>=NUM_BUFFERS) { return; }
  filenames[total_loaded_buffers]=(char *) malloc(strlen(bufferpath)+1);
  strcpy(filenames[total_loaded_buffers],bufferpath);
  ++total_loaded_buffers;
}

/*
 * void SetListenerValues()
 *
 *	We already defined certain values for the listener, but we need
 *	to tell OpenAL to use that data. This function does just that.
 */
void SetListenerValues()
{
	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

/*
 * ALboolean LoadALData()
 *
 *	This function will load our sample data from the disk using the alut
 *	utility and send the data into OpenAL as a buffer. A source is then
 *	also created to play that buffer.
 */
int LoadSoundBuffers()
{
    int theerror=0;
	// Variables to load into.
    if (total_loaded_buffers==0) { fprintf(stderr,"No Sounds to load!\n");
                                   return 0; }

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into buffers.
	alGenBuffers(total_loaded_buffers, Buffers);
	if(alGetError() != AL_NO_ERROR) { fprintf(stderr,"Error Generating Buffers!\n");
                                      return 0; }

    for (unsigned int i=0; i<total_loaded_buffers; i++)
      {
         alutLoadWAVFile((ALbyte *)filenames[i], &format, &data, &size, &freq, &loop);
         theerror=alGetError();
	     if(theerror != AL_NO_ERROR) { fprintf(stderr,"Error (%u) loading Wav sound Buffer %u %s\n",theerror,i,filenames[i]);   }

         alBufferData(Buffers[i], format, data, size, freq);
	     theerror=alGetError();
	     if(theerror != AL_NO_ERROR) { fprintf(stderr,"Error (%u) loading Wav sound Buffer %u %s\n",theerror,i,filenames[i]);   }

         alutUnloadWAV(format, data, size, freq);
	     theerror=alGetError();
	     if(theerror != AL_NO_ERROR) { fprintf(stderr,"Error (%u) loading Wav sound Buffer %u %s\n",theerror,i,filenames[i]);   }

	     free(filenames[i]); /* Release memory for filename */
      }
	// Bind buffers into audio sources.


	alGenSources(NUM_SOURCES, Sources);
    theerror=alGetError();
	if(theerror != AL_NO_ERROR) { fprintf(stderr,"Error Generating Sources (%u)!\n",theerror);
                                  return 0; }


    for (unsigned int i=0; i<total_loaded_buffers; i++)
      {
         /*alGenSources( 1, &Sources[i] );
         theerror=alGetError();
         if(theerror != AL_NO_ERROR) { cout<<"Error Generating Sources ("<<theerror<<")!\n";
                                       return false; }   */
         alSourcei (Sources[i], AL_BUFFER,   Buffers[i]   );
         alSourcef (Sources[i], AL_PITCH,    1.0f              );
         alSourcef (Sources[i], AL_GAIN,     1.0f              );
	     alSourcefv(Sources[i], AL_POSITION, SourcesPos[i]);
	     alSourcefv(Sources[i], AL_VELOCITY, SourcesVel[i]);
	     alSourcei (Sources[i], AL_LOOPING,  AL_FALSE           );
      }
	// Bind buffers into audio sources.


	// Do another error check and return.
	if(alGetError() != AL_NO_ERROR) return 0;

	SetListenerValues();

    return 1;
}


/*
 * void KillALData()
 *
 *	We have allocated memory for our buffers and sources which needs
 *	to be returned to the system. This function frees that memory.
 */
void KillALData()
{
	alDeleteBuffers(NUM_BUFFERS, Buffers);
	alDeleteSources(NUM_SOURCES, Sources);
	alutExit();
}

void StartSoundLibrary()
{
    alutInit(NULL, 0);
	alGetError();


	// Setup an exit procedure.
	atexit(KillALData);
}


void SoundLibrary_PlaySound(unsigned int num)
{
   if (!frame.enable_sound_effects) { return; }
   if (num<total_loaded_buffers) { alSourcePlay(Sources[num]);}
}

void SoundLibrary_LoopSound(unsigned int num)
{
   if (!frame.enable_sound_effects) { return; }
   if (num<total_loaded_buffers) { alSourcei (Sources[num], AL_LOOPING,  AL_TRUE          );
                                   alSourcePlay(Sources[num]); }
}

void SoundLibrary_PlaySoundPos(unsigned int num,float x,float y,float z)
{
   if (!frame.enable_sound_effects) { return; }
   if (num<total_loaded_buffers) {  SourcesPos[num][0]=x;
                                    SourcesPos[num][1]=y;
                                    SourcesPos[num][2]=z;
                                    alSourcefv(Sources[num], AL_POSITION, SourcesPos[num]);
                                    alSourcePlay(Sources[num]); }
}
