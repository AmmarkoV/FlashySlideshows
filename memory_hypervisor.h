#ifndef MEMORY_HYPERVISOR_H_INCLUDED
#define MEMORY_HYPERVISOR_H_INCLUDED

extern int SignalGPUFull;

int RAM_System_Memory_can_accomodate(unsigned int newfile);
int GPU_Memory_can_accomodate(unsigned int newfile);

int MasterMemoryStrategist();


int CreatePicturesIfNeeded();
int DestroyPicturesIfNeeded();

int LoadPicturesIfNeeded();
int UnLoadPicturesIfNeeded(unsigned int clear_gpu_ram,unsigned int clear_system_ram);

int LoadTexturesIfNeeded(int count_only);

int ManagePicturesCreationMemory();
int ManagePicturesLoadingMemory();
int ExecuteMemoryStrategyPlanOnSystemMemory();

#endif // MEMORY_HYPERVISOR_H_INCLUDED
