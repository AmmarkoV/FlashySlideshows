#include "image_sensing.h"
#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include "load_images.h"
#include "environment.h"



IplImage  *image=0;
char * opencv_pointer_retainer=0; // This is a kind of an ugly hack ( see lines noted with UGLY HACK ) to minimize memcpying between my VisCortex and OpenCV , without disturbing OpenCV

CvHaarClassifierCascade *cascade=0;
CvMemStorage            *storage=0;


int GetInterestingAreasList(struct Picture *picture)
{
  /* This function should call edge/face detection on the picture to find out intresting regions on the image
     They will be used for smart zoom in/out and improving the computer camera
  */
  if (PrintDevMsg()) fprintf(stderr," stub : GetInterestingAreasList not implemented yet\n");
  return 0;




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
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );

         //  SaveRegisterPartToFile(timestamped_filename,vid_reg, r->x , r->y , r->width , r->height );

    }


   // image->imageData = opencv_pointer_retainer; // UGLY HACK
    cvReleaseImage( &image );

  return 1;
}



void InitFaceRecognition(char * filename)
{

    /* load the classifier  */
    cascade = ( CvHaarClassifierCascade* ) cvLoad( filename, 0, 0, 0 );

    /* setup memory buffer; needed by the face detector */
    storage = cvCreateMemStorage( 0 );

}

void CloseFaceRecognition()
{
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );

}

