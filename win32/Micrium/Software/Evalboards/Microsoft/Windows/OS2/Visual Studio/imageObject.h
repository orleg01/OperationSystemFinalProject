#ifndef __IMAGE_OBJECT__H
#define __IMAGE_OBJECT__H

#include "Mat3d.h"

typedef struct 
{
	int shaderId;
	int textureId;
	vec3 position;
	float sizeX;
	float sizeY;
}imageObject_t;

imageObject_t initImageObject(int shaderId, int textureId, vec3 position, float sizeX, float sizeY);
void draw();
int initImageObjectShader();
int initTexture(const char* imageName);

#endif