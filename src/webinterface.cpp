#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webinterface.h"
#include "controls.h"
#include "AmmarServer/src/AmmServerlib/AmmServerlib.h"

#define MAX_WEB_COMMAND_SIZE 512

struct AmmServer_Instance  * flashy_server=0;
struct AmmServer_RH_Context index_control={0};


void * index_control_page(char * content)
{

  char command[MAX_WEB_COMMAND_SIZE]={0};

  //If we have the console argument set this means we dont want the html output enabled so we switch it off
  if ( _GET(flashy_server,&index_control,"UP",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(1,0,0); }
  if ( _GET(flashy_server,&index_control,"DOWN",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(2,0,0); }
  if ( _GET(flashy_server,&index_control,"LEFT",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(3,0,0); }
  if ( _GET(flashy_server,&index_control,"RIGHT",command,MAX_WEB_COMMAND_SIZE) ) { Controls_Handle_Keyboard(4,0,0); }


  //No range check but since everything here is static max_stats_size should be big enough not to segfault with the strcat calls!
  sprintf(content,"<html><head><title>FlashySlideshows Remote Control</title></head><body>");
  strcat(content,"<form name=\"input\" action=\"index.html\" method=\"get\"> <input type=\"hidden\" value=\"UP\"> <input type=\"submit\" value=\"Up\"> </form> <br>");
  strcat(content,"<form name=\"input\" action=\"index.html\" method=\"get\"> <input type=\"hidden\" value=\"DOWN\"> <input type=\"submit\" value=\"Down\"> </form> <br>");
  strcat(content,"<form name=\"input\" action=\"index.html\" method=\"get\"> <input type=\"hidden\" value=\"LEFT\"> <input type=\"submit\" value=\"Left\"> </form> <br>");
  strcat(content,"<form name=\"input\" action=\"index.html\" method=\"get\"> <input type=\"hidden\" value=\"RIGHT\"> <input type=\"submit\" value=\"Right\"> </form> <br><br>");


  sprintf(command,"<a href=\"index.html?%s=1&tick=%u\">%s</a><br>\n",(char*) "UP"   , rand()%100000 , (char*) "Go Up");     strcat(content,command);
  sprintf(command,"<a href=\"index.html?%s=1&tick=%u\">%s</a><br>\n",(char*) "LEFT" , rand()%100000 , (char*) "Go Left");   strcat(content,command);
  sprintf(command,"<a href=\"index.html?%s=1&tick=%u\">%s</a><br>\n",(char*) "RIGHT", rand()%100000 , (char*) "Go Right");  strcat(content,command);
  sprintf(command,"<a href=\"index.html?%s=1&tick=%u\">%s</a><br>\n",(char*) "DOWN" , rand()%100000 , (char*) "Go Down");   strcat(content,command);


  strcat(content,"</body></html>");
  index_control.content_size=strlen(content);
  return 0;
}





//This function adds a Resource Handler for the pages stats.html and formtest.html and associates stats , form and their callback functions
void init_dynamic_content(char * webroot)
{
  if (! AmmServer_AddResourceHandler(flashy_server,&index_control,"/index.html",webroot,4096,0,(void* ) &index_control_page,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding index page\n"); }
  AmmServer_DoNOTCacheResourceHandler(flashy_server,&index_control);
}

//This function destroys all Resource Handlers and free's all allocated memory..!
void close_dynamic_content()
{
    AmmServer_RemoveResourceHandler(flashy_server,&index_control,1);
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


