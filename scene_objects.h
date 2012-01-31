#ifndef SCENE_OBJECTS_H_INCLUDED
#define SCENE_OBJECTS_H_INCLUDED
#include "slideshow.h"

 struct SceneObject
{
   bool exists;
   bool moveable;
   bool drawable;
   bool nocull;
   bool noclip;
   unsigned int id;

   struct Point3D position;
   struct Point3D velocity;
   struct Point3D acceleration;
   struct Point3D decceleration;

   float width,height;

   struct Color color;
   float proximity_radius,max_velocity,max_acceleration; //0 = No limits
   float heading,pitch,roll;

   float transparency;
   unsigned int weight;
   unsigned int shape;

   unsigned int hardcoded_shape;
   unsigned int effect;

   unsigned int part_of_object;
   unsigned int created;

   unsigned int lifetime;
   unsigned int stage;

};

unsigned int Render_3DObjects();
unsigned int Add_3DObject(struct SceneObject newobj);
unsigned int Add_3DObject(float x,float y,float z,float width,float height,unsigned int stock_number,unsigned int lifetime);
void Run3DObjects(unsigned int microseconds);

#endif // SCENE_OBJECTS_H_INCLUDED
