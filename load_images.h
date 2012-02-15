/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammarkov.ath.cx
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

#ifndef LOAD_IMAGES_H_INCLUDED
#define LOAD_IMAGES_H_INCLUDED

#include <GL/glu.h>
#include "slideshow.h"

#define OVERFLOW_GUARD_BYTES 10101001

struct Position3D
{
  char ok;
  float x,y,z;
  float heading,yaw,pitch;
  float size_x,size_y;
};

struct Picture
{


  unsigned int height;
  unsigned int width;
  unsigned int depth;
  float transparency; /* 0.0 -> 1.0 */
  float target_transparency; /* 0.0 -> 1.0 */

  unsigned int time_viewed;
  unsigned int times_viewed;

  unsigned int failed_to_load;
  unsigned char thumbnail_texture_loaded;
  unsigned char texture_loaded;

 // unsigned int ready_for_texture; /* Will be replaced with marked_for_texture_loading*/

  unsigned char marked_for_texture_loading;
  unsigned char marked_for_texture_removal;
  unsigned char marked_for_rgbdata_loading;
  unsigned char marked_for_rgbdata_removal;

  GLuint gl_rgb_texture;
  GLuint gl_rgb_thumbnail_texture;

  struct Position3D position;

  //char filename[MAX_PATH]; This can be acquired through the directory_list
  unsigned int directory_list_index;

  unsigned long rgb_data_size;
  char * rgb_data;
  unsigned long overflow_guard;

};

extern struct Picture *star;
extern struct Picture *heart;
extern struct Picture *play_img;
extern struct Picture *pause_img;
extern struct Picture *loading;
extern struct Picture *loading_texture;
extern struct Picture *failed;
extern struct Picture *background;
extern struct Picture *picture_frame;

extern int OpenGL_is_rendering;
extern int OpenGL_is_making_textures;

unsigned int GetWidthQuality(unsigned int quality);
unsigned int GetHeightQuality(unsigned int quality);

int PictureCreationPending(struct Picture * picturedata);
int PictureLoadingPending(struct Picture * picturedata);
int PictureFailed(struct Picture * picturedata);
int PrintPictureData(struct Picture * picturedata);

struct Picture * CreatePicture(char * filename,unsigned int force_load);
int LoadPicture(char * filename,struct Picture * pic);
int UnLoadPicture(struct Picture * pic);
int DestroyPicture(struct Picture * pic);

int PositionPicture(struct Picture * pic,unsigned int place);
#endif // LOAD_IMAGES_H_INCLUDED
