#ifndef CAMERA_CONTROL_H_INCLUDED
#define CAMERA_CONTROL_H_INCLUDED

void SetDestinationCenter();
void SetDestinationOverPicture(unsigned int x,unsigned int y);
void SetDestinationOverPicture_HoverEffect(unsigned int x,unsigned int y,unsigned int position_start,unsigned int position_end);
void SetDestinationOverNextPicture();
void PerformCameraStep();

#endif // CAMERA_CONTROL_H_INCLUDED
