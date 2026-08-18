#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED

#include <unistd.h>

long timeval_diff ( struct timeval *difference, struct timeval *end_time, struct timeval *start_time );

/* Was already defined in environment.cpp , it just never had a declaration to go with
   it , and visuals/dynamic_background.cpp needs it to find the shaders directory */
int FileExists(const char *fname);

int LoadStockTexturesAndSounds();
int UnLoadStockTexturesAndSounds();

int PrintOpenGLDebugMsg();
int PrintPictureLoadingMsg();
extern int DEBUG_TRACE;
int PrintDevMsg();

void DisableScreenSaver();
void EnableScreenSaver();

int EmmitDialogWarning(char * inpt);

unsigned int GetSystemFreeMemory();
unsigned int GetGraphicsCardMemory();

int QueryAndSaveGPUAndSystemCapabilities();

unsigned int UploadPhotoToMyloader(char * photo);
unsigned int UploadCurrentPhotoToMyloader();
#endif // ENVIRONMENT_H_INCLUDED
