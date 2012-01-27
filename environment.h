#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED

#include <unistd.h>

long timeval_diff ( struct timeval *difference, struct timeval *end_time, struct timeval *start_time );
int LoadStockTexturesAndSounds();
int UnLoadStockTexturesAndSounds();

#endif // ENVIRONMENT_H_INCLUDED
