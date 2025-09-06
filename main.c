#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <GL/glut.h>

///glut 상수들은 glut.h의 232번줄부터 있다.

float angle=0.0;
float r=0.0,g=0.0,b=0.0;

void renderscene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glRotatef(angle,0.0,1.0,0.0);

    glColor3f(1.0,1.0,1.0);

    glBegin(GL_QUADS);
        glVertex3f( 2.0, 2.0,0.0);
        glVertex3f( 2.0,-2.0,0.0);
        glVertex3f(-2.0,-2.0,0.0);
        glVertex3f(-2.0, 2.0,0.0);
    glEnd();

    glPopMatrix();

    glutSwapBuffers();

//    angle+=0.1;
}

void changesize(int w, int h) {
    if(h==0) h=1;
    float ratio= 1.0* w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

    gluPerspective(45,ratio,1,1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,5.0,
              0.0,0.0,0.0,
              0.0,1.0,0.0);
}

void pnk(unsigned char key, int x, int y) {//눌린 키, 키가 눌렸을 때의 마우스 좌표
    if(key==27)//ESC
        exit(0);
    if(key=='1')
        if(r==0.0) r=1.0;
        else r=0.0;
    if(key=='2')
        if(b==0.0) b=1.0;
        else b=0.0;
    if(key=='3')
        if(g==0.0) g=1.0;
        else g=0.0;

}

void main(int argc, char **argv) {
    glutInit(&argc, argv);//초기화

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//이중 버퍼, RGB색상 사용(작성 시 "or 연산" 사용)

    glutInitWindowPosition(300,100);//왼쪽 위 기준 가로 100, 세로 100 떨어진곳에 창의 온쪽 위가 위치
    glutInitWindowSize(320,320);//창의 크기(가로, 세로)
    glutCreateWindow("GLUT Practice");//창 만들기(제목)

    glutDisplayFunc(renderscene);//렌더링할 함수를 설정
    glutIdleFunc(renderscene);//idle 상태일 때 렌더링할 함수

    glutKeyboardFunc(pnk);

    glutReshapeFunc(changesize);//창의 크기가 바뀌었을 때, 어떻게 할 것인가를 설정

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();//이벤트가 생길때까지 루프
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC) {
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

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

