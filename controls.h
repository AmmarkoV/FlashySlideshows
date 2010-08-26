#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

float RefreshDesiredStep_AccordingToPosition();
void Controls_Handle_MouseMotion(int button,int state, int x, int y);
void Controls_Handle_MouseButtons(int button,int state, int x, int y);
int Controls_Handle_Keyboard(unsigned char key, int x, int y);

#endif // CONTROLS_H_INCLUDED
