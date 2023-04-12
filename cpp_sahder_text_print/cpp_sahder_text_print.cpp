#include "ourGraphics.h"
#include <ctime>


struct pt2 { float x, y; };

Shader * shad_font; Shader * shad_chart;
unsigned int fontTexture;
int winx, winy;
float mxy, mny;
float vertScale = 1.0f, horScale = 1.0f;

vector<pt2> chartdata = {};

float func(float a, float b, float c, float x, int ftype)
{
	if (ftype == 0)
	return a * pow(x, 2) + b * x + c;
	if (ftype == 1)
	return a * sin(b*x*3.14f/180.0f + c);
	if (ftype == 2)
	return a * pow(x, 3) + b * pow(x,2) + c * x;
	if (ftype == 3)
	return a * tan(b*x*3.14f / 180.0f + c);
}

void prepChart(float xmin, float xstep, float xmax, vector<pt2> &cdata, float &miny, float &maxy, int ft)
{
	std::cout << "Building chart dataset..." << std::endl;
	cdata.clear();
	float cx = xmin;
	float cy = func(10, 3, -5, cx, ft);
	float ymin = cy;
	float ymax = cy;
	while (cx <= xmax)
	{
		cy = func(10, 3, -5, cx, ft);
		cdata.push_back({cx,cy});
		if (cy < ymin) ymin = cy;
		if (cy > ymax) ymax = cy;
		cx += xstep;
	}

	miny = ymin;
	maxy = ymax;
}

float scaleVal(float val, float scale)
{
	return val * scale;
}

flarr pointArrToFlArr(vector<pt2> cdata, float xscale, float yscale)
{
	flarr dat = {};
	for (int i = 0; i < cdata.size(); i++)
	{
		dat.push_back(scaleVal(cdata[i].x,xscale));
		dat.push_back(scaleVal(cdata[i].y,yscale));
		dat.push_back(0.0f);
		dat.push_back(1.0f);
		dat.push_back(0.0f);
		dat.push_back(0.0f);
	}
	return dat;
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

	srand(time(NULL));

	int ftu = rand() % 4;

	float ch_start_x = -100, ch_end_x = 100;

	OGLManager ogl(800, 600, framebuffer_size_callback);
	prepShaders();
	prepChart(ch_start_x,1, ch_end_x,chartdata,mny,mxy,ftu);

	fontTexture = makeTexture("font_map.png");

	while (!glfwWindowShouldClose(ogl.window))
	{
		processInput(ogl.window);

		glfwGetWindowSize(ogl.window, &winx, &winy);

		glm::mat4 projection = glm::ortho(
			(float)(-winx / 2), (float)(winx / 2),
			(float)(-winy / 2), (float)(winy / 2), 
			-10.0f, 10.0f);
		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 model = glm::mat4(1.0f);

		float modscale = 1.0f;

		model = glm::scale(model, glm::vec3(modscale));

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//setting up font shader
		shad_font->use();

		shad_font->setInt("fontTexture", 0);
		
		shad_font->setMatrix4f("projection", projection);
		shad_font->setMatrix4f("view", view);
		shad_font->setMatrix4f("model", model);

		shad_font->setBool("useColour", false);

		//setting up chart shader
		shad_chart->use();

		model = glm::mat4(1.0f);

		shad_chart->setMatrix4f("projection", projection);
		shad_chart->setMatrix4f("view", view);
		shad_chart->setMatrix4f("model", model);

		horScale = 0.9*(winx / (ch_end_x - ch_start_x));
		vertScale = 0.4*(winy/(mxy - mny));

		//draw hor axis
		

		//draw chart line
		drawOurVBO(pointArrToFlArr(chartdata,horScale,vertScale),6,GL_LINE_STRIP,3);

		//print header
		shad_font->use();
		printBitmapText(-(winx/2.0f)+20.0f,(winy/2.0f)-40.0f,30.0f,"Chart output",fontTexture);

		//print axis marks
		float chart_scales_stp = (ch_end_x - ch_start_x) / 10.0f;
		for (int i = 0; i < 10; i++)
		{
			float cPX = -(winx / 2.0f)+((float)i*(winx/10.0f));
			char str_val[128];
			sprintf_s(str_val, "%.1f", ch_start_x + chart_scales_stp * (float)i);
			printBitmapText(cPX, 30.0f, 25.0f, str_val, fontTexture);
		}

		ogl.endDraw();
	}

	return 0;
}
