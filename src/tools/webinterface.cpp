#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../slideshow.h"
#include "webinterface.h"
#include "../controls.h"
#include "../AmmarServer/src/AmmServerlib/AmmServerlib.h"

#define MAX_WEB_COMMAND_SIZE 512

struct AmmServer_Instance  * flashy_server=0;
struct AmmServer_RH_Context index_control={0};

char * index_content =0 ;
unsigned int index_content_size = 0;

int WebInterfaceCompiledIn()
{
  if ( AmmServer_Version() != 0 ) { return 1; }
  return 0;
}


void * index_control_page(struct AmmServer_DynamicRequest  * rqst)
{
  char command[MAX_WEB_COMMAND_SIZE]={0};

  //If we have the console argument set this means we dont want the html output enabled so we switch it off
  if ( _GET(flashy_server,rqst,(char*) "UP",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(1,0,0); }
  if ( _GET(flashy_server,rqst,(char*) "DOWN",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(2,0,0); }
  if ( _GET(flashy_server,rqst,(char*) "LEFT",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(3,0,0); }
  if ( _GET(flashy_server,rqst,(char*) "RIGHT",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(4,0,0); }
  if ( _GET(flashy_server,rqst,(char*) "PLAY",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(13,0,0); }

  rqst->contentSize=0;
  if (index_content!=0)
    {
     strncpy(rqst->content,index_content,index_content_size);
     rqst->contentSize=index_content_size ;
    }

  return 0;
}


//This function adds a Resource Handler for the pages stats.html and formtest.html and associates stats , form and their callback functions
void init_dynamic_content(char * webroot,char * app_clipart)
{
  char index_file_template[MAX_FILE_PATH];
  strcpy(index_file_template,app_clipart);
  strcat(index_file_template,"/remote_control.html");

  index_content = AmmServer_ReadFileToMemory(index_file_template,&index_content_size);
  if (index_content==0) { fprintf(stderr,"Could not find index page template..\n"); }

  if (! AmmServer_AddResourceHandler(flashy_server,&index_control, (char *) "/index.html",webroot,4096,0,(void* ) &index_control_page,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding index page\n"); }
  AmmServer_DoNOTCacheResourceHandler(flashy_server,&index_control);
}

//This function destroys all Resource Handlers and free's all allocated memory..!
void close_dynamic_content()
{
    AmmServer_RemoveResourceHandler(flashy_server,&index_control,1);
}






int StartWebInterface(char * IP , unsigned int port , char * fileroot,char * app_clipart)
{
    flashy_server = AmmServer_Start
        (
           IP,
           port,
           0, /*This means we don't want a specific configuration file*/
           fileroot,
           fileroot
         );
     if (!flashy_server) { return 0; }

    init_dynamic_content(fileroot,app_clipart);

  return 1;
}

int StopWebInterface()
{

    close_dynamic_content();

    AmmServer_Stop(flashy_server);

  return 0;
}


