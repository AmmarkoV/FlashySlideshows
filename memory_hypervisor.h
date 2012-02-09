#ifndef MEMORY_HYPERVISOR_H_INCLUDED
#define MEMORY_HYPERVISOR_H_INCLUDED

extern int SignalGPUFull;

int RAM_Memory_can_accomodate(unsigned int newfile);
int CreatePicturesIfNeeded();
int DestroyPicturesIfNeeded();
int ManagePicturesCreationMemory();

int GPU_Memory_can_accomodate(unsigned int newfile);
int LoadPicturesIfNeeded();
int UnLoadPicturesIfNeeded();
int ManagePicturesLoadingMemory();


#endif // MEMORY_HYPERVISOR_H_INCLUDED
