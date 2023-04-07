#include "ourGraphics.h"


struct pt2 { float x, y; };

Shader * shad_font; Shader * shad_chart;
unsigned int fontTexture;
int winx, winy;

vector<pt2>chartdata = {};

float func(float a, float b, float c, float x)
{
	return a * pow(x, 2) + b * x + c;
}

void prepcahrt(float xmin, float xstep, float xmax, vector<pt2> &cdata)
{
	cdata.clear();
	float cx = xmin;
	while (cx <= xmax)
	{
		cdata.push_back({ cx,func(10,3,-5,cx)});
	}
}

void prepShaders()
{
	std::cout << "Preparing shaders..." << std::endl;
	shad_font = new Shader("shader_font_vert.gls", "shader_font_frag.gls");
	shad_chart = new Shader("shader_chart_vert.gls", "shader_chart_frag.gls");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	OGLManager ogl(800, 600, framebuffer_size_callback);
	prepShaders();
	fontTexture = makeTexture("font_map.png");

	while (!glfwWindowShouldClose(ogl.window))
	{
		processInput(ogl.window);

		glfwGetWindowSize(ogl.window, &winx, &winy);

		glm::mat4 projection = glm::ortho((float)(-winx / 2), (float)(winx / 2),
			(float)(-winy / 2), (float)(winy / 2), -10.0f, 10.0f);
		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 model = glm::mat4(1.0f);

		float modscale = 1.0f;
		if (winx > winy) modscale = (float)winy / 2;
		else modscale = (float)winx / 2;

		model = glm::scale(model, glm::vec3(modscale));

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shad_font->use();

		shad_font->setInt("fontTexture", 0);
		
		shad_font->setMatrix4f("projection", projection);
		shad_font->setMatrix4f("view", view);
		shad_font->setMatrix4f("model", model);

		shad_font->setBool("useColour", false);

		shad_chart->use();

		shad_chart->setInt("fontTexture", 0);

		shad_chart->setMatrix4f("projection", projection);
		shad_chart->setMatrix4f("view", view);
		shad_chart->setMatrix4f("model", model);

		shad_chart->setBool("useColour", false);

		shad_font->use();
		printBitmapText(-1.0f,0.9f,0.1f,"Chart output",fontTexture);

		ogl.endDraw();
	}

	return 0;
}
