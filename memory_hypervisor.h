#ifndef MEMORY_HYPERVISOR_H_INCLUDED
#define MEMORY_HYPERVISOR_H_INCLUDED

int RAM_Memory_can_accomodate(unsigned int newfile);
int LoadPicturesIfNeeded();
int UnLoadPicturesIfNeeded();
int ManagePicturesMemory();

int GPU_Memory_can_accomodate(unsigned int newfile);
int LoadTexturesIfNeeded();
int UnLoadTexturesIfNeeded();
int ManageTexturesMemory();


#endif // MEMORY_HYPERVISOR_H_INCLUDED
