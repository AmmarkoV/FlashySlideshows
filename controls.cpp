#include "slideshow.h"
#include "controls.h"

void inline MouseLook(int x,int y)
{

 //  angle_y -= (float) (x-prev_x)*mouse_freedom;
 //  angle_x -= (float) (y-prev_y)*mouse_freedom;
 //  prev_x = x;
 //  prev_y = y;

}

void Controls_Handle_MouseButtons(int button,int state, int x, int y)
{

}

void Controls_Handle_MouseMotion(int button,int state, int x, int y)
{

}


int Controls_Handle_Keyboard(unsigned char key, int x, int y)
{
    if (frame.vz<frame.distance_barrier_after_considered_zoom)  frame.desired_step=frame.desired_step_zoom; else
    if (frame.vz<frame.distance_barrier_after_considered_close)  frame.desired_step=frame.desired_step_close; else
    if (frame.vz<frame.distance_barrier_after_considered_far)  frame.desired_step=frame.desired_step_far;

    int nokey=0;
    switch (key)
    {
        case 1 : /* UP */ frame.angle_x-=0.5; break;
        case 2 : /* DOWN */ frame.angle_x+=0.5; break;
        case 3 : /* LEFT */ frame.angle_y-=0.5; break;
        case 4 : /* RIGHT */ frame.angle_y+=0.5;break;
        case 'o': frame.desired_x=0; frame.desired_y=0; frame.desired_z=0;
                  frame.angle_x=0; frame.angle_y=0; frame.angle_z=180; break;

        case 'r': frame.desired_z+=frame.desired_step; break;
        case 'f': frame.desired_z-=frame.desired_step; break;
        case 'a': frame.desired_x+=frame.desired_step; break;
        case 'd': frame.desired_x-=frame.desired_step; break;
        case 's': frame.desired_y+=frame.desired_step; break;
        case 'w': frame.desired_y-=frame.desired_step; break;
        case 'z': frame.angle_x-=0.5; break;
        case 'c': frame.angle_x+=0.5; break;
        case 't': frame.angle_y-=0.5; break;
        case 'g': frame.angle_y+=0.5; break;
        case 'y': frame.angle_z-=0.5; break;
        case 'h': frame.angle_z+=0.5; break;

       default : nokey=1;
            break;
    }

  return nokey;
}
