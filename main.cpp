#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <GL/glut.h>
#include <queue>
#include <vector>
#include <tuple>
///glut 상수들은 glut.h의 232번줄부터 있다.
using namespace std;

int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
float angle=0.0;
float r=0.0,g=0.0,b=0.0;                //왼쪽
vector<vector<int>> state={{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                           { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                           {-1,-1,-1,-1, 0, 0,-1, 0,-1, 0},
                    /*하단*/{ 0, 0, 0, 0,-1, 0,-1, 0,-1, 0},//상단
                           { 0, 0,-1, 0,-1, 0,-1, 1,-1, 0},
                           { 0, 0,-1, 0,-1, 0, 0,-1, 0, 0},
                           { 0, 0,-1,-2,-1, 0, 0, 0, 0, 0},
                           { 0, 0, 0,-1, 0, 0, 0, 0, 0, 0},
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int p[2]={0,9};                          //오른쪽
float scale=0.4;
int ms=10;
int turn=1;
int cl=0;

queue<tuple<int,int,int>> go;

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
    else if(color==4) glColor3f(0.5, 0.5, 0.5);//wall
    else glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(x, y, 0.0);
        glVertex3f(x+scale, y, 0.0);
        glVertex3f(x+scale, y+scale, 0.0);
        glVertex3f(x, y+scale, 0.0);
    glEnd();
}

void idle(void){
    glColor3f(0.0,1.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(-1.0, -1.0, 0.0);
    glVertex3f( 1.0, -1.0, 0.0);
    glVertex3f( 1.0,  1.0, 0.0);
    glVertex3f(-1.0,  1.0, 0.0);
    glEnd();
}

void renderscene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glColor3f(1.0,1.0,1.0);

    if(cl==1) stageclear();
    if(cl==-1) stagefail();

    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(state[p[0]][p[1]]==-2) cl=1;
            else if(state[p[0]][p[1]]>0&&state[p[0]][p[1]]<=turn) cl=-1;
            if(state[i][j]==-2) sq(-2.0+(i*scale),-2.0+(j*scale), 2);//goal
            else if(state[i][j]==-1) sq(-2.0+(i*scale),-2.0+(j*scale), 4);//wall
            else if(state[i][j]>0&&state[i][j]<=turn) sq(-2.0+(i*scale),-2.0+(j*scale), 1);//fire
            else if(i==p[0] && j==p[1]) sq(-2.0+(i*scale),-2.0+(j*scale), 3);//player
            else sq(-2.0+(i*scale),-2.0+(j*scale), 0);//space
        }
    }

    glPopMatrix();

    glutSwapBuffers();
}

void pnk(unsigned char key, int x, int y) {//눌린 키, 키가 눌렸을 때의 마우스 좌표
    switch(key){//게임이 끝나도 할 수 있는 행동들
    case 27:
        exit(0);
        break;
    case 'r':
    case 'R':
        turn=1;
        p[0]=0;
        p[1]=9;
        cl=0;
        break;
    }
    if(cl) return;
    switch(key){//게임이 끝나면 할 수 없는 행동들
    case 'w':
    case 'W':
        p[1]++;
        if(p[1]>=ms) p[1]=ms-1;
        if(state[p[0]][p[1]]==-1) p[1]--;
        turn++;
        break;
    case 'a':
    case 'A':
        p[0]--;
        if(p[0]<0) p[0]=0;
        if(state[p[0]][p[1]]==-1) p[0]++;
        turn++;
        break;
    case 's':
    case 'S':
        p[1]--;
        if(p[1]<0) p[1]=0;
        if(state[p[0]][p[1]]==-1) p[1]++;
        turn++;
        break;
    case 'd':
    case 'D':
        p[0]++;
        if(p[0]>=ms) p[0]=ms-1;
        if(state[p[0]][p[1]]==-1) p[0]--;
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

    for(int i=0;i<ms;i++)
        for(int j=0;j<ms;j++)
        if(state[i][j]==1) go.push({i,j,1});

    while(!go.empty()){
        auto[x,y,d]=go.front();
        go.pop();
        if(d>1&&state[x][y]!=0) continue;
        state[x][y]=d;

        for(auto[dx,dy]:dir){
            if(dx+x>=ms||dx+x<0) continue;
            if(dy+y>=ms||dy+y<0) continue;

            go.push({dx+x,dy+y,d+1});
        }
    }


    glutInit(&argc, argv);//초기화

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//이중 버퍼, RGB색상, 깊이 버퍼 사용 (작성 시 "or 연산" 사용)

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

