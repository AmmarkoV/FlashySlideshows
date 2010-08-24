#include "load_images.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "load_textures.h"

#define PPMREADBUFLEN 256

struct Picture *loading=0;
int OpenGL_is_rendering=0;
int OpenGL_is_making_textures=0;


int PreparePictureForImage(struct Picture * pic,unsigned int width,unsigned int height,unsigned int depth)
{
    fprintf(stderr,"PreparePictureForImage Called for an image %u x %u : %u \n",width,height,depth);

    if ((pic->rgb_data!=0) || (pic->rgb_data_size!=0))
     {
         /* CLEAR MEMORY TO ALLOCATE IT LATER*/
         fprintf(stderr,"Picture Structure is dirty\n");
         free (pic->rgb_data);
         pic->rgb_data_size=0;
     }

    if ( ( width == 0 ) || ( height == 0 ) || ( depth == 0 ) ) {  fprintf(stderr,"PreparePictureForImage only cleared allocated memory\n");
                                                                  return 1; }

    if ((pic->rgb_data==0) || (pic->rgb_data_size==0))
     {
         /* ALLOCATE ENOUGH MEMORY FOR THE RAW IMAGE */
         pic->overflow_guard=OVERFLOW_GUARD_BYTES;
         pic->rgb_data=(char *) malloc( ( width*height*depth ) + depth );

         if  ( pic->rgb_data <=0 )
          {
            fprintf(stderr,"Error allocating memory for raw image \n");
            return 0;
          }
     }

     pic->width=width;
     pic->height=height;
     pic->depth=depth;
     pic->rgb_data_size=width*height*depth;
     fprintf(stderr,"PreparePictureForImage completed\n");

    return 1;
}


int ReadPPM(char * filename,struct Picture * pic)
{
    FILE *pf=0;
    pf = fopen(filename,"rb");

    if (pf!=0 )
    {
        char buf[PPMREADBUFLEN], *t;
        unsigned int w=0, h=0, d=0;
        int r=0;

        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( (t == 0) || ( strncmp(buf, "P6\n", 3) != 0 ) ) { fclose(pf); return 0; }
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == 0 ) { fclose(pf); return 0; }
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) { fclose(pf); return 0; }
        // The program fails if the first byte of the image is equal to 32. because
        // the fscanf eats the space and the image is read with some bit less
        r = fscanf(pf, "%u\n", &d);
        if ( (r < 1) || ( d != 255 ) ) { fclose(pf); return 0; }

       PreparePictureForImage(pic,w,h,3);

        if ( (w!=pic->width) || (h!=pic->height) )
           {
             fprintf(stderr,"Incorrect file size ( %s ) :P\n",filename);
             if ( w * h > pic->width * pic->height )
               {
                 fprintf(stderr,"File %s will lead to overflow stopping read..\n",filename);
                 fclose(pf);
                 return 0;
               }
           }

        if ( pic->rgb_data != 0 )
        {
            size_t rd = fread(pic->rgb_data,3, w*h, pf);
            fclose(pf);
            if ( rd < w*h )
            {
               return 0;
            }
            return 1;
        }
        fclose(pf);
    }
  return 0;
}


int LoadPicture(char * filename,struct Picture * pic)
{
  if ( pic == 0 ) { fprintf(stderr,"Error loading picture (%s) , accomodation structure is not allocated\n",filename); return 0; }

  char command[512]={0};
  sprintf(pic->ppm_filename,"%s.ppm",filename);

  /* COMMAND LINE CONVERSION OF FILE TO PPM */
  sprintf(command,"convert %s %s",filename,pic->ppm_filename);
  int i=system((const char *)command);
  if ( i != 0 ) fprintf(stderr,"Error (%d) converting image\n",i);
  ReadPPM(pic->ppm_filename,pic);
  /*----------------------------------------*/


  /* REMOVE TEMPORARY PPM FILE */
  sprintf(command,"rm %s",pic->ppm_filename);
  i=system((const char *)command);
  if ( i != 0 ) fprintf(stderr,"Error (%d) removing used image\n",i);
  /*----------------------------------------*/


  if ( pic->overflow_guard != OVERFLOW_GUARD_BYTES ) { fprintf(stderr,"Memory Overflow at Picture structure \n "); }

  if ( loading != 0 ) { pic->gl_rgb_texture=loading->gl_rgb_texture;
                        pic->ready_for_texture=1;
                      } else
                      {  make_texture(pic); } // <- STARTING TEXTURE WILL BE CALLED ON MAIN THREAD

  /* MAKE TEXTURE */
  //  make_texture(pic);
  /*----------------------------------------*/

 return 1;
}



struct Picture * CreatePicture(char * filename)
{
  struct Picture * new_picture=0;

  new_picture = (struct Picture *) malloc( sizeof( struct Picture ) );
  if (  new_picture == 0 ) { fprintf(stderr,"Could not allocate memory for picture %s \n",filename); return 0; }

    new_picture->rgb_data=0;
    new_picture->rgb_data_size=0;

    new_picture->ready_for_texture=0;
    new_picture->gl_rgb_texture=0;

    new_picture->height=0,new_picture->width=0,new_picture->depth=0;

    /* new_picture->position.x=0.0;  new_picture->position.y=0.0; new_picture->position.z=0.0; */
    /* new_picture->position.heading=0.0; new_picture->position.yaw=0.0; new_picture->position.pitch=0.0;*/

  new_picture->time_viewed=0;
  new_picture->times_viewed=0;

  new_picture->overflow_guard=OVERFLOW_GUARD_BYTES;

  LoadPicture(filename,new_picture);

  return new_picture;
}


int UnLoadPicture(struct Picture * pic)
{
  fprintf(stderr,"Unloading Picture\n");

  pic->rgb_data_size=0;
  if ( pic->rgb_data != 0 ) free(pic->rgb_data);

  glDeleteTextures(1,&pic->gl_rgb_texture);

  if ( pic != 0 ) free(pic);
 return 1;
}
