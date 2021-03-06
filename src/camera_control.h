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
void SetDestinationOverPicture(unsigned int place);
void SetDestinationOverPictureId(unsigned int id);
void SetDestinationOverPictureImmediate(unsigned int pic_place);
void SetDestinationOverNextPicture();


void CalculateActiveImage_AccordingToPosition(unsigned char force_check);

void CameraBounced();


int ChangeActiveImage(unsigned int place);
int ChangeActiveImage(unsigned int x,unsigned int y);

int CameraMoving();
int CameraOverPicture(unsigned int x,unsigned int y);
int CameraOverPicture(unsigned int pic_place);

int CameraSeesOnlyOnePicture();
int CameraSeesBackground();

void PerformCameraMovement(unsigned int microseconds_of_movement);

#endif // CAMERA_CONTROL_H_INCLUDED
