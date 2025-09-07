#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <GL/glut.h>

///glut ������� glut.h�� 232���ٺ��� �ִ�.

float angle=0.0;
float r=0.0,g=0.0,b=0.0;
int state[5][5]={{-1,8,7,6,5},//2 : goal
                 {8,7,6,5,4},//1 : fire
                 {7,6,5,4,3},//0 : space
                 {6,5,4,3,2},
                 {5,4,3,2,1}};
int player[2]={2,2};
float size=0.8;
int turn=1;
int cl=0;

void stageclear(){
    glColor3f(1.0,1.0,0.0);
    glRasterPos3f(-0.3,0.0,0.1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'c');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'l');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'e');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'a');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'r');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'!');
}

void stagefail(){
    glColor3f(1.0,0.5,0.5);
    glRasterPos3f(-0.3,0.0,0.1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'f');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'a');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'i');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'l');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'e');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'d');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'!');
}

void sq(float x, float y, int color){
    if(color==0) glColor3f(1.0, 1.0, 1.0);//space
    else if(color==1) glColor3f(1.0, 0.0, 0.0);//fire
    else if(color==2) glColor3f(1.0, 1.0, 0.0);//goal
    else if(color==3) glColor3f(0.0, 1.0, 0.0);//player
    else glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(x, y, 0.0);
        glVertex3f(x+size, y, 0.0);
        glVertex3f(x+size, y+size, 0.0);
        glVertex3f(x, y+size, 0.0);
    glEnd();
}

void renderscene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glColor3f(1.0,1.0,1.0);

    if(cl==1) stageclear();
    if(cl==-1) stagefail();

    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            if(state[player[0]][player[1]]==-1) cl=1;
            else if(state[player[0]][player[1]]<=turn) cl=-1;
            if(state[i][j]==-1) sq(-2.0+(i*size),-2.0+(j*size), 2);//goal
            else if(state[i][j]<=turn) sq(-2.0+(i*size),-2.0+(j*size), 1);//fire
            else if(i==player[0] && j==player[1]) sq(-2.0+(i*size),-2.0+(j*size), 3);//player
            else sq(-2.0+(i*size),-2.0+(j*size), 0);//space
        }
    }

    glPopMatrix();

    glutSwapBuffers();
}

void pnk(unsigned char key, int x, int y) {//���� Ű, Ű�� ������ ���� ���콺 ��ǥ
    if(cl) return;
    switch(key){
    case 27://ESC
        exit(0);
        break;
    case 'w':
        player[1]++;
        if(player[1]>=5) player[1]=4;
        turn++;
        break;
    case 'a':
        player[0]--;
        if(player[0]<0) player[0]=0;
        turn++;
        break;
    case 's':
        player[1]--;
        if(player[1]<0) player[1]=0;
        turn++;
        break;
    case 'd':
        player[0]++;
        if(player[0]>=5) player[0]=4;
        turn++;
        break;
    }
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

int main(int argc, char **argv) {
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

    return 0;
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

