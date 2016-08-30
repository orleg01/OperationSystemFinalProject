#include "imageObject.h"

#include <GL/glew.h>
#include <includes.h>

const GLchar* vertexShaderSource =  "#version 330 core\n"
									"layout (location = 0) in vec3 position;\n"
									"void main()\n"
									"{\n"
									"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
									"}\0";

const GLchar* fragmentShaderSource =	"#version 330 core\n"
										"out vec4 color;\n"
										"void main()\n"
										"{\n"
										"	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
										"}\n\0";

GLuint shaderImageObject;
GLuint vaoImageObject;
GLuint vboImageObject;
GLuint eboImageObject;

void draw(imageObject_t* obj)
{
	// Draw container
	glBindVertexArray(vaoImageObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void initImageObjectShader()
{
	GLfloat rect[] = {		//the rect
		// Positions          // Texture Coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	GLint success;
	GLchar infoLog[512];
	GLuint vertexShader;
	GLuint fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		APP_TRACE("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		APP_TRACE("%s\n",infoLog);
	}
	// Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		APP_TRACE("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
		APP_TRACE("%s\n", infoLog);
	}
	// Link shaders
	shaderImageObject = glCreateProgram();
	glAttachShader(shaderImageObject, vertexShader);
	glAttachShader(shaderImageObject, fragmentShader);
	glLinkProgram(shaderImageObject);
	// Check for linking errors
	glGetProgramiv(shaderImageObject, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderImageObject, 512, NULL, infoLog);
		APP_TRACE("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
		APP_TRACE("%s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//generate the rect
	glGenVertexArrays(1, &vaoImageObject);
	glGenBuffers(1, &vboImageObject);
	glGenBuffers(1, &eboImageObject);

	glBindVertexArray(vaoImageObject);

	glBindBuffer(GL_ARRAY_BUFFER, vboImageObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboImageObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

int initTexture(const char* imageName)
{

}

void freeShaderImageObject()
{
	glDeleteVertexArrays(1, &vaoImageObject);
	glDeleteBuffers(1, &vboImageObject);
	glDeleteBuffers(1, &eboImageObject);
}

imageObject_t initImageObject(int textureId, vec3 position, float sizeX, float sizeY)
{
	imageObject_t out;
	out.textureId = textureId;
	out.position = position;
	out.sizeX = sizeX;
	out.sizeY = sizeY;
	return out;
}
