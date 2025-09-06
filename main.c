#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <GL/glut.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void sq(float x1, float y1, float x2, float y2, float r, float g, float b);



int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",//style
                          "opengl project",//title
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          600,
                          600,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    /* program main loop */

    int turn=0,xd=1,yd=1;
    float r=1,g=0,b=0,
    t=1,
    rt=1.0f,
    x=0.0f,y=-0.5f,
    size=0.25f,
    speed=0.01f;
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//background color
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();
//            glRotatef(theta, 0.0f, 0.0f, 1.0f);//rotate
//            glTranslatef(0.1f, 0.1f,0.f);

            //background
            sq(-1.0f,0.6f,0.6f,0.4f,1.0f,0.0f,0.0f);
            sq(1.0f,0.0f,-0.6f,-0.2f,1.0f,0.0f,0.0f);
            sq(-1.0f,-0.6f,0.6f,-0.8f,1.0f,0.0f,0.0f);
            //player
            sq(-1.0f, 1.0f, -0.8f, 0.8f, 0.0f, 1.0f, 0.0f);

            glPopMatrix();

            SwapBuffers(hDC);

            theta += 1.0f*rt;

            printf("%f %f %f %d\n",r,g,b,turn);

            x+=speed*xd;
            if(x+size>=1.0f||x<=-1.0f){
                xd*=-1;
                turn=(turn+1)%6;
            }
            y+=speed*yd;
            if(y+size>=1.0f||y<=-1.0f){
                yd*=-1;
                turn=(turn+1)%6;
            }

            switch(turn){
            case 0:
                r=1;
                g=0;
                b=0;
                break;
            case 1:
                r=1;
                g=1;
                b=0;
                break;
            case 2:
                r=0;
                g=1;
                b=0;
                break;
            case 3:
                r=0;
                g=1;
                b=1;
                break;
            case 4:
                r=0;
                g=0;
                b=1;
                break;
            case 5:
                r=1;
                g=0;
                b=1;
                break;
            }

            Sleep(t);//delay t(ms)
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

void sq(float x1, float y1, float x2, float y2, float r, float g, float b){
    glBegin(GL_QUADS);

    glColor3f(r,g,b); glVertex2f(x1,y1);
    glColor3f(r,g,b); glVertex2f(x1,y2);
    glColor3f(r,g,b); glVertex2f(x2,y2);
    glColor3f(r,g,b); glVertex2f(x2,y1);

    glEnd();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

