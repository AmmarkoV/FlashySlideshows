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


void SetDestinationCenter();
void ResetCameraOrientation();
void MoveDestinationCenterRaw(float x,float y,float z);
int  MoveToPicture(unsigned int direction);
void MoveDestinationCenter(unsigned int movement_direction);
void SetDestinationOverPicture(unsigned int x,unsigned int y);
void SetDestinationOverPictureId(unsigned int id);
void SetDestinationOverPictureImmediate(unsigned int x,unsigned int y);
void SetDestinationOverPicture_HoverEffect(unsigned int x,unsigned int y,unsigned int position_start,unsigned int position_end);
void SetDestinationOverNextPicture();

void PickHoverEffect(unsigned int x,unsigned int y);

void CheckForTargetInBounds();


int ChangeActiveImage(unsigned int place);
int ChangeActiveImage(unsigned int x,unsigned int y);

int CameraMoving();
int CameraOverPicture(unsigned int x,unsigned int y);
int CameraOverPicture(unsigned int pic_place);

int CameraSeesOnlyOnePicture();
int CameraSeesBackground();

void PerformCameraMovement(unsigned int microseconds_of_movement);

#endif // CAMERA_CONTROL_H_INCLUDED
