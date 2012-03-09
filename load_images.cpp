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
#include "memory_hypervisor.h"
#include "directory_listing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wx/image.h>

#include "load_textures.h"
#include "image_sensing.h"
#include "environment.h"

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
    case 1: chosen_width=320; break;
    case 2: chosen_width=640; break;
    case 3: chosen_width=800; break;
    case 4: chosen_width=1024; break;
    case 5: chosen_width=1152; break;
    case 6: chosen_width=1600; break;
    case 7: chosen_width=1920; break;

  };

  if ( frame.gpu.maximum_frame_dimension_size<chosen_width )
    {  //Make sure graphics card can take it..
        chosen_width=frame.gpu.maximum_frame_dimension_size-1;
    }

  return chosen_width;
}

unsigned int GetHeightQuality(unsigned int quality)
{
  return GetWidthQuality(quality)*3/4;
}


unsigned int PickPictureRescaleRatio(unsigned int start_width,unsigned int start_height)
{
  unsigned int best_height = GetHeightQuality(frame.quality_setting);
  unsigned int best_width = GetWidthQuality(frame.quality_setting);


  if ( ( start_width < best_width ) && (start_height < best_height) ) { return 100; }
  float ratio_width=start_width/best_width;
  float ratio_height=start_height/best_height;

  unsigned int scale_percentage=40;

  if (( ratio_width==0 ) | ( ratio_height==0 )) {  } else
  if ( ratio_width < ratio_height ) { scale_percentage=(float) 100/ratio_width; } else
                                    { scale_percentage=(float) 100/ratio_height; }
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
  return picturedata->marked_for_rgbdata_loading;
}

int PictureFailed(struct Picture * picturedata)
{
  if ( picturedata == 0 ) return 1;
  if ( picturedata == loading ) return 0;
  return picturedata->marked_for_rgbdata_loading;
}

int PrintPictureData(struct Picture * picturedata)
{
   if ( picturedata == 0 ) { fprintf(stderr,"Null picture structure \n"); return 0; }
   if (!PrintPictureLoadingMsg()) { return 0; }
   fprintf(stderr,"_____________________________________\n");
   fprintf(stderr,"Directory list index : %u \n",picturedata->directory_list_index);
   PrintDirectoryListItem(picturedata->directory_list_index);
   fprintf(stderr,"Image size : %ux%u:%u ratio %0.2f",picturedata->width,picturedata->height,picturedata->depth,(float)picturedata->width/picturedata->height);
   fprintf(stderr," OpenGL size : %0.2fx%0.2f ratio %0.2f",picturedata->position.size_x,picturedata->position.size_y,(float)picturedata->position.size_x/picturedata->position.size_y);

   fprintf(stderr,"\ntime_viewed : %u , times_viewed : %u , transparency : %0.2f \n",picturedata->time_viewed,picturedata->times_viewed,picturedata->transparency);

   fprintf(stderr," Status\n");
   fprintf(stderr,"Failed to Load : %u , Thumbnail texture loaded : %u , Texture loaded : %u \n",picturedata->failed_to_load,picturedata->thumbnail_texture_loaded,picturedata->texture_loaded);
   fprintf(stderr,"Mark tex load : %u , Mark tex rm : %u , Mark RGB load: %u Mark RGB rm : %u \n",picturedata->marked_for_texture_loading,picturedata->marked_for_texture_removal,picturedata->marked_for_rgbdata_loading,picturedata->marked_for_rgbdata_removal);
   fprintf(stderr,"_____________________________________\n");
 return 1;
}

int PreparePictureForImage(struct Picture * pic,unsigned int width,unsigned int height,unsigned int depth)
{
    if (PrintPictureLoadingMsg()) fprintf(stderr,"PreparePictureForImage , image %u x %u : %u ",width,height,depth);

    if ((pic->rgb_data!=0) || (pic->rgb_data_size!=0))
     {
         /* CLEAR MEMORY TO ALLOCATE IT LATER*/
         fprintf(stderr,"dirty Picture struct ");
         frame.system.usedRAM-=pic->rgb_data_size;

         if ( pic->rgb_data!=0 ) { free (pic->rgb_data);
                                   pic->rgb_data=0;
                                 }
         pic->rgb_data_size=0;
     }

    if ( ( width == 0 ) || ( height == 0 ) || ( depth == 0 ) ) {  fprintf(stderr,"PreparePictureForImage only cleared allocated memory\n"); return 1; }
    if ( ( width < 50 ) || ( height < 50 ) ) {  fprintf(stderr,"This picture is very small (%u,%u) , skipping it ,NEED A BETER IMPLEMENTATION TO PREVENT BLINKING\n",width,height);
                                                 pic->marked_for_rgbdata_loading=0;  /* THIS MAY CAUSE A BLINK NO USE IN RETRYING , IT HAS FAILED*/
                                                 return 0; }

    if ((pic->rgb_data==0) || (pic->rgb_data_size==0))
     {
         /* ALLOCATE ENOUGH MEMORY FOR THE RAW IMAGE */
         pic->overflow_guard=OVERFLOW_GUARD_BYTES;
         //if (frame.system.maxRAM < frame.system.usedRAM + width*height*depth ) { fprintf(stderr,"System memory bounds reached"); return 0; }

         frame.system.lastTexture=width*height*depth;
         if ( !RAM_Memory_can_accomodate(frame.system.lastTexture) ) { fprintf(stderr,"RAM Memory cannot accomodate %u bytes \n",(unsigned int) frame.system.lastTexture); return 0; }

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
     if (PrintPictureLoadingMsg()) fprintf(stderr,"ok\n");

    return 1;
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


 //Handle orientation here..!
 FILE *fpipe;
 char command[1024]={0};
 strcpy(command,"jpegexiforient \"");
 strcat(command,filename);
 strcat(command,"\"\0");


 fprintf(stderr,"Executing %s \n",command);

 char line[256];
 if ( !(fpipe = (FILE*)popen(command,"r")) )
   {
       fprintf(stderr,"Error extracting orientation for %s \n",filename);
   } else
   {

       /*
               1        2       3      4

              888888  888888      88  88
              88          88      88  88
              8888      8888    8888  8888
              88          88      88  88
              88          88  888888  888888


              5            6           7          8

              8888888888  88                  88  8888888888
              88  88      88  88          88  88      88  88
              88          8888888888  8888888888          88

     */

     while ( fgets( line, sizeof line, fpipe))
       {
          //fprintf(stderr,"Orientation for %s is `%s` \n",filename,line);
          if (line[0]=='1') { fprintf(stderr,"Normal Orientation\n"); } else
          if (line[0]=='2') { fprintf(stderr,"Inverted , but Normal Orientation\n");  pic->mirror=1;  }else
          if (line[0]=='3') { fprintf(stderr,"Rotated 180 degs but Normal Orientation\n"); pic->rotate=180; pic->target_rotate=180; }else
          if (line[0]=='4') { fprintf(stderr,"Rotated 180 degs but Inverted Orientation\n");  pic->mirror=1; pic->rotate=180; pic->target_rotate=180; }else
          if (line[0]=='5') { fprintf(stderr,"Rotated 270 degs but Inverted Orientation\n");  pic->mirror=1; pic->rotate=270; pic->target_rotate=270; }else
          if (line[0]=='6') { fprintf(stderr,"Rotated 270 degs but Normal Orientation\n"); pic->rotate=270; pic->target_rotate=270; }else
          if (line[0]=='7') { fprintf(stderr,"Rotated 90 degs but Inverted Orientation\n"); pic->mirror=1; pic->rotate=90; pic->target_rotate=90;  }else
          if (line[0]=='8') { fprintf(stderr,"Rotated 90 degs but Normal Orientation\n"); pic->rotate=90; pic->target_rotate=90; }
       }

       pic->default_rotate=pic->rotate;
   }
 pclose(fpipe);


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
  if (PrintPictureLoadingMsg()) fprintf(stderr,"Loading picture %s \n",filename);
  if ( pic == 0 ) { fprintf(stderr,"Error loading picture (%s) , accomodation structure is not allocated\n",filename); return 0; }



    if ( WxLoadJPEG(filename,pic) )       { pic->marked_for_rgbdata_loading=0; /* PICTURE IS LOADED ALL IS DONE :) */ }
                                     else {
                                            pic->failed_to_load=1;
                                            pic->gl_rgb_texture=failed->gl_rgb_texture;
                                            fprintf(stderr,"Failed to open and load picture \n");
                                            return 0;
                                          }


  FixOpenGLPictureSize(pic);
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

    new_picture->directory_list_index=0;

    new_picture->height=0,new_picture->width=0,new_picture->depth=0;

    new_picture->mirror=0;

    new_picture->default_rotate=0;
    new_picture->rotate=0;
    new_picture->target_rotate=0;

    new_picture->transparency=1.0;
    new_picture->target_transparency=1.0;

    new_picture->position.ok=0;
    new_picture->position.size_x=6;  new_picture->position.size_y=4.5;
    new_picture->position.x=0.0;  new_picture->position.y=0.0; new_picture->position.z=0.0;
    new_picture->position.heading=0.0; new_picture->position.yaw=0.0; new_picture->position.pitch=0.0;

  new_picture->time_viewed=0;
  new_picture->times_viewed=0;


  new_picture->overflow_guard=OVERFLOW_GUARD_BYTES;

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


  PictureIDtoXY(&x,&y,place);

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

  return 1;
}


int DestroyPicture(struct Picture * pic)
{
  UnLoadPicture(pic);
  if ( pic != 0 ) free(pic);
  return 1;
}
