#include "load_images.h"
#include "memory_hypervisor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wx/image.h>

#include "slideshow.h"
#include "load_textures.h"
#include "image_sensing.h"

#define PPMREADBUFLEN 256


struct Picture *star=0;
struct Picture *heart=0;
struct Picture *loading=0;
struct Picture *loading_texture=0;
struct Picture *failed=0;
struct Picture *background=0;
struct Picture *picture_frame=0;


int PictureCreationPending(struct Picture * picturedata)
{
  if ( picturedata == 0 ) return 1;
  if ( picturedata == loading ) return 1;
  return 0;
}


int PictureLoadingPending(struct Picture * picturedata)
{
  /*In case picture creation is pending , picture loading is not pending ( not the next thing to do )*/
  if ( picturedata == 0 ) return 0;
  if ( picturedata == loading ) return 0;
  return picturedata->marked_for_rgbdata_loading;
}



int PreparePictureForImage(struct Picture * pic,unsigned int width,unsigned int height,unsigned int depth)
{
    fprintf(stderr,"PreparePictureForImage Called for an image %u x %u : %u \n",width,height,depth);

    if ((pic->rgb_data!=0) || (pic->rgb_data_size!=0))
     {
         /* CLEAR MEMORY TO ALLOCATE IT LATER*/
         fprintf(stderr,"Picture Structure is dirty\n");
         frame.system.usedRAM-=pic->rgb_data_size;

         if ( pic->rgb_data!=0 ) { free (pic->rgb_data);
                                   pic->rgb_data=0;
                                 }
         pic->rgb_data_size=0;
     }

    if ( ( width == 0 ) || ( height == 0 ) || ( depth == 0 ) ) {  fprintf(stderr,"PreparePictureForImage only cleared allocated memory\n");
                                                                  return 1; }

    if ((pic->rgb_data==0) || (pic->rgb_data_size==0))
     {
         /* ALLOCATE ENOUGH MEMORY FOR THE RAW IMAGE */
         pic->overflow_guard=OVERFLOW_GUARD_BYTES;
         //if (frame.system.maxRAM < frame.system.usedRAM + width*height*depth ) { fprintf(stderr,"System memory bounds reached"); return 0; }

         frame.system.lastTexture=width*height*depth;
         if ( !RAM_Memory_can_accomodate(frame.system.lastTexture) ) { return 0; }

         frame.system.usedRAM+=frame.system.lastTexture;
         pic->rgb_data=(char *) malloc( ( width*height*depth ) + depth );

         if  ( pic->rgb_data <=0 )
          {
            fprintf(stderr,"Error allocating memory for raw image \n");
            pic->rgb_data=0;
            pic->rgb_data_size=0;
            return 0;
          }
     }

     pic->width=width;
     pic->height=height;
     pic->depth=depth;
     pic->rgb_data_size=width*height*depth;
     frame.system.lastTexture=pic->rgb_data_size;
     fprintf(stderr,"PreparePictureForImage completed\n");

    return 1;
}

inline wxString _U(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}


unsigned int PickPictureRescaleRatio(unsigned int start_width,unsigned int start_height)
{
  float ratio_width=start_width/1024;
  float ratio_height=start_height/768;

  unsigned int scale_percentage=40;

  if (( ratio_width==0 ) | ( ratio_height==0 )) {  } else
  if ( ratio_width < ratio_height ) { scale_percentage=(unsigned int) 100/ratio_width; } else
                                    { scale_percentage=(unsigned int) 100/ratio_height; }
  return scale_percentage;
}

int WxLoadJPEG(char * filename,struct Picture * pic)
{

 wxImage new_img;
 new_img.LoadFile(_U(filename));

 unsigned int width = new_img.GetWidth();
 unsigned int height = new_img.GetHeight();
 unsigned int rescale_ratio=PickPictureRescaleRatio(width,height);

 width  = (unsigned int) (width  * rescale_ratio / 100);
 height = (unsigned int) (height * rescale_ratio / 100);

 new_img.Rescale(width,height);

 unsigned char * data = new_img.GetData();

       /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>
          PREPARE MEMORY TO HOLD DATA
          >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
       if ( PreparePictureForImage(pic,width,height,3) == 0 ) { fprintf(stderr,"PreparePictureForImage could not prepare memory! \n"); return 0; }
       /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

 memcpy(pic->rgb_data,data,width*height*3);

 return 1;
}


int LoadPicture(char * filename,struct Picture * pic)
{
  fprintf(stderr,"Loading picture\n");
  if ( pic == 0 ) { fprintf(stderr,"Error loading picture (%s) , accomodation structure is not allocated\n",filename); return 0; }



    if ( WxLoadJPEG(filename,pic) )       {/* PICTURE IS LOADED ALL IS DONE :) */ } /* MUCH MORE ELEGANT WAY TO LOAD PICTURES :P */
                                     else { pic->failed_to_load=1; fprintf(stderr,"Failed to open and load picture \n"); return 0; }



  GetInterestingAreasList(pic);


  if ( pic->overflow_guard != OVERFLOW_GUARD_BYTES ) { fprintf(stderr,"Memory Overflow at Picture structure \n "); }

  if ( loading != 0 ) { pic->gl_rgb_texture=loading->gl_rgb_texture;
                        pic->marked_for_texture_loading=1;
                        /* Signal picture ready for texture creating*/
                      }

  frame.total_images_loaded++; /* ADDED HERE */

 return 1;
}



struct Picture * CreatePicture(char * filename,unsigned int force_load)
{
  struct Picture * new_picture=0;

  new_picture = (struct Picture *) malloc( sizeof( struct Picture ) );
  if (  new_picture == 0 ) { fprintf(stderr,"Could not allocate memory for picture %s \n",filename); return 0; }


    new_picture->rgb_data=0;
    new_picture->rgb_data_size=0;

    new_picture->failed_to_load=0;
    new_picture->marked_for_texture_loading=0;
    new_picture->marked_for_texture_removal=0;
    new_picture->marked_for_rgbdata_loading=1;
    new_picture->marked_for_rgbdata_removal=0;
    new_picture->gl_rgb_texture=0;

    new_picture->height=0,new_picture->width=0,new_picture->depth=0;

    new_picture->transparency=1.0;

    new_picture->position.ok=0;
    new_picture->position.size_x=6;  new_picture->position.size_y=4.5;
    new_picture->position.x=0.0;  new_picture->position.y=0.0; new_picture->position.z=0.0;
    new_picture->position.heading=0.0; new_picture->position.yaw=0.0; new_picture->position.pitch=0.0;

  new_picture->time_viewed=0;
  new_picture->times_viewed=0;


  new_picture->overflow_guard=OVERFLOW_GUARD_BYTES;

  if (force_load==1)
   {
    LoadPicture(filename,new_picture);
    frame.total_images_loaded++;
   }
  return new_picture;
}


int UnLoadPicture(struct Picture * pic)
{
  fprintf(stderr,"Unloading Picture\n");
  frame.system.usedRAM-=pic->rgb_data_size;
  frame.gpu.usedRAM-=pic->width*pic->height*4;
  frame.total_images_loaded--;

  pic->rgb_data_size=0;
  if ( pic->rgb_data != 0 ) { free(pic->rgb_data); pic->rgb_data=0; }

  glDeleteTextures(1,&pic->gl_rgb_texture);


 return 1;
}

int PositionPicture(struct Picture * pic,unsigned int place)
{
  if (pic==0) {return 0;}

  unsigned int x,y;
  if ( frame.images_per_line == 0 ) { return 0; }
  y = (unsigned int ) ( place / frame.images_per_line);
  x = (unsigned int ) ( place % frame.images_per_line);


  pic->position.ok=1;

  pic->position.x=(x*14);
  pic->position.x=pic->position.x-14;
  pic->position.x=(-1)*pic->position.x;

  pic->position.y=(y*12);
  pic->position.y=pic->position.y-12;

  pic->position.z=-5;

  pic->position.heading=0;
  pic->position.yaw=0;
  pic->position.pitch=0;

  pic->position.size_x=6;
  pic->position.size_y=4.5;

  return 1;
}


int DestroyPicture(struct Picture * pic)
{
  UnLoadPicture(pic);
  if ( pic != 0 ) free(pic);
  return 1;
}
