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

#include "load_images.h"
#include "../tools/jpegexiforient_embed.h"
#include "memory_hypervisor.h"
#include "../layouts/layout_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wx/image.h>
#include <wx/string.h>

#include "../Codecs/codecs.h"

#include "load_textures.h"
#include "../tools/image_sensing.h"
#include "../tools/environment.h"

struct Picture *label=0;
struct Picture *star=0;
struct Picture *heart=0;
struct Picture *play_img;
struct Picture *pause_img;
struct Picture *loading=0;
struct Picture *loading_texture=0;
struct Picture *failed=0;
struct Picture *background=0;
struct Picture *picture_frame=0;

int STRECH_IMAGES=0;


inline wxString _U(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

unsigned int GetWidthQuality(unsigned int quality)
{
  unsigned int chosen_width=1920;
  switch (quality)
  {
    case 0: chosen_width=320; break;
    case 1: chosen_width=640; break;
    case 2: chosen_width=800; break;
    case 3: chosen_width=1024; break;
    case 4: chosen_width=1560; break;
    case 5: chosen_width=2048; break;
    case 6: chosen_width=frame.gpu.maximum_frame_dimension_size;  break;
    case 7: chosen_width=frame.gpu.maximum_frame_dimension_size;  break;
    case 8: chosen_width=frame.gpu.maximum_frame_dimension_size;  break;
    //case 6: chosen_width=1920; break;
    //case 7: chosen_width=2048; break;
    //case 8: chosen_width=2560; break;

  };

  if ( frame.gpu.maximum_frame_dimension_size<=chosen_width )
    {  //Make sure graphics card can take it..
        chosen_width=frame.gpu.maximum_frame_dimension_size-2;
    }


  if ( 4096<=chosen_width )
    {  //Make sure graphics card can take it..
        chosen_width=4096;
    }

  return chosen_width;
}

unsigned int GetHeightQuality(unsigned int quality)
{
  unsigned int chosen_height=GetWidthQuality(quality)*3/4;
  if ( frame.gpu.maximum_frame_dimension_size<=chosen_height )
    {  //Make sure graphics card can take it..
        chosen_height=frame.gpu.maximum_frame_dimension_size-1;
    }

  return chosen_height;
}


float PickPictureRescaleRatio(unsigned int start_width,unsigned int start_height)
{
  unsigned int best_height = GetHeightQuality(frame.quality_setting);
  unsigned int best_width = GetWidthQuality(frame.quality_setting);

  if ( (start_width<frame.gpu.maximum_frame_dimension_size) &&
       (start_height<frame.gpu.maximum_frame_dimension_size) )
          {   if ( ( start_width < best_width ) && (start_height < best_height) ) { return 100; }  }

  float ratio_width=(float)start_width/best_width;
  float ratio_height=(float)start_height/best_height;

  float scale_percentage=30;

  if (( ratio_width==0 ) | ( ratio_height==0 )) {  } else
  if ( ratio_width < ratio_height ) { scale_percentage=(float) 100/ratio_width;  } else
                                    { scale_percentage=(float) 100/ratio_height; }

  if ( frame.gpu.maximum_frame_dimension_size < (start_width  * scale_percentage / 100) )
    {
        ratio_width=(float)start_width/(frame.gpu.maximum_frame_dimension_size-1);
        scale_percentage=(float) 100/ratio_width;
    }

  if ( frame.gpu.maximum_frame_dimension_size < (start_height  * scale_percentage / 100) )
    {
        ratio_height=(float)start_height/(frame.gpu.maximum_frame_dimension_size-1);
        scale_percentage=(float) 100/ratio_height;
    }

  return scale_percentage;
}


int FixOpenGLPictureSize(struct Picture * pic)
{
  if (STRECH_IMAGES)
   {
     pic->position.size_x=6;
     pic->position.size_y=4.5;
     return 1;
   }


  if(pic->height==0) { return 0; }

  if (pic->width > pic->height)
   {
     float size_ratio = pic->width / 6;
     if (size_ratio==0) { return 0; }
     pic->position.size_x=6;  pic->position.size_y=pic->height/size_ratio;
   } else
  if (pic->width < pic->height)
   {
     float size_ratio = pic->height / 4.5;
     if (size_ratio==0) { return 0; }
     pic->position.size_y=4.5;  pic->position.size_x=pic->width/size_ratio;
   }


  return 1;
}

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
  return picturedata->system.marked_for_rgbdata_loading;
}

int PictureFailed(struct Picture * picturedata)
{
  if ( picturedata == 0 ) return 1;
  if ( picturedata == loading ) return 0;
  return picturedata->system.marked_for_rgbdata_loading;
}

int PrintPictureData(struct Picture * picturedata)
{
   if ( picturedata == 0 ) { fprintf(stderr,"Null picture structure \n"); return 0; }
   if (!PrintPictureLoadingMsg()) { return 0; }
   fprintf(stderr,"_____________________________________\n");
   fprintf(stderr,"Directory list index : %u \n",picturedata->directory_list_index);
//   PrintDirectoryListItem(picturedata->directory_list_index);
   fprintf(stderr,"Image size : %ux%u:%u ratio %0.2f",picturedata->width,picturedata->height,picturedata->depth,(float)picturedata->width/picturedata->height);
   fprintf(stderr," OpenGL size : %0.2fx%0.2f ratio %0.2f",picturedata->position.size_x,picturedata->position.size_y,(float)picturedata->position.size_x/picturedata->position.size_y);

   fprintf(stderr,"\ntime_viewed : %u , times_viewed : %u , transparency : %0.2f \n",picturedata->time_viewed,picturedata->times_viewed,picturedata->transparency);

   fprintf(stderr," Status\n");
   fprintf(stderr,"Failed to Load : %u , Thumbnail texture loaded : %u , Texture loaded : %u \n",picturedata->failed_to_load,picturedata->gpu.thumbnail_texture_loaded,picturedata->gpu.texture_loaded);
   fprintf(stderr,"Mark tex load : %u , Mark tex rm : %u , Mark RGB load: %u Mark RGB rm : %u \n",picturedata->gpu.marked_for_texture_loading,picturedata->gpu.marked_for_texture_removal,picturedata->system.marked_for_rgbdata_loading,picturedata->system.marked_for_rgbdata_removal);
   fprintf(stderr,"_____________________________________\n");
 return 1;
}

int PreparePictureForImage(struct Picture * pic,unsigned int width,unsigned int height,unsigned int depth)
{
    if (PrintPictureLoadingMsg()) fprintf(stderr,"PreparePictureForImage , image %u x %u : %u ",width,height,depth);

    if ((pic->system.rgb_data!=0) || (pic->system.rgb_data_size!=0))
     {
         /* CLEAR MEMORY TO ALLOCATE IT LATER*/
         fprintf(stderr,"dirty Picture struct ");
         frame.system.usedRAM-=pic->system.rgb_data_size;

         if ( pic->system.rgb_data!=0 ) {
                                          free (pic->system.rgb_data);
                                          pic->system.rgb_data=0;
                                        }
         pic->system.rgb_data_size=0;
     }

    if ( ( width == 0 ) || ( height == 0 ) || ( depth == 0 ) ) {  fprintf(stderr,"PreparePictureForImage only cleared allocated memory\n"); return 1; }
    if ( ( width < 50 ) || ( height < 50 ) ) {  fprintf(stderr,"This picture is very small (%u,%u) , skipping it ,NEED A BETER IMPLEMENTATION TO PREVENT BLINKING\n",width,height);
                                                 pic->system.marked_for_rgbdata_loading=0;  /* THIS MAY CAUSE A BLINK NO USE IN RETRYING , IT HAS FAILED*/
                                                 return 0; }

    if ((pic->system.rgb_data==0) && (pic->system.rgb_data_size==0))
     {
         /* ALLOCATE ENOUGH MEMORY FOR THE RAW IMAGE */
         pic->system.overflow_guard=OVERFLOW_GUARD_BYTES;
         //if (frame.system.maxRAM < frame.system.usedRAM + width*height*depth ) { fprintf(stderr,"System memory bounds reached"); return 0; }

         frame.system.lastTexture=width*height*depth;
         if ( !RAM_System_Memory_can_accomodate(frame.system.lastTexture) ) { fprintf(stderr,"RAM Memory cannot accomodate %u bytes \n",(unsigned int) frame.system.lastTexture); return 0; }

         frame.system.usedRAM+=frame.system.lastTexture;
         pic->system.rgb_data=(char *) malloc( (width+1)*(height+1)*depth * sizeof(char) );

         if  ( pic->system.rgb_data <=0 )
          {
            fprintf(stderr,"Error allocating memory for raw image \n");
            pic->system.rgb_data=0;
            pic->system.rgb_data_size=0;
            return 0;
          }
     }

     pic->width=width;
     pic->height=height;
     pic->depth=depth;
     pic->system.rgb_data_size=width*height*depth;
     frame.system.lastTexture=pic->system.rgb_data_size;
     if (PrintPictureLoadingMsg()) fprintf(stderr,"ok\n");

    return 1;
}


int GetJPEGExifOrientation(char * filename,struct Picture * pic)
{
 fprintf(stderr,"Getting Orientation for `%s` ",filename);
 unsigned int orientation = GetOrientationOfFile(filename);
 fprintf(stderr,"= %u",orientation);
 pic->rotate=0; pic->mirror=0;

 /*            1        2       3      4       5            6           7          8

              888888  888888      88  88       8888888888  88                  88  8888888888
              88          88      88  88       88  88      88  88          88  88      88  88
              8888      8888    8888  8888     88          8888888888  8888888888          88
              88          88      88  88
              88          88  888888  888888                                                        */

 if (orientation==0) {  } else
 if (orientation==1) { fprintf(stderr," - Normal Orientation"); } else
 if (orientation==2) { fprintf(stderr," - Inverted , but Normal Orientation");  pic->mirror=1;  }else
 if (orientation==3) { fprintf(stderr," - Rotated 180 degs but Normal Orientation"); pic->rotate=180; pic->target_rotate=180; }else
 if (orientation==4) { fprintf(stderr," - Rotated 180 degs but Inverted Orientation");  pic->mirror=1; pic->rotate=180; pic->target_rotate=180; }else
 if (orientation==5) { fprintf(stderr," - Rotated 270 degs but Inverted Orientation");  pic->mirror=1; pic->rotate=270; pic->target_rotate=270; }else
 if (orientation==6) { fprintf(stderr," - Rotated 270 degs but Normal Orientation"); pic->rotate=270; pic->target_rotate=270; }else
 if (orientation==7) { fprintf(stderr," - Rotated 90 degs but Inverted Orientation"); pic->mirror=1; pic->rotate=90; pic->target_rotate=90;  }else
 if (orientation==8) { fprintf(stderr," - Rotated 90 degs but Normal Orientation"); pic->rotate=90; pic->target_rotate=90; } else
                     {  }

 pic->default_rotate=pic->rotate;
 fprintf(stderr,"\n");

return 1;
}

int WxLoadJPEG(char * filename,struct Picture * pic)
{
 wxImage new_img;
 if ( new_img.LoadFile(_U(filename)) )
 {
 unsigned int width = new_img.GetWidth();
 unsigned int height = new_img.GetHeight();
 pic->initial_width=width;
 pic->initial_height=height;
 unsigned int rescale_ratio=PickPictureRescaleRatio(width,height);

 width  = (unsigned int) (width  * (float) rescale_ratio / 100);
 height = (unsigned int) (height * (float) rescale_ratio / 100);

 new_img.Rescale(width,height);

 if (pic->is_jpeg)
  {
    if (!GetJPEGExifOrientation(filename,pic))
     {
         fprintf(stderr,"Could not get Exif orientation for file %s \n",filename);
     }
  }


 unsigned char * data = new_img.GetData();

       /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>
          PREPARE MEMORY TO HOLD DATA
          >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
       if ( PreparePictureForImage(pic,width,height,3) == 0 ) { fprintf(stderr,"PreparePictureForImage could not prepare memory! \n"); return 0; }
       /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


 memcpy(pic->system.rgb_data,data,width*height*3*sizeof(char));
 pic->width=width;   // Width has been changed!
 pic->height=height; // Height has been changed!

 if (pic->system.overflow_guard!=OVERFLOW_GUARD_BYTES)
   {
     fprintf(stderr,"ERROR : Loading the picture caused an overflow!\n");
   }

  return 1;
 }

 fprintf(stderr,"ERROR : Could not load %s picture!\n",filename);

 return 0;
}








int loadFromCodecs(char * filename,struct Picture * pic)
{
 unsigned int imageType=guessFilenameTypeStupid(filename);
 struct Image * tmpImg = readImage(filename,imageType,0);

 if ( tmpImg!=0 )
 {
 unsigned int width = tmpImg->width;
 unsigned int height = tmpImg->height;
 pic->initial_width=width;
 pic->initial_height=height;

/*
 fprintf(stderr,"Resizing  %ux%u to ",width,height);
 unsigned int rescale_ratio=PickPictureRescaleRatio(width,height);
 width  = (unsigned int) (width  * (float) rescale_ratio / 100);
 height = (unsigned int) (height * (float) rescale_ratio / 100);
 fprintf(stderr," %ux%u .. ",width,height);
 resizeImage(tmpImg,width,height);
 fprintf(stderr,"done\n");
*/

 pic->is_jpeg = ( imageType == JPG_CODEC);
 if (pic->is_jpeg)
  {
    if (!GetJPEGExifOrientation(filename,pic))
     {
         fprintf(stderr,"Could not get Exif orientation for file %s \n",filename);
     }
  }

  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>
   PREPARE MEMORY TO HOLD DATA
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
   if ( PreparePictureForImage(pic,width,height,3) == 0 )
      {
        fprintf(stderr,"PreparePictureForImage could not prepare memory! \n");
        destroyImage(tmpImg);
        return 0;
      }
   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


 memcpy(pic->system.rgb_data,tmpImg->pixels,width*height*3*sizeof(char));
 pic->width=width;   // Width has been changed!
 pic->height=height; // Height has been changed!

 destroyImage(tmpImg);

 if (pic->system.overflow_guard!=OVERFLOW_GUARD_BYTES)
   {
     fprintf(stderr,"ERROR : Loading the picture caused an overflow!\n");
   }

  return 1;
 }

 fprintf(stderr,"ERROR : Could not load %s picture!\n",filename);

 return 0;
}






int LoadPicture(char * filename,struct Picture * pic)
{
  if (PrintPictureLoadingMsg()) fprintf(stderr,"Loading picture %s \n",filename);
  if ( pic == 0 ) { fprintf(stderr,"Error loading picture (%s) , accomodation structure is not allocated\n",filename); return 0; }



  //if ( loadFromCodecs(filename,pic) ) { pic->system.marked_for_rgbdata_loading=0; /* PICTURE IS LOADED ALL IS DONE :) */ }
  if ( WxLoadJPEG(filename,pic) )       { pic->system.marked_for_rgbdata_loading=0; /* PICTURE IS LOADED ALL IS DONE :) */ }
                                     else {
                                            pic->failed_to_load=1;
                                            pic->gpu.gl_rgb_texture=failed->gpu.gl_rgb_texture;
                                            fprintf(stderr,"Failed to open and load picture \n");
                                            return 0;
                                          }


  FixOpenGLPictureSize(pic);
  GetInterestingAreasList(pic);

  if ( pic->system.overflow_guard != OVERFLOW_GUARD_BYTES ) { fprintf(stderr,"Memory Overflow at Picture structure \n "); }

  if ( loading != 0 ) { pic->gpu.gl_rgb_texture=loading->gpu.gl_rgb_texture;
                        pic->gpu.marked_for_texture_loading=1;
                        /* Signal picture ready for texture creating*/
                      }
  frame.total_images_loaded++; /* ADDED HERE */

 return 1;
}


void EmptyPicture(struct Picture *new_picture,unsigned int empty_textures)
{

    new_picture->system.rgb_data=0;
    new_picture->system.rgb_data_size=0;
    new_picture->system.marked_for_rgbdata_loading=1;
    new_picture->system.marked_for_rgbdata_removal=0;

    new_picture->failed_to_load=0;


    if (empty_textures)
    {
     new_picture->gpu.marked_for_texture_loading=0;
     new_picture->gpu.marked_for_texture_removal=0;
     new_picture->gpu.gl_rgb_texture=0;
     new_picture->gpu.texture_data_size=0;

     new_picture->gpu.texture_loaded=0;
     new_picture->gpu.thumbnail_texture_loaded=0;
    }

    new_picture->is_jpeg=0;

    new_picture->directory_list_index=0;

    new_picture->height=0,new_picture->width=0,new_picture->depth=0;
    new_picture->initial_width=0,new_picture->initial_height=0;

    new_picture->mirror=0;

    new_picture->default_rotate=0;
    new_picture->rotate=0;
    new_picture->target_rotate=0;

    new_picture->transparency=1.0;
    new_picture->target_transparency=1.0;

    new_picture->position.ok=0;
    new_picture->position.size_x=6;  new_picture->position.size_y=4.5;
    new_picture->position.x=0.0;  new_picture->position.y=0.0; new_picture->position.z=500.0; // Posotion off screen to make it "invisible" until positioning :P
    new_picture->position.heading=0.0; new_picture->position.roll=0.0; new_picture->position.pitch=0.0;

    new_picture->time_viewed=0;
    new_picture->times_viewed=0;


    new_picture->system.overflow_guard=OVERFLOW_GUARD_BYTES;
}



struct Picture * CreatePicture(char * filename,unsigned int force_load)
{
  struct Picture * new_picture=0;

  new_picture = (struct Picture *) malloc( sizeof( struct Picture ) );
  if (  new_picture == 0 ) { fprintf(stderr,"Could not allocate memory for picture %s \n",filename); return 0; }

  EmptyPicture(new_picture,1);


  if (force_load==1)
   {
    if ( LoadPicture(filename,new_picture) )
      {
        //everything seems good
      } else
      {
        //Picture cannot be loaded this may have happened for a lot of reasons..
        //Not enough memory , bad file , removed file , whatever
        free(new_picture);
        return 0;
      }
   }
  return new_picture;
}


int UnLoadPicture(struct Picture * pic)
{
  if ( pic->system.rgb_data != 0 )
  {
   frame.system.usedRAM-=pic->system.rgb_data_size;
   frame.total_images_loaded--;

   pic->system.rgb_data_size=0;
   if ( pic->system.rgb_data != 0 ) { free(pic->system.rgb_data); pic->system.rgb_data=0; }
   pic->system.marked_for_rgbdata_loading=1;
  }
  EmptyPicture(pic,0);


  //frame.gpu.usedRAM-=pic->width*pic->height*4;
  //glDeleteTextures(1,&pic->gl_rgb_texture);


 return 1;
}

int PositionPicture(struct Picture * pic,unsigned int place)
{
  if (pic==0) {return 0;}

  return HandlePictureLayout(pic,place);
}


int DestroyPicture(struct Picture * pic)
{
  UnLoadPicture(pic);
  if ( pic != 0 ) free(pic);
  return 1;
}
