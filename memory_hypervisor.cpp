#include "memory_hypervisor.h"
#include "slideshow.h"

int MinPictureThatIsVisible()
{
  unsigned int min_picture=0;
  if ( frame.active_image_place  < frame.images_per_line * 3 ) {} else
                                                               { min_picture=frame.active_image_place-frame.images_per_line * 3;  }
  return min_picture;
}

int MaxPictureThatIsVisible()
{
  unsigned int max_picture=frame.active_image_place + frame.images_per_line * 3;
  if ( max_picture >= frame.total_images ) { max_picture=frame.total_images-1; }
  return max_picture;
}

int PicturesNeededToBeLoaded()
{
  return 0;
}


/*



*/




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
