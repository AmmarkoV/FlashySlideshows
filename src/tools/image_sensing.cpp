/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "image_sensing.h"
#include <stdlib.h>
#include <stdio.h>
#include "../hypervisor/load_images.h"
#include "environment.h"


#define USE_FACEDETECTION 0


#if USE_FACEDETECTION
#include <cv.h>
#include <cxcore.h>

IplImage  *image=0;
char * opencv_pointer_retainer=0; // This is a kind of an ugly hack ( see lines noted with UGLY HACK ) to minimize memcpying between my VisCortex and OpenCV , without disturbing OpenCV

CvHaarClassifierCascade *cascade=0;
CvMemStorage            *storage=0;
#endif



int GetInterestingAreasList(struct Picture *picture)
{
  /* This function should call edge/face detection on the picture to find out intresting regions on the image
     They will be used for smart zoom in/out and improving the computer camera
  */
  if (PrintDevMsg()) fprintf(stderr," stub : GetInterestingAreasList not implemented yet\n");
  return 0;


#if USE_FACEDETECTION
    image = cvCreateImage( cvSize(picture->width,picture->height), IPL_DEPTH_8U, 3 );
    opencv_pointer_retainer = image->imageData; // UGLY HACK

    /* detect faces */
   // image->imageData=(char*) video_register[vid_reg].pixels; // UGLY HACK


    CvSeq *faces = cvHaarDetectObjects
           (
            image,
            cascade,
            storage,
            1.1,
            3,
            0 /*CV_HAAR_DO_CANNY_PRUNNING*/
            , cvSize( 40, 40 )
            //, cvSize( 50, 50 ) // <--- This might have to be commented out on older OpenCV versions where there is only a minimum Window!
            );

    int i;
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {
       // CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );

         //  SaveRegisterPartToFile(timestamped_filename,vid_reg, r->x , r->y , r->width , r->height );

    }


   // image->imageData = opencv_pointer_retainer; // UGLY HACK
    cvReleaseImage( &image );

  return 1;
  #endif
}



void InitFaceRecognition(char * filename)
{
#if USE_FACEDETECTION
    /* load the classifier  */
    cascade = ( CvHaarClassifierCascade* ) cvLoad( filename, 0, 0, 0 );

    /* setup memory buffer; needed by the face detector */
    storage = cvCreateMemStorage( 0 );
#endif
}

void CloseFaceRecognition()
{
#if USE_FACEDETECTION
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );
#endif
}

