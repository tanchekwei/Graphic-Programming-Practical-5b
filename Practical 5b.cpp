#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>
#define WINDOW_TITLE "OpenGL Window"

void drawCone();
void drawTower();
void drawPencil();

void drawCloud(float size);
void drawCirle(float x1, float y1, float radius);

void drawCuboid(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ);
void drawBridgeCuboid(float size, float widthScale, float thinness, float longness);
void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks);
void drawBridge();
void drawBridgeLine();
void drawMoon();

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
float speed = 0;
bool rotate = false;
float z = -8, x = 0, y = 0, bridgeDegree = 0, bridgeLineUp = 0;

bool isOrtho = false;

bool isLift = false;

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        else if (wParam == VK_F1)
        {
            if (isOrtho)
                isOrtho = false;
            else
                isOrtho = true;
        }
        else if (wParam == VK_F2)
        {
            if (isLift)
                isLift = false;
            else
                isLift = true;
        }
        else if (wParam == VK_SPACE)
        {
            if (rotate)
                rotate = false;
            else
                rotate = true;
        }
        else if (wParam == VK_RIGHT)
            x += 0.1;
        else if (wParam == VK_LEFT)
            x -= 0.1;
        else if (wParam == VK_UP)
            y += 0.1;
        else if (wParam == VK_DOWN)
            y -= 0.1;
        else if (wParam == VK_NUMPAD8)
            z += 0.1;
        else if (wParam == VK_NUMPAD2)
            z -= 0.1;
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

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // glLoadIdentity();
    // glTranslatef( 0, 0, -5 );

    glRotatef(speed, 0, 1, 0);

    // glPushMatrix();
    // {
    //     glScalef(0.5, 0.5, 0.5);
    //     drawBridge();
    // }
    // glPopMatrix();

    float towerDistance = 1.25;
    glPushMatrix();
    {
        glTranslatef(towerDistance, 0, 0);
        glScalef(0.6, 0.6, 0.6);
        glRotatef(180, 0, 0, 1);
        drawTower();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-towerDistance, 0, 0);
        glScalef(0.6, 0.6, 0.6);
        glRotatef(180, 0, 0, 1);
        drawTower();
    }
    glPopMatrix();

    drawBridge();

    glPushMatrix();
    {
        glTranslatef(-0.8, -1.2 + bridgeLineUp, 0);
        glRotatef(bridgeDegree, 0, 0, 1);
        glColor3ub(146, 113, 55);
        drawBridgeLine();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(180, 0, 1, 0);
        glTranslatef(-0.8, -1.2 + bridgeLineUp, 0);
        glRotatef(bridgeDegree, 0, 0, 1);
        glColor3ub(146, 113, 55);
        drawBridgeLine();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3ub(252, 212, 64);
        glTranslatef(0, 5, 0);
        drawMoon();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-5, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-7, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-2, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(4, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();
    
}

void drawCone()
{
    glPushMatrix();
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    // glRotatef(270, 1, 0, 0);
    // glTranslatef(0, 0, -1);
    // glColor3ub(186, 129, 83);
    // glPointSize(2);
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluCylinder(cylinder, 0.01, 0.3, 1, 20, 20);
    gluDeleteQuadric(cylinder);
    glPopMatrix();

    glPushMatrix();
    cylinder = gluNewQuadric();
    // glRotatef(270, 1, 0, 0);
    // glTranslatef(0, 0, -1);
    glColor3ub(98, 66, 40);
    gluQuadricDrawStyle(cylinder, GLU_LINE);
    gluCylinder(cylinder, 0.011, 0.31, 1, 5, 5);
    gluDeleteQuadric(cylinder);
    glPopMatrix();
}

void drawTower()
{
    float towerWidth = 0.4;
    glPushMatrix();
    {
        glScalef(2.5, 1.05, 2.5);
        glTranslatef(0, -0.15, 0);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-towerWidth, 0, towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(towerWidth, 0, towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(towerWidth, 0, -towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-towerWidth, 0, -towerWidth);
        drawPencil();
    }
    glPopMatrix();

    glPushMatrix();
    {
        float towerCubeWidth = 0.51;
        glColor3ub(131, 83, 65);
        drawCuboid(-towerCubeWidth, towerCubeWidth * 5.85, towerCubeWidth,
                   -towerCubeWidth, towerCubeWidth * 5.85, -towerCubeWidth,
                   -towerCubeWidth, 0, towerCubeWidth, -towerCubeWidth,
                   0, -towerCubeWidth);
    }
    glPopMatrix();

    // glPushMatrix();
    // {
    //     GLUquadricObj *cylinder = NULL;
    //     cylinder = gluNewQuadric();
    //     glColor3ub(234, 171, 102);
    //     glRotatef(-90, 1, 0, 0);
    //     glTranslatef(0, 0, 0);
    //     gluCylinder(cylinder, 0.4, 0.4, 2, 15, 15);
    //     gluDeleteQuadric(cylinder);
    // }
    // glPopMatrix();
}

void drawPencil()
{
    glPushMatrix();
    {
        glScalef(0.5, 1, 0.5);

        glPushMatrix();
        {
            glRotatef(270, 1, 0, 0);
            glTranslatef(0, 0, -1);
            glColor3ub(186, 129, 83);
            glPointSize(2);
            drawCone();
        }
        glPopMatrix();

        glPushMatrix();
        {
            double baseRadius = 0.3;
            double topRadius = 0.3;
            double cylinderHeight = 3;
            int slices = 30;
            int stacks = 30;
            glRotatef(-90, 1, 0, 0);
            drawCylinder(baseRadius, topRadius, cylinderHeight, slices, stacks);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawCuboid(float topLeftX, float topLeftY, float topLeftZ,
                float topRightX, float topRightY, float topRightZ,
                float botLeftX, float botLeftY, float botLeftZ,
                float botRightX, float botRightY, float botRightZ)
{
    glPushMatrix();
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
    glPopMatrix();
}

void drawBridgeCuboid(float size, float widthScale, float thinness, float longness)
{
    glBegin(GL_QUADS);
    // front
    // glColor3ub(30, 136, 229);
    glVertex3f(0, 0, size / widthScale);
    glVertex3f(size / thinness, 0, size / widthScale);
    glVertex3f(size / thinness, 0, 0);
    glVertex3f(0, 0, 0);

    // left
    // glColor3ub(223, 120, 239);
    glVertex3f(0, size * longness, size / widthScale);
    glVertex3f(0, 0, size / widthScale);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size * longness, 0);

    // bottom
    // glColor3ub(128, 226, 126);
    glVertex3f(0, size * longness, 0);
    glVertex3f(size / thinness, size * longness, 0);
    glVertex3f(size / thinness, 0, 0);
    glVertex3f(0, 0, 0);

    // right
    // glColor3ub(255, 255, 114);
    glVertex3f(size / thinness, 0, size / widthScale);
    glVertex3f(size / thinness, size * longness, size / widthScale);
    glVertex3f(size / thinness, size * longness, 0);
    glVertex3f(size / thinness, 0, 0);

    // behind
    // glColor3ub(255, 201, 71);
    glVertex3f(size / thinness, size * longness, size / widthScale);
    glVertex3f(0, size * longness, size / widthScale);
    glVertex3f(0, size * longness, 0);
    glVertex3f(size / thinness, size * longness, 0);

    // top
    // glColor3ub(115, 232, 255);
    glVertex3f(0, size * longness, size / widthScale);
    glVertex3f(size / thinness, size * longness, size / widthScale);
    glVertex3f(size / thinness, 0, size / widthScale);
    glVertex3f(0, 0, size / widthScale);

    glEnd();
}

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks)
{
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);

    gluQuadricNormals(cylinder, GLU_SMOOTH);
    gluQuadricTexture(cylinder, GLU_TRUE);

    gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(cylinder);
}

void drawBridge()
{
    // top bridge
    float bridgeWidth = 1.287;
    float bridgeHeight = 0.15;
    float bridgeUp = -0.15;
    float bridgeDown = -1;

    glPushMatrix();
    {
        glColor3ub(104, 160, 176);
        glTranslatef(0, bridgeUp, -0.21);
        glRotatef(90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3ub(104, 160, 176);
        glTranslatef(0, bridgeUp + 0.05, -0.21);
        glRotatef(-90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-1.2, bridgeDown + 0.05, -0.21);
        glRotatef(bridgeDegree, 0, 0, 1);
        glColor3ub(183, 141, 69);
        glRotatef(270, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 0.8);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(1.2, bridgeDown, -0.21);
        glRotatef(-bridgeDegree, 0, 0, 1);
        glColor3ub(183, 141, 69);
        glRotatef(-270, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 0.8);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3ub(104, 160, 176);
        glTranslatef(-1.5, bridgeDown, -0.21);
        glRotatef(90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3ub(104, 160, 176);
        glTranslatef(1.5, bridgeDown + 0.05, -0.21);
        glRotatef(-90, 0, 0, 1);
        drawBridgeCuboid(1.5, 4, 30, 1);
    }
    glPopMatrix();
}

void drawBridgeLine()
{
    glPushMatrix();
    {
        glLineWidth(10);
        glBegin(GL_LINES);
        {
            float x = 0.5 * sin(42);
            float y = 0.5 * cos(42);
            glVertex3f(x, y, 0); // First endpoint of line

            x = 0.5 * sin(42 + 3.142);
            y = 0.5 * cos(42 + 3.142);
            glVertex3f(x, y, 0); // Second endpoint of line
        }
        glEnd();
        glLineWidth(1);
    }
    glPopMatrix();
}

void drawMoon()
{
    float x = 0, y = 0, z = -15, GL_PI = 3.142, radius = 0.4;
    glPushMatrix();
    {
        // Call only once for all remaining points
        glBegin(GL_LINES);

        // All lines lie in the xy plane.
        for (float angle = 0.0f; angle <= GL_PI; angle += (GL_PI / 50.0f))
        {
            // Top half of the circle
            x = radius * sin(angle);
            y = radius * cos(angle);
            glVertex3f(x + 1, y + 2, z); // First endpoint of line

            // Bottom half of the circle
            x = radius * sin(angle + GL_PI);
            y = radius * cos(angle + GL_PI);
            glVertex3f(x + 1, y + 2, z); // Second endpoint of line
        }
        // Done drawing points
        glEnd();
    }
    glPopMatrix();
}

void drawCloud(float size)
{
    drawCirle(0, 0, size);
    drawCirle(0.2, 0, size);

    drawCirle(0.35, 0.1, size);

    drawCirle(0.2, 0.2, size);
    drawCirle(0, 0.2, size);

    drawCirle(-0.15, 0.1, size);
}

void drawCirle(float x1, float y1, float radius)
{
    float x2 = x1, y2 = y1;

    glColor3ub(246, 246, 246);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);
    for (float angle = 0; angle <= 360; angle += 2)
    {
        x2 = x1 + cos(angle) * radius;
        y2 = y1 + sin(angle) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
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

    if (!RegisterClassEx(&wc))
        return false;

    HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
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
    if (!wglMakeCurrent(hdc, hglrc))
        return false;

    //--------------------------------
    //	End initialization
    //--------------------------------

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // setup the camera
        // handle resolution problem
        // toggle different view mode
        if (isOrtho)
        {
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-2 * ar, 2 * ar, -2, 2, -2, 2);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            // glTranslatef(0, 0, z + 9);
        }
        else
        {
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, ar, 1, 2000);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(x, y, z);
        }
        display();

        if (rotate)
            speed += 1;
        else
            speed = speed;

        if (isLift)
        {
            if (bridgeDegree < 35)
            {
                bridgeDegree += 1;
            }
            if (bridgeLineUp < 0.26)
            {
                bridgeLineUp += 0.008;
            }
        }
        else
        {
            if (bridgeDegree > 0)
            {
                bridgeDegree -= 1;
            }
            if (bridgeLineUp > 0)
            {
                bridgeLineUp -= 0.008;
            }
        }

        SwapBuffers(hdc);
    }
    UnregisterClass(WINDOW_TITLE, wc.hInstance);
    return true;
}

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}