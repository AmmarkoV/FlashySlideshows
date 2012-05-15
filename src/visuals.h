#ifndef VISUALS_H_INCLUDED
#define VISUALS_H_INCLUDED

void DisplayHUD(unsigned int view_instructions);
void MainDisplayFunction();
void DrawBackground();
int DrawEffects();

void DrawDecal(float x,float y,float z,float  rotation,float width,float height,unsigned int decal_type,char * text);

int MinPictureThatIsVisible();
int MaxPictureThatIsVisible();

void InitEffects();

int NewLabel(float x,float y,char * text);


extern unsigned int ENABLE_WIGGLING;
#endif // VISUALS_H_INCLUDED
