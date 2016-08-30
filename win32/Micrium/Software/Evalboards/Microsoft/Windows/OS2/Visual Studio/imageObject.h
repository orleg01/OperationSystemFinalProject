#ifndef __IMAGE_OBJECT__H
#define __IMAGE_OBJECT__H

#include "Mat3d.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>

typedef struct 
{
	GLuint textureId;
	vec3 position;
	float sizeX;
	float sizeY;
}imageObject_t;

imageObject_t initImageObject(int textureId, vec3 position, float sizeX, float sizeY);
void draw(imageObject_t* obj);
void initImageObjectShader();
int initTexture(const char* imageName);
void freeShaderImageObject();

#endif