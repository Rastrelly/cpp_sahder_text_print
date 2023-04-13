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
typedef vector<glm::vec3> vec3arr;

class OGLManager
{
private:
	bool ready;
	bool gladLoaded;
	int wx, wy;
	
public:
	glm::mat4 projection, view, model;
	GLFWwindow* window;
	OGLManager(int pwx, int pwy, GLFWframebuffersizefun callback);
	~OGLManager() { glfwTerminate(); };
	bool initOGL(int pwx, int pwy, GLFWframebuffersizefun callback);
	bool getReady() { return ready; };
	int getWX() { return wx; };
	int getWY() { return wy; };
	bool getGLADLoaded() { return gladLoaded; };
	void setWX(int val) { wx = val; };
	void setWY(int val) { wy = val; };
	void endDraw();
	void setDefaultProjections() { 
		projection = glm::mat4(1.0f); view = glm::mat4(1.0f); model = glm::mat4(1.0f);	};
	glm::mat4 getProjection() { return projection; };
	glm::mat4 getModel() { return model; };
	glm::mat4 getView() { return view; };
	void setProjection(glm::mat4 val) { projection = val; };
	void setModel(glm::mat4 val) { model = val; };
	void setView(glm::mat4 val) { view = val; };
	void updateProjectionForShader(Shader * shader);
};

void drawOurVBO(flarr verts, int verts_block_size, GLenum objType, int vertAttrSize);
void drawOurEBO(flarr verts, intarr inds, unsigned int tex, int verts_block_size);
unsigned int makeTexture(string fileName);
int getSymbolId(char smb);
void getSymbolCoords(int rowWidth, int smbId, float &sx, float &sy, float &sw);
smbarr symbolsList();
void printBitmapText(Shader * sh, float tx, float ty, float size, string txt, unsigned int fontTex);
float valToDevice(float dimension, float value);
void drawChartLine(Shader * shad, vec3arr chartPoints, glm::vec3 colour, float horScale, float vertScale, float depthScale);
flarr pointArrToFlArr(vec3arr cdata, glm::vec3 colour, float xscale, float yscale, float zscale);
float scaleVal(float val, float scale);
void drawLine(Shader * shad, glm::vec3 p1, glm::vec3 p2, glm::vec3 colour);