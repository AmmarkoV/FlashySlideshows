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

#include "effects.h"
#include "../load_images.h"
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif


int wiggle_count = 0;
float points[45][45][3];   // the array for the points on the grid of our "wave"


void RenderPictureWiggling(struct Picture * pic,unsigned int place,float x,float y,float z,float heading,float pitch,float roll)
{  //based on http://nehe.gamedev.net/tutorial/flag_effect_%28waving_texture%29/16002/
    glBindTexture(GL_TEXTURE_2D, pic->gl_rgb_texture );
    glPolygonMode(GL_BACK, GL_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);

    glBegin(GL_QUADS);
    unsigned int ux,uy;
    float float_x,float_y,float_xb,float_yb;
    for (ux=0; ux<44; ux++)
    {
	  for (uy=0; uy<44; uy++)
       {
	    float_x  = (float) (ux)/44;
	    float_y  = (float) (uy)/44;
	    float_xb = (float) (ux+1)/44;
	    float_yb = (float) (uy+1)/44;

	    glTexCoord2f( float_x, float_y);
	                                 //-pic->position.size_x -*pic->position.size_y
	    glVertex3f(x+pic->position.x+ points[ux][uy][0]  ,
                   y+pic->position.y+ points[ux][uy][1]  ,
                   z+pic->position.z+points[ux][uy][2] );

	    glTexCoord2f( float_x, float_yb );
	    glVertex3f(x+pic->position.x+ points[ux][uy+1][0] ,
                   y+pic->position.y+ points[ux][uy+1][1],
                   z+pic->position.z+points[ux][uy+1][2] );

	    glTexCoord2f( float_xb, float_yb );
	    glVertex3f(x+pic->position.x+ points[ux+1][uy+1][0],
                   y+pic->position.y+ points[ux+1][uy+1][1],
                   z+pic->position.z+points[ux+1][uy+1][2] );

	    glTexCoord2f( float_xb, float_y );
	    glVertex3f(x+pic->position.x+ points[ux+1][uy][0],
                   y+pic->position.y+ points[ux+1][uy][1],
                   z+pic->position.z+points[ux+1][uy][2] );
	   }
    }
    glEnd();
}


void PerformWiggling()
{
    if (wiggle_count == 100)
      { // cycle the sine values
        unsigned int ux,uy;
	    for (uy = 0; uy <45; uy++) { points[44][uy][2] = points[0][uy][2]; }

	    for( ux = 0; ux < 44; ux++ )
	     {
	      for( uy = 0; uy < 45; uy++)
	      {
		   points[ux][uy][2] = points[ux+1][uy][2];
	       }
	     }
	    wiggle_count = 0;
      }
    wiggle_count++;
}


void InitEffects()
{
  float float_x , float_y;
  for(float_x = 0.0f; float_x < 9.0f; float_x +=  0.2f )
  {
	for(float_y = 0.0f; float_y < 9.0f; float_y += 0.2f)
	{
	    points[ (int) (float_x*5) ][ (int) (float_y*5) ][0] = float_x - 4.4f;
	    points[ (int) (float_x*5) ][ (int) (float_y*5) ][1] = float_y - 4.4f;
	    points[ (int) (float_x*5) ][ (int) (float_y*5) ][2] = (float) (sin( ( (float_x*5*8)/360 ) * 3.14159 * 2 ));
	}
  }
}
