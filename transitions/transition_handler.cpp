#include "transition_handler.h"

/*
  This is a basic transition handler for using a ZxN album like
  album outline
*/










/*
 ======================================
 ==================================================
 DEFAULT HANDLERS ======================================
 ==================================================
 ======================================
*/

//SEEK MOVEMENT  ======================================
int HandleState_OneTime_Start_BasicMovementToNextPicture()
{
  return 0;
}

int HandleState_During_BasicMovementToNextPicture()
{
  return 0;
}

int HandleState_OneTime_End_BasicMovementToNextPicture()
{
  return 0;
}


//KEN_BURNS(LIKE) EFFECT ======================================
int HandleState_OneTime_BasicStartEffect()
{
  return 0;
}

int HandleState_During_BasicEffect()
{
  return 0;
}

int HandleState_OneTime_BasicEndEffect()
{
  return 0;
}


int HandleState_OneTime_Start_MovementTo()
{
  return 0;
}

int HandleState_During_MovementTo()
{
  return 0;
}

int HandleState_OneTime_End_MovementTo()
{
  return 0;
}
