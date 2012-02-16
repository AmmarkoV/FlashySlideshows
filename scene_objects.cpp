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

#include "scene_objects.h"
#include "visuals.h"
#include <stdlib.h>
#include <stdio.h>


 struct SceneObject objects[MAX_SCENE_OBJECTS ]={0};
 unsigned int existing_objects=0;


unsigned int Render_3DObject(unsigned int objnum)
{
  DrawDecal(
             objects[objnum].position.x,
             objects[objnum].position.y,
             objects[objnum].position.z,
             objects[objnum].roll,
             objects[objnum].width,
             objects[objnum].height,
             objects[objnum].hardcoded_shape
           );
  return 1;
}

unsigned int Render_3DObjects()
{
  unsigned int object_traverser=0;
  while ( object_traverser < existing_objects )
   {
     Render_3DObject(object_traverser);
     ++object_traverser;
   }
  return 1;
}

unsigned int Full_of_3DObjects()
{
    if (MAX_SCENE_OBJECTS <=existing_objects) { return 1; }
    return 0;
}

unsigned int Object_Out_Of_Mem(unsigned int objnum)
{
    if (MAX_SCENE_OBJECTS <= objnum) { return 1; } else
    if (existing_objects<= objnum) { return 1; }
    return 0;
}


unsigned int Clear_3DObject(unsigned int objnum)
{
    if (MAX_SCENE_OBJECTS <= objnum) { return 1; }

    struct SceneObject clear_obj = {0};
    objects[objnum]=clear_obj;

    return 0;
}


unsigned int Add_3DObject(struct SceneObject newobj)
{
  if (!Full_of_3DObjects())
   {
     objects[existing_objects]=newobj;
     ++existing_objects;
     fprintf(stderr,"Object %u Created \n",existing_objects);
     return existing_objects-1;
   }
  return 0; /*Return object number*/
}

unsigned int Add_3DObject(float x,float y,float z,float width,float height,unsigned int stock_number,unsigned int lifetime)
{
  if (!Full_of_3DObjects())
   {
     Clear_3DObject(existing_objects);
     objects[existing_objects].position.x=x;
     objects[existing_objects].position.y=y;
     objects[existing_objects].position.z=z;
     objects[existing_objects].width=width;
     objects[existing_objects].height=height;
     objects[existing_objects].lifetime=lifetime;
     objects[existing_objects].hardcoded_shape=stock_number;

     fprintf(stderr,"Object %u Created @ %0.2f %0.2f %0.2f \n",existing_objects,x,y,z);
     ++existing_objects;
     return existing_objects-1;
   }
  return 0; /*Return object number*/
}


unsigned int Delete_3DObject(unsigned int objnum)
{
   if (existing_objects==0) { return 0; }
   if (Object_Out_Of_Mem(objnum)) { return 0; }
   if (objnum==existing_objects-1)
    {
       --existing_objects;
    } else
    {
       objects[objnum]=objects[existing_objects-1];
       --existing_objects;
    }

  return 1;
}




void Run3DObjects(unsigned int microseconds)
{
  unsigned int object_traverser=0;
  while ( object_traverser < existing_objects )
   {

     objects[object_traverser].position.x+=objects[object_traverser].velocity.x;
     objects[object_traverser].position.y+=objects[object_traverser].velocity.y;
     objects[object_traverser].position.z+=objects[object_traverser].velocity.z;

     // NOT SURE IF AXIS ARE CORRECT :P
     objects[object_traverser].heading+=objects[object_traverser].rotation_velocity.y;
     objects[object_traverser].roll+=objects[object_traverser].rotation_velocity.z;
     objects[object_traverser].pitch+=objects[object_traverser].rotation_velocity.x;


     if (objects[object_traverser].lifetime!=0)
      {
          if (microseconds>=objects[object_traverser].lifetime)
            { //Object Lifetime Expired..!
              objects[object_traverser].lifetime=0;
              Delete_3DObject(object_traverser);
              fprintf(stderr,"Object %u Timed out\n",object_traverser);
            } else
            {
              objects[object_traverser].lifetime-=microseconds;
            }
      }




     ++object_traverser;
   }
}
