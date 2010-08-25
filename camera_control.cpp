#include "slideshow.h"
#include "camera_control.h"

void PerformCameraStep()
{

    /*
      -------------------------------------
      CAMERA SMOOTH ZOOM/PAN ETC
      -------------------------------------
    */
    float speed_multiplier=frame.fps/5; // 250;
    if ( speed_multiplier == 0 ) speed_multiplier=250;


    if ( frame.desired_x != frame.vx ) { if ( frame.desired_x < frame.vx )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vx = frame.vx- ( frame.vx - frame.desired_x ) / speed_multiplier;
                                   if ( frame.desired_x > frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_x > frame.vx )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vx = frame.vx+ ( frame.desired_x - frame.vx ) / speed_multiplier;
                                   if ( frame.desired_x < frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }

    if ( frame.desired_y != frame.vy ) { if ( frame.desired_y < frame.vy )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vy = frame.vy- ( frame.vy - frame.desired_y ) / speed_multiplier;
                                   if ( frame.desired_y > frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_y > frame.vy )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vy = frame.vy+ ( frame.desired_y - frame.vy ) / speed_multiplier;
                                   if ( frame.desired_y < frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }


    if ( frame.desired_z != frame.vz ) { if ( frame.desired_z < frame.vz )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vz = frame.vz- ( frame.vz - frame.desired_z ) / speed_multiplier;
                                   if ( frame.desired_z > frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_z > frame.vz )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vz = frame.vz+ ( frame.desired_z - frame.vz ) / speed_multiplier;
                                   if ( frame.desired_z < frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }
  /* -------------------------------------
     CAMERA SAFE GUARD!
     -------------------------------------
  */
   if ( frame.vz<=frame.distance_block_lower) { frame.vz=frame.distance_block_lower; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */
   if ( frame.vz>=frame.distance_block_upper) { frame.vz=frame.distance_block_upper; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */

}
