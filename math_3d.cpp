
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

#include "math_3d.h"
#include <math.h>

/*
  TAKEN FROM http://www.lighthouse3d.com/opengl/maths/index.php?raytriint

*/


#define innerProduct(v,q) \
       ((v)[0] * (q)[0] + \
		(v)[1] * (q)[1] + \
		(v)[2] * (q)[2])


#define crossProduct(a,b,c) \
        (a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
        (a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
        (a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];



/* a = b - c */
#define vector(a,b,c) \
        (a)[0] = (b)[0] - (c)[0];	\
        (a)[1] = (b)[1] - (c)[1];	\
        (a)[2] = (b)[2] - (c)[2];


int rayIntersectsTriangle(float *p, float *d,float *v0, float *v1, float *v2)
{

	float e1[3],e2[3],h[3],s[3],q[3];
	float a,f,u,v;

	vector(e1,v1,v0);
	vector(e2,v2,v0);
	crossProduct(h,d,e2);
	a = innerProduct(e1,h);

	if (a > -0.00001 && a < 0.00001)
		return(false);

	f = 1/a;
	vector(s,p,v0);
	u = f * (innerProduct(s,h));

	if (u < 0.0 || u > 1.0)
		return(false);

	crossProduct(q,s,e1);
	v = f * innerProduct(d,q);
	if (v < 0.0 || u + v > 1.0)
		return(false);
	// at this stage we can compute t to find out where
	// the intersection point is on the line
	float t = f * innerProduct(e2,q);
	if (t > 0.00001) // ray intersection
		return(true);
	else // this means that there is a line intersection
		 // but not a ray intersection
		 return (false);
}


int rayIntersectsRectangle(float *p, float *d,float *v0, float *v1, float *v2, float *v3)
{
   if (  rayIntersectsTriangle(p,d,v0,v1,v2) )
     {
       return 1;
     }

   if (  rayIntersectsTriangle(p,d,v1,v2,v3) )
     {
       return 1;
     }

   return 0;
}

float distanceBetween3DPoints(float *x1,float*y1,float *z1,float *x2,float*y2,float *z2)
{
  return (float) sqrt( (*x1 * *x2) + (*y1 * *y2) + (*z1* *z2) );
}
