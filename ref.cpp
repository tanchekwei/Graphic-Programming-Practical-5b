#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <GL/glext.h>
#include <tchar.h>
#include <wingdi.h>
#include <windef.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks);
void drawCuboid(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ);
void drawLine();
void drawCurve();
void drawPencil();
void drawTowerBlock();
void drawRoadsidePoles();
void drawBridge();
void drawRoad();

float rotateAngle = 0.0f;
float viewDistance = -1000.0f;
float rotateY = 0.0f;

GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == 0x4F) //O key
		{
			if (rotateAngle >= 0.0f && rotateAngle < 60.0f) {
				rotateAngle += 1.0f;
			}
		}
		else if (wParam == 0x43) //C key
		{
			if (rotateAngle > 0.0f && rotateAngle <= 60.0f) {
				rotateAngle -= 1.0f;
			}
		}
		else if (wParam == VK_UP) {
			viewDistance += 10.0f;
		}
		else if (wParam == VK_DOWN) {
			viewDistance -= 10.0f;
		}
		else if (wParam == VK_LEFT) {
			rotateY -= 10.0f;
		}
		else if (wParam == VK_RIGHT) {
			rotateY += 10.0f;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void LoadBitmapImage (const char *filename) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
}

void endTexture() {
	glDisable(GL_TEXTURE_2D);
	DeleteObject(hBMP);
	glDeleteTextures(1, &texture);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, viewDistance);
	glRotatef(rotateY, 0, 1, 0);

	//left tower
	glPushMatrix();
		glTranslatef(-250, 0, 0);
		drawTowerBlock();
	glPopMatrix();

	//right tower
	glPushMatrix();
		glTranslatef(250, 0, 0);
		drawTowerBlock();
	glPopMatrix();

	//right roadside pole BACK
	glPushMatrix();
		glTranslatef(300, 0, -50);
		drawRoadsidePoles();
	glPopMatrix();

	//right roadside pole FRONT
	glPushMatrix();
		glTranslatef(300, 0, 50);
		drawRoadsidePoles();
	glPopMatrix();

	//left roadside pole BACK
	glPushMatrix();
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(300, 0, -50);
		drawRoadsidePoles();
	glPopMatrix();

	//left roadside pole FRONT
	glPushMatrix();
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(300, 0, 50);
		drawRoadsidePoles();
	glPopMatrix();

	//Left Bridge
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		LoadBitmapImage("images/street.bmp");
		glTranslatef(-200.0f, 0.0f, 0.0f);
		glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
		drawBridge();
		endTexture();
	glPopMatrix();

	//Right Bridge
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		LoadBitmapImage("images/street.bmp");
		glTranslatef(200.0f, 0.0f, 0.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
		drawBridge();
		endTexture();
	glPopMatrix();

	//Right Road
	glPushMatrix();
		glTranslatef(500.0f, 0.0f, 0.0f);
		drawRoad();
	glPopMatrix();

	//Left Road
	glPushMatrix();
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(500.0f, 0.0f, 0.0f);
		drawRoad();
	glPopMatrix();

	//link bridge FRONT
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		LoadBitmapImage("images/linkbridge.bmp");
		drawCuboid(200, 300, 30, 200, 300, 50, 200, 275, 30, 200, 275, 50);
		endTexture();
	glPopMatrix();

	//link bridge BACK
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		LoadBitmapImage("images/linkbridge.bmp");
		drawCuboid(200, 300, -50, 200, 300, -30, 200, 275, -50, 200, 275, -30);
		endTexture();
	glPopMatrix();
}
//--------------------------------------------------------------------

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks) {
	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);

	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricTexture(cylinder, GLU_TRUE);

	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);
}
void drawCone(double baseRadius, double height, int slices, int stacks) {
	GLUquadricObj *cone = NULL;
	cone = gluNewQuadric();
	gluQuadricDrawStyle(cone, GLU_LINE);
	gluCylinder(cone, baseRadius, 0, height, slices, stacks);
	gluDeleteQuadric(cone);
}
void drawCuboid(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ) {
	glBegin(GL_QUADS);
	//Face 1
	glTexCoord2f(0, 1);
		glVertex3f(topLeftX, topLeftY, topLeftZ);
	glTexCoord2f(0, 0);
		glVertex3f(botLeftX, botLeftY, botLeftZ);
	glTexCoord2f(1, 0);
		glVertex3f(botRightX, botRightY, botRightZ);
	glTexCoord2f(1, 1);
		glVertex3f(topRightX, topRightY, topRightZ);
	//Face 2
	glTexCoord2f(0, 1);
		glVertex3f(topLeftX, topLeftY, topLeftZ);
	glTexCoord2f(0, 0);
		glVertex3f(botLeftX, botLeftY, botLeftZ);
	glTexCoord2f(1, 0);
		glVertex3f(-botLeftX, botLeftY, botLeftZ);
	glTexCoord2f(1, 1);
		glVertex3f(-topLeftX, topLeftY, topLeftZ);
	//Face 3
	glTexCoord2f(0, 1);
		glVertex3f(-topLeftX, topLeftY, topLeftZ);
	glTexCoord2f(0, 0);
		glVertex3f(-botLeftX, botLeftY, botLeftZ);
	glTexCoord2f(1, 0);
		glVertex3f(-botRightX, botRightY, botRightZ);
	glTexCoord2f(1, 1);
		glVertex3f(-topRightX, topRightY, topRightZ);
	//Face 4
	glTexCoord2f(0, 1);
		glVertex3f(-topRightX, topRightY, topRightZ);
	glTexCoord2f(0, 0);
		glVertex3f(-botRightX, botRightY, botRightZ);
	glTexCoord2f(1, 0);
		glVertex3f(botRightX, botRightY, botRightZ);
	glTexCoord2f(1, 1);
		glVertex3f(topRightX, topRightY, topRightZ);
	//Face 5
	glTexCoord2f(0, 1);
		glVertex3f(topRightX, topRightY, topRightZ);
	glTexCoord2f(0, 0);
		glVertex3f(topLeftX, topLeftY, topLeftZ);
	glTexCoord2f(1, 0);
		glVertex3f(-topLeftX, topLeftY, topLeftZ);
	glTexCoord2f(1, 1);
		glVertex3f(-topRightX, topRightY, topRightZ);

	glEnd();
}
void drawLine() {
	glLineWidth(10);
	glBegin(GL_LINE);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(250, 325,-50);
	glVertex3f(700, 0.0, -50);
	glEnd();
}
void drawCurve() {
	/*float radius = 200;
	float twoPI = 2 * 3.142;

	glPointSize(5.0);
	glBegin(GL_POINTS);
	for (float i = twoPI / 4; i <= 3 * twoPI / 4; i += 0.001)
	{
		glVertex2f((sin(i)*radius), (cos(i)*radius) + -0.1);
	}
	glEnd();*/

	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for (int i = 200; i <= 300; i++)
	{
		float x = cos((2 * i*3.14) / 360) * 300;
		float y = sin((2 * i*3.14) / 360) * 300;
		glVertex2f(x, y);
	}
	glEnd();
}


void drawPencil() {
	double baseRadius = 10;
	double topRadius = 10;
	double cylinderHeight = 350;
	double coneHeight = 20;
	int slices = 30;
	int stacks = 30;

	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		LoadBitmapImage("images/towercolpil.bmp");
		glTranslatef(0, 350, 0);
		glRotatef(90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight, slices, stacks);
		endTexture();
	glPopMatrix();

	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		LoadBitmapImage("images/towercolpil.bmp");
		glTranslatef(0, 350, 0);
		glRotatef(-90, 1, 0, 0);
		drawCone(baseRadius, coneHeight, slices, stacks);
		endTexture();
	glPopMatrix();

}

void drawTowerBlock() {
		//tower
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			LoadBitmapImage("images/towerwaterface.bmp");
			drawCuboid(-50, 325, 50, -50, 325, -50, -50, 0, 50, -50, 0, -50);
			endTexture();
		glPopMatrix();

		//tower dome
		glPushMatrix();
			//glColor3f(0.917647, 0.917647, 0.678431);
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			LoadBitmapImage("images/towercolpil.bmp");
			drawCuboid(-10, 390, 10, -10, 390, -10, -50, 325, 50, -50, 325, -50);
			endTexture();
		glPopMatrix();

		//tower roof cube front
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			LoadBitmapImage("images/domewindow.bmp");
			drawCuboid(-25, 350, 50, -20, 350, -50, -25, 325, 50, -25, 325, -50);
			endTexture();
		glPopMatrix();

		//tower roof cube side
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			LoadBitmapImage("images/domewindow.bmp");
			drawCuboid(-50, 350, 25, -50, 350, -25, -50, 325, 25, -50, 325, -25);
			endTexture();
		glPopMatrix();

		//pencil top left
		glPushMatrix();
			glTranslatef(-50, 0, -50);
			drawPencil();
		glPopMatrix();

		//pencil top right
		glPushMatrix();
			glTranslatef(-50, 0, 50);
			drawPencil();
		glPopMatrix();

		//pencil bottom left
		glPushMatrix();
			glTranslatef(50, 0, -50);
			drawPencil();
		glPopMatrix();

		//pencil bottom left
		glPushMatrix();
			glTranslatef(50, 0, 50);
			drawPencil();
		glPopMatrix();

		//tower base
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			LoadBitmapImage("images/towerbasepoly.bmp");
			drawCuboid(-70, 0, 70, -70, 0, -70, -70, -130, 70, -70, -130, -70);
			endTexture();
		glPopMatrix();


		////tower base cyclinder platform front
		//glPushMatrix();
		//	glTranslatef(0, 0, -50);
		//	glRotatef(90, 1, 0, 0);
		//	drawCylinder(50, 50, 100, 30, 30);
		//glPopMatrix();

		////tower base cyclinder platform back
		//glPushMatrix();
		//	glTranslatef(0, 0, 50);
		//	glRotatef(90, 1, 0, 0);
		//	drawCylinder(50, 50, 100, 30, 30);
		//glPopMatrix();

}

void drawRoadsidePoles() {
	double baseRadius = 1;
	double topRadius = 1;
	double cylinderHeight = 190;
	int slices = 30;
	int stacks = 30;

	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(50, 0, 0);
		glRotatef(-90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight, slices, stacks);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(100, 0, 0);
		glRotatef(-90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight-60, slices, stacks);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(150, 0, 0);
		glRotatef(-90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight-90, slices, stacks);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.0f, 0.5f, 1.0f);
		glRotatef(-10, 0.0f, 0.0f, 1);
		glTranslatef(240, 400, 0);
		drawCurve();
	glPopMatrix();



}

void drawBridge() {
	//drawCuboid(-100.0f, 5.0f, 50.0f, -100.0f, 5.0f, -50.0f, -100.0f, 0.0f, 50.0f, -100.0f, 0.0f, -50.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
			glVertex3f(0.0f, 5.0f, 50.0f);
		glTexCoord2f(0, 1);
			glVertex3f(0.0f, 5.0f, -50.f);
		glTexCoord2f(1, 1);
			glVertex3f(200.0f, 5.0f, -50.0f);
		glTexCoord2f(1, 0);
			glVertex3f(200.0f, 5.0f, 50.0f);
	glEnd();
}

void drawRoad() {
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		LoadBitmapImage("images/street.bmp");
		drawCuboid(-200.0f, 5.0f, 50.0f, -200.0f, 5.0f, -50.0f, -200.0f, 0.0f, 50.0f, -200.0f, 0.0f, -50.0f);
		endTexture();
	glPopMatrix();
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-500, 500, -500, 500, -500, 500);


	gluPerspective(70, 1, 0.6, 2000);
	

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
