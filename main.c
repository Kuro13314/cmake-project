#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <GL/glut.h>

///glut ������� glut.h�� 232���ٺ��� �ִ�.

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

void pnk(unsigned char key, int x, int y) {//���� Ű, Ű�� ������ ���� ���콺 ��ǥ
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
    glutInit(&argc, argv);//�ʱ�ȭ

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//���� ����, RGB���� ���(�ۼ� �� "or ����" ���)

    glutInitWindowPosition(300,100);//���� �� ���� ���� 100, ���� 100 ���������� â�� ���� ���� ��ġ
    glutInitWindowSize(320,320);//â�� ũ��(����, ����)
    glutCreateWindow("GLUT Practice");//â �����(����)

    glutDisplayFunc(renderscene);//�������� �Լ��� ����
    glutIdleFunc(renderscene);//idle ������ �� �������� �Լ�

    glutKeyboardFunc(pnk);

    glutReshapeFunc(changesize);//â�� ũ�Ⱑ �ٲ���� ��, ��� �� ���ΰ��� ����

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();//�̺�Ʈ�� ���涧���� ����
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

