#include "commandlineParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "environment.h"
#include "webinterface.h"
#include "wxwidgets_stuff.h"
#include "../slideshow.h"

#include "../layouts/layout_handler.h"

int parseArguments(int argc, char *argv[])
{

    if ( argc <1 )
     {
       fprintf(stderr,"Something weird is happening , argument zero should be executable path :S \n");
       return 1;
     } else
    if ( argc == 1 )
     {
       fprintf(stderr,"Usage flashyslideshow \"path\\to\\directory\\of\\album\"\n\n");
       strncpy((char * ) frame.program_data_directory,argv[0],MAX_PATH);
     } else
     {
        int i=0;
        for (i=0; i<argc; i++)
         {
             if (strcmp(argv[i],"-web")==0)
                   { //Recursive Directory command
                       fprintf(stderr,"Web Interface Enabled %u - %s\n",i,argv[i]);
                       if (!WebInterfaceCompiledIn())
                          {
                            //Web interface has been enabled by user BUT its not compiled in! :S
                            EmmitDialogWarning( (char *) "Please note that this build of FlashySlideshow hasn't got a compiled in WebInterface..!\n To download it run ./update_from_git.sh from the project's root dir..\n\n\nCheck here for more info :\nhttps://github.com/AmmarkoV/FlashySlideshows\n");
                          }
                       frame.enable_web_interface=1;
                   } else
             if (strcmp(argv[i],"-r")==0)
                   { //Recursive Directory command
                       fprintf(stderr,"Recursive Directory Enabled %u - %s\n",i,argv[i]);
                       frame.recursive=1;
                   } else
             if (strcmp(argv[i],"-fullscreen")==0)
                   { //Fullscreen window
                       fprintf(stderr,"Auto Fullscreen %u - %s\n",i,argv[i]);
                       frame.fullscreen=1;
                   } else
             if (strcmp(argv[i],"-play")==0)
                   { //Recursive Directory command
                       fprintf(stderr,"AutoStarting Playback %u - %s\n",i,argv[i]);
                       ToggleAutomaticSlideshow();
                   } else
             if (strcmp(argv[i],"-sfx")==0)
                   { //Sound Effects command
                       fprintf(stderr,"%u Sound Effects Enabled %s\n",i,argv[i]);
                       frame.enable_sound_effects=1;
                   } else
             if (strcmp(argv[i],"-file_move")==0)
                   { //Move Files command
                       fprintf(stderr,"%u Move Sorting Enabled with keys 0 to 9 %s\n",i,argv[i]);
                       frame.allow_operation_move=1;
                   } else
             if (strcmp(argv[i],"-file_resize")==0)
                   { //Resize Files command
                        if (i+1<=argc)
                        {
                         fprintf(stderr,"%u Move Resized Enabled at Resolution %s with keys 0 to 9 %s\n",i,argv[i+1],argv[i]);
                         frame.allow_operation_resize=1;
                         strcpy(frame.rescale_resolution_string,argv[i+1]);
                        }
                   } else
             if (strcmp(argv[i],"-file_copy")==0)
                   { //File Copy command
                       fprintf(stderr,"%u Move Sorting Enabled with keys 0 to 9 %s\n",i,argv[i]);
                       frame.allow_operation_copy=1;
                   } else
             if (strcmp(argv[i],"-file_link")==0)
                   { //File Copy command
                       fprintf(stderr,"%u Link Sorting Enabled with keys 0 to 9 %s\n",i,argv[i]);
                       frame.allow_operation_link=1;
                   } else
             if (strcmp(argv[i],"-no_hud")==0)
                   { //File Copy command
                       fprintf(stderr,"Hud Disabled..");
                       frame.disable_hud=1;
                   } else
             if (strcmp(argv[i],"-no_background")==0)
                   { //File Copy command
                       fprintf(stderr,"Background Disabled..");
                       frame.plain_background_no_image=1;
                   } else
             if (strcmp(argv[i],"-fd")==0)
                   { //Face Detection command
                       fprintf(stderr,"%u Face Detection Enabled %s\n",i,argv[i]);
                   } else
             if (strcmp(argv[i],"-v")==0)
                   { //Visuals Enabled command
                       fprintf(stderr,"%u Visuals Enabled %s\n",i,argv[i]);
                   } else
             if (strcmp(argv[i],"-m")==0)
                   { //Mipmaping Enabled command
                       fprintf(stderr,"%u Mipmaping Enabled %s\n",i,argv[i]);
                       frame.mipmaping=1;
                   } else
             if (strcmp(argv[i],"-q")==0)
                   { //Quality command
                      if (i+1<=argc)
                      {
                       frame.quality_setting=atoi(argv[i+1]); // Quality
                       fprintf(stderr,"%u Image Quality %s = %s ( %u )\n",i,argv[i],argv[i+1],frame.quality_setting);
                       if (frame.quality_setting>=3) { frame.try_for_best_render_quality=1; }
                       fprintf(stderr,"   Rendering Quality Switch %u\n",frame.try_for_best_render_quality);
                       //THIS HAS A SECOND PAYLOAD THAT HAPPENS AFTERWARDS----> to initialize frame.gpu.maximum_frame_total_size
                      }
                   } else
             if (strcmp(argv[i],"-images_per_line")==0)
                   { //Images per line command
                      if (i+1<=argc)
                      {
                        frame.images_per_line=atoi(argv[i+1]); // Background
                        fprintf(stderr,"%u Images per line Manually Chosen %s = %s ( %u )\n",i,argv[i],argv[i+1],frame.images_per_line);
                        frame.active_image_x=((unsigned int) frame.images_per_line/2);
                        frame.active_image_y=1;
                        frame.active_image_place=PictureXYtoID(frame.active_image_x,frame.active_image_y);

                        frame.last_image_x=frame.active_image_x;
                        frame.last_image_y=frame.active_image_y;
                        frame.last_image_place=frame.active_image_place;
                        GetLayoutCoordinatesForXY(frame.active_image_x,frame.active_image_y,&frame.vx,&frame.vy,&frame.vz);
                        frame.vz=8.0;
                        frame.desired_x=frame.vx; frame.desired_y=frame.vy; frame.desired_z=GetLayoutMaximumZ()-0.5;

                        fprintf(stderr,"Setting Center to %u,%u (%u) --> coords %0.2f,%0.2f,%0.2f\n",frame.active_image_x,frame.active_image_y,frame.active_image_place,frame.vx,frame.vy,frame.vz);

                      }
                   } else
             if (strcmp(argv[i],"-b")==0)
                   { //Quality command
                      if (i+1<=argc)
                      {
                        frame.background_number=atoi(argv[i+1]); // Background
                        fprintf(stderr,"%u Frame Background Manually Chosen %s = %s ( %u )\n",i,argv[i],argv[i+1],frame.background_number);
                      }
                   } else
             if (strcmp(argv[i],"-d")==0)
                   { //Transition delay command
                      if (i+1<=argc)
                      {
                       frame.time_ms_between_two_transitions=atoi(argv[i+1]);
                       fprintf(stderr,"%u Transition Delay %s = %s ( %u ) \n",i,argv[i],argv[i+1],frame.time_ms_between_two_transitions);
                      }
                   } else
             if (strcmp(argv[i],"-t")==0)
                   { //Transition Type command
                      if (i+1<=argc)
                      {
                       fprintf(stderr,"%u Transition Type %s=%s\n",i,argv[i],argv[i+1]);
                       if  ( *argv[i+1]=='0' ) { fprintf(stderr,"3D Seek\n");      frame.transitions.transition_mode=0; } else
                       if  ( *argv[i+1]=='1' ) { fprintf(stderr,"Immediate\n");    frame.transitions.transition_mode=1; } else
                       if  ( *argv[i+1]=='2' ) { fprintf(stderr,"Transparency\n"); frame.transitions.transition_mode=2; }  else
                                               { fprintf(stderr,"Incorrect Transition Type..\n"); }
                      }
                   } else
             if (strcmp(argv[i],"-l")==0)
                   { //Layout Type command
                      if (i+1<=argc)
                      {
                       fprintf(stderr,"%u Layout Type %s=%s\n",i,argv[i],argv[i+1]);
                       if  ( *argv[i+1]=='0' ) { fprintf(stderr,"Basic Layout\n");      frame.layout.layout_mode=0; } else
                       if  ( *argv[i+1]=='1' ) { fprintf(stderr,"Expo\n");    frame.layout.layout_mode=1; } else
                                               { fprintf(stderr,"Incorrect Layout Type..\n"); }
                      }
                   } else
             if (strcmp(argv[i],"-s")==0)
                   { //Sorting Type command
                      if (i+1<=argc)
                      {
                       fprintf(stderr,"%u Sorting Type %s=%s\n",i,argv[i],argv[i+1]);
                       if  ( *argv[i+1]=='0' ) { fprintf(stderr,"Ascending Name\n"); frame.sort_type=0; frame.sort_ascending=1; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='1' ) { fprintf(stderr,"Ascending Date\n"); frame.sort_type=1; frame.sort_ascending=1; frame.sort_randomization=0;  } else
                       if  ( *argv[i+1]=='2' ) { fprintf(stderr,"Ascending Size\n"); frame.sort_type=2; frame.sort_ascending=1; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='3' ) { fprintf(stderr,"Descending Name\n"); frame.sort_type=0; frame.sort_ascending=0; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='4' ) { fprintf(stderr,"Descending Date\n"); frame.sort_type=1; frame.sort_ascending=0; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='5' ) { fprintf(stderr,"Descending Size\n"); frame.sort_type=2; frame.sort_ascending=0; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='r' ) { fprintf(stderr,"Randomized\n"); frame.sort_randomization=1; }  else
                                               { fprintf(stderr,"Incorrect Sorting Type..\n"); }
                      }
                   }

         }
       strncpy((char * ) frame.program_data_directory,argv[0],MAX_PATH);
       strncpy((char * ) frame.album_directory,argv[argc-1],MAX_PATH);

       if ( strncmp((char*)frame.album_directory,"pick",4)==0 )
         {
           /* PICK ALBUM DIR VIA GUI */
           if ( LaunchGUI_PickDir() == 0 )  { fprintf(stderr,"No dir selected! \n"); return 0; }
         }
     }
    strcat((char * ) frame.album_directory,"/");
    fprintf(stderr,"Slideshow directory is = %s (%u chars) \n", frame.album_directory ,(unsigned int) strlen((char*) frame.album_directory));


    return 1;
}
