#ifndef CAMERA_CONTROL_H_INCLUDED
#define CAMERA_CONTROL_H_INCLUDED

enum Directions
{
   D_NONE=0,
   D_RIGHT,
   D_LEFT,
   D_UP,
   D_DOWN,
   D_IN,
   D_OUT
};

void ToggleTransitionMode();

void SetDestinationCenter();
void MoveDestinationCenter(unsigned int movement_direction);
void SetDestinationOverPicture(unsigned int x,unsigned int y);
void SetDestinationOverPictureImmediate(unsigned int x,unsigned int y);
void SetDestinationOverPicture_HoverEffect(unsigned int x,unsigned int y,unsigned int position_start,unsigned int position_end);
void SetDestinationOverNextPicture();

void PickHoverEffect(unsigned int x,unsigned int y);
void PerformCameraMovement(unsigned int microseconds_of_movement);

#endif // CAMERA_CONTROL_H_INCLUDED
