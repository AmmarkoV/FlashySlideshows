#include "memory_hypervisor.h"


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      Pictures are malloced structures that carry RGB data on them
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
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
