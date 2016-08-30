#include "imageObject.h"

#include <GL/glew.h>

#define TO_STR(STR) (#STR)

const GLchar vertexShaderSource[] = { #include "vertexImageObject.gl" };

const GLchar* fragmentShaderSource = TO_STR(#include "fragImageObject.gl");

void draw()
{

}

int initImageObjectShader()
{

}

int initTexture(const char* imageName)
{

}

imageObject_t initImageObject(int shaderId, int textureId, vec3 position, float sizeX, float sizeY)
{
	imageObject_t out;
	out.shaderId = shaderId;
	out.textureId = textureId;
	out.position = position;
	out.sizeX = sizeX;
	out.sizeY = sizeY;
	return out;
}
