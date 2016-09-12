#include "imageObject.h"

#include <GL/glew.h>
#include <includes.h>
#include <SOIL\SOIL.h>
#include "ArrayList.h"

const GLchar* vertexShaderSource = "#version 330 core\n"
									"layout (location = 0) in vec3 position;\n"
									"layout (location = 1) in vec2 textCoord;\n"

									"uniform float op;\n"
									"uniform vec3 pos;\n"
									"uniform vec2 size;\n"
									"uniform vec2 screenSize;\n"

									"out vec2 TexCoord;\n"

									"void main()\n"
									"{\n"
									
									"	gl_Position = vec4((position.x * size.x + pos.x) / (screenSize.x / 2), (position.y * size.y + pos.y) / (screenSize.y / 2), position.z + pos.z, 1.0);\n"
									"	TexCoord = textCoord;\n"
									"	if(op != 0)"
									"		TexCoord.y = 1 - TexCoord.y;\n"
									"	TexCoord.y = 1 - TexCoord.y;\n"
									"}\0";

const GLchar* fragmentShaderSource =	"#version 330 core\n"
										"out vec4 color;\n"
										"in vec2 TexCoord;\n"
										"uniform sampler2D ourTexture;\n"
										"void main()\n"
										"{\n"
										"	color = texture(ourTexture, TexCoord);\n"
										"}\n\0";

GLuint shaderImageObject;
GLuint vaoImageObject;
GLuint vboImageObject;
GLuint eboImageObject;

void draw(imageObject_t* obj)
{
	// Draw  
	glBindTexture(GL_TEXTURE_2D, obj->textureId);
	glUseProgram(shaderImageObject);

	GLint vectorLoc = glGetUniformLocation(shaderImageObject,"pos");
	glUniform3f(vectorLoc, obj->position.X_, obj->position.Y_,obj->position.Z_);
	
	vectorLoc = glGetUniformLocation(shaderImageObject, "size");
	glUniform2f(vectorLoc, obj->sizeX, obj->sizeY);

	vectorLoc = glGetUniformLocation(shaderImageObject, "screenSize");
	glUniform2f(vectorLoc, 800,600);

	vectorLoc = glGetUniformLocation(shaderImageObject, "op");
	glUniform1f(vectorLoc, obj->op);

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
		sendMassage(ERROR_SHADER_VERTEX_COMPILATION_FAILED);
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
		sendMassage(ERROR_SHADER_FRAGMENT_COMPILATION_FAILED);
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
		sendMassage(ERROR_SHADER_PROGRAM_LINKING_FAILED);
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
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

GLuint initTexture(const char* imageName)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(imageName, &width, &height, 0, SOIL_LOAD_RGBA);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

void freeShaderImageObject()
{
	glDeleteVertexArrays(1, &vaoImageObject);
	glDeleteBuffers(1, &vboImageObject);
	glDeleteBuffers(1, &eboImageObject);
}

imageObject_t initImageObject(int textureId, vec3 position, float sizeX, float sizeY , int op , float startPos)
{
	imageObject_t out;
	out.textureId = textureId;
	out.position = position;
	out.sizeX = sizeX;
	out.sizeY = sizeY;
	out.op = op;
	out.startPos = startPos;
	out.angle = 0;
	return out;
}
