#include "memory_hypervisor.h"
#include "slideshow.h"
#include <stdio.h>

/*



*/




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      Pictures are malloced structures that carry RGB data on them
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
int RAM_Memory_can_accomodate(unsigned int newfile)
{
  if ( frame.gpu.maxRAM < newfile + frame.gpu.usedRAM )
    {
       fprintf(stderr,"It looks like there is no free memory on GPU :P \n");
       return 0;
    }
   if (frame.system.maxRAM < frame.system.usedRAM + newfile)
    {
       fprintf(stderr,"System memory bounds reached"); return 0;
    }
 return 1;
}


int LoadPicturesIfNeeded()
{
  return 0;
}

int UnLoadPicturesIfNeeded()
{
  return 0;
}

int ManagePicturesMemory()
{
   if ( UnLoadPicturesIfNeeded() == 0 ) { return 0; }
   if ( LoadPicturesIfNeeded() == 0 ) { return 0; }
   return 1;
}







/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>




                         SYSTEM DATA /\




                          GPU DATA   \/







>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */












/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   Textures are GPU allocated structures that carry RGB data on them
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

int GPU_Memory_can_accomodate(unsigned int newfile)
{
  if ( frame.gpu.maxRAM < newfile + frame.gpu.usedRAM )
    {
       fprintf(stderr,"It looks like there is no free memory on GPU :P \n");
       return 0;
    }

 return 1;
}

int LoadTexturesIfNeeded()
{
  return 0;
}

int UnLoadTexturesIfNeeded()
{
  return 0;
}

int ManageTexturesMemory()
{
   if ( UnLoadTexturesIfNeeded() == 0 ) { return 0; }
   if ( LoadTexturesIfNeeded() == 0 ) { return 0; }
   return 1;
}

