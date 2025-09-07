#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <GL/glut.h>

///glut 상수들은 glut.h의 232번줄부터 있다.

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

void pnk(unsigned char key, int x, int y) {//눌린 키, 키가 눌렸을 때의 마우스 좌표
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

