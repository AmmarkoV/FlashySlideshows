#ifndef MATH_3D_H_INCLUDED
#define MATH_3D_H_INCLUDED

int rayIntersectsTriangle(float *p, float *d,float *v0, float *v1, float *v2);
int rayIntersectsRectangle(float *p, float *d,float *v0, float *v1, float *v2, float *v3);
float distanceBetween3DPoints(float *x1,float*y1,float *z1,float *x2,float*y2,float *z2);

#endif // MATH_3D_H_INCLUDED
