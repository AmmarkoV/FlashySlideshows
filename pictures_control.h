#ifndef PICTURES_CONTROL_H_INCLUDED
#define PICTURES_CONTROL_H_INCLUDED

void PerformPicturesMovement(unsigned int microseconds_of_movement);

int FadeInPicture();
int FadeOutPicture();

int RotatePictureRight();
int RotatePictureLeft();
int FreeRotatePicture(unsigned int max_rotation_either_way);
#endif // PICTURES_CONTROL_H_INCLUDED
