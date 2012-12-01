#include <stdio.h>
#include <string.h>
#include "webinterface.h"
#include "AmmarServer/src/AmmServerlib/AmmServerlib.h"


struct AmmServer_Instance  * flashy_server=0;
struct AmmServer_RH_Context index_control={0};
struct AmmServer_RH_Context commands={0};


void * index_control_page(char * content)
{
  //No range check but since everything here is static max_stats_size should be big enough not to segfault with the strcat calls!
  sprintf(content,"<html><head><title>FlashySlideshows Remote Control</title></head><body>");
  strcat(content,"PREV , NEXT</body></html>");
  index_control.content_size=strlen(content);
  return 0;
}


//This function adds a Resource Handler for the pages stats.html and formtest.html and associates stats , form and their callback functions
void init_dynamic_content(char * webroot)
{
  if (! AmmServer_AddResourceHandler(flashy_server,&index_control,"/index.html",webroot,4096,0,(void* ) &index_control_page,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding index page\n"); }
  AmmServer_DoNOTCacheResourceHandler(flashy_server,&index_control);

  if (! AmmServer_AddResourceHandler(flashy_server,&commands,"/command.html",webroot,4096,0,(void* ) &index_control_page,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding command page\n"); }
  AmmServer_DoNOTCacheResourceHandler(flashy_server,&commands);


}

//This function destroys all Resource Handlers and free's all allocated memory..!
void close_dynamic_content()
{
    AmmServer_RemoveResourceHandler(flashy_server,&index_control,1);
    AmmServer_RemoveResourceHandler(flashy_server,&commands,1);
}
















int StartWebInterface(char * IP , unsigned int port , char * fileroot)
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

    init_dynamic_content(fileroot);

  return 1;
}

int StopWebInterface()
{

    close_dynamic_content();

    AmmServer_Stop(flashy_server);

  return 0;
}


