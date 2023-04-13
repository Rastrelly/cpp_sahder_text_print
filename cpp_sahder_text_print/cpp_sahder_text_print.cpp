#include "ourGraphics.h"
#include <ctime>


struct pt2 { float x, y; };

Shader * shad_font; Shader * shad_chart;
unsigned int fontTexture;
int winx, winy;
float mxy, mny;

float vertScale = 1.0f, horScale = 1.0f;

vec3arr chartdata = {};

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

void prepChart(float xmin, float xstep, float xmax, vec3arr &cdata, float &miny, float &maxy, int ft)
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
		cdata.push_back({cx,cy,0.0f});
		if (cy < ymin) ymin = cy;
		if (cy > ymax) ymax = cy;
		cx += xstep;
	}

	miny = ymin;
	maxy = ymax;
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

void printAxisMarks(glm::vec3 ch_start, glm::vec3 ch_end, glm::vec3 aSize, glm::vec3 aShift)
{
	//print axis marks
	int nObjX = 10;
	int nObjY = 10;

	glm::vec3 osize;
	osize = { (int)winx / 10, (int)winy / 10, 0.0f };

	if (ch_start.x == ch_end.x) nObjX = 0;
	if (ch_start.y == ch_end.y) nObjY = 0;

	if (osize.x < 160) nObjX = 5;
	if (osize.y < 40) nObjY = 5;

	if (osize.x == 0) nObjX = 0;
	if (osize.y == 0) nObjY = 0;

	glm::vec3 chart_scales_stp;
	
	int nObj = 10;
	
	if (nObjX>nObjY)
		nObj = nObjX;
	else
		nObj = nObjY;

	chart_scales_stp = (ch_end - ch_start) / (float)nObj;

	for (int i = 0; i < nObj; i++)
	{

		float cPX=0.0f;
		float cPY=0.0f;
		if (nObjX > 0)
			cPX = -(winx / 2.0f) + ((float)i*(winx / (float)nObj));
		else
			cPX = ch_start.x;

		if (nObjY > 0)
			float cPY = -(winy / 2.0f) + ((float)i*(winy / (float)nObj));
		else
			cPY = ch_start.y;

		char str_val[128];
		if (nObjX > 0)
		{
			sprintf_s(str_val, "%.1f", ch_start.x + chart_scales_stp.x * (float)i);
		}
		if (nObjY > 0)
		{
			sprintf_s(str_val, "%.1f", ch_start.y + chart_scales_stp.y * (float)i);
		}
		printBitmapText(cPX+aShift.x, cPY+aShift.y, 25.0f, str_val, fontTexture);
		
	}
}

int main()
{

	srand(time(NULL));

	int ftu = rand() % 4;

	float ch_start_x = -100, ch_end_x = 100;

	OGLManager ogl(800, 600, framebuffer_size_callback);
	prepShaders();
	prepChart(ch_start_x,1,ch_end_x,chartdata,mny,mxy,ftu);

	fontTexture = makeTexture("font_map.png");

	while (!glfwWindowShouldClose(ogl.window))
	{
		processInput(ogl.window);

		glfwGetWindowSize(ogl.window, &winx, &winy);

		ogl.setDefaultProjections();

		ogl.setProjection(glm::ortho(
			(float)(-winx / 2), (float)(winx / 2),
			(float)(-winy / 2), (float)(winy / 2), 
			-10.0f, 10.0f));

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//setting up font shader
		shad_font->use();

		shad_font->setInt("fontTexture", 0);
		shad_font->setBool("useColour", false);

		ogl.updateProjectionForShader(shad_font);
		
		//setting up chart shader
		shad_chart->use();
		ogl.updateProjectionForShader(shad_chart);

		horScale = 1*(winx / (ch_end_x - ch_start_x));
		vertScale = 0.4*(winy/(mxy - mny));

		//draw hor axis
		drawLine({-winx/2.0f,0.0f,0.0f}, { winx / 2.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });
		
		//draw vert axis
		float dAx = 0.0f - ch_start_x;
		float wAx = ch_end_x - ch_start_x;
		float axX = (-winx/2.0f) + (winx*(dAx / wAx));
		drawLine({ axX,-winy / 2.0f,0.0f }, { axX,winx / 2.0f,0.0f }, { 1.0f,1.0f,1.0f });

		//draw chart line
		drawChartLine(chartdata, { 1.0f, 0.0f, 0.0f }, horScale, vertScale, 1.0f);
	
		//print header
		shad_font->use();
		printBitmapText(-(winx/2.0f)+20.0f,(winy/2.0f)-40.0f,30.0f,"Chart output",fontTexture);
		shad_font->use();
		printBitmapText(-(winx / 2.0f) + 20.0f, (winy / 2.0f) - 75.0f, 30.0f, "Func "+to_string(ftu), fontTexture);

		//print axis marks
		printAxisMarks({ ch_start_x,0.0f,0.0f }, { ch_end_x,0.0f,0.0f }, { 160.0f,0.0f,0.0f }, {0.0f,-30.0f,0.0f});
		printAxisMarks({ axX,mny,0.0f }, { axX,mxy,0.0f }, { 0.0f,40.0f,0.0f }, { -150.0f,0.0f,0.0f });

		/*int nObj = 10;
		int osize = (int)winx / 10;
		if (osize < 160) nObj = 5;
		float chart_scales_stp = (ch_end_x - ch_start_x) / (float)nObj;

		for (int i = 0; i < nObj; i++)
		{
			float cPX = -(winx / 2.0f)+((float)i*(winx/(float)nObj));
			char str_val[128];
			sprintf_s(str_val, "%.1f", ch_start_x + chart_scales_stp * (float)i);
			printBitmapText(cPX, -30.0f, 25.0f, str_val, fontTexture);
		}*/

		ogl.endDraw();
	}

	return 0;
}
