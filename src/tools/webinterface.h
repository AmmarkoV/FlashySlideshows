#ifndef WEBINTERFACE_H_INCLUDED
#define WEBINTERFACE_H_INCLUDED


int WebInterfaceCompiledIn();

int StartWebInterface(char * IP , unsigned int port , char * fileroot ,char * app_clipart);
int StopWebInterface();

#endif // WEBINTERFACE_H_INCLUDED
