#pragma once

#include <iostream>
#include <ctime>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <cctype>
#include "shader.h"



using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;

typedef vector<float> flarr;
typedef vector<int> intarr;
typedef vector<char> smbarr;

class OGLManager
{
private:
	bool ready;
	bool gladLoaded;
	int wx, wy;
public:
	GLFWwindow* window;
	OGLManager(int pwx, int pwy, GLFWframebuffersizefun callback);
	OGLManager() { glfwTerminate(); };
	bool initOGL(int pwx, int pwy, GLFWframebuffersizefun callback);
	bool getReady() { return ready; };
	int getWX() { return wx; };
	int getWY() { return wy; };
	bool getGLADLoaded() { return gladLoaded; };
	void setWX(int val) { wx = val; };
	void setWY(int val) { wy = val; };
	void endDraw();
};

void drawOurVBO(flarr verts, int verts_block_size, GLenum objType, int vertAttrSize);
void drawOurEBO(flarr verts, intarr inds, unsigned int tex, int verts_block_size);
unsigned int makeTexture(string fileName);
int getSymbolId(char smb);
void getSymbolCoords(int rowWidth, int smbId, float &sx, float &sy, float &sw);
smbarr symbolsList();
void printBitmapText(float tx, float ty, float size, string txt, unsigned int fontTex);
float valToDevice(float dimension, float value);