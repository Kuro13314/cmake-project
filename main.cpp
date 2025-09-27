#include "base.hpp"
#include <queue>
#include <vector>
#include <tuple>
///glut ������� glut.h�� 232���ٺ��� �ִ�.
using namespace std;

extern int width, height;
extern int asdf;

int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
float angle=0.0,z=0.0;
float r=0.0,g=0.0,b=0.0;                 //����
vector<vector<int>> board={{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                           { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                           {-1,-1,-1,-1, 0, 0,-1, 0,-1, 0},
                           { 0, 0, 0,-1, 0, 0,-1, 0,-1, 0},//���
                           { 0,-1, 0,-1, 0, 0,-1, 1,-1, 0},
                           { 0,-1, 0,-1, 0, 0, 0,-1, 0, 0},
                           { 0,-1,-2,-1, 0, 0, 0, 0, 0, 0},
                           { 0,-1,-1,-1, 0, 0, 0, 0, 0, 0},
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int p[2]={0,9};                          //������
float scale=0.4;
int ms=10;
int turn=1;
int menu=0;
float mx,my;
int state=0;
char s[100];
/*
state�� ���� ����
-1: stage failed
0 : normal
1 : stage clear
*/

queue<tuple<int,int,int>> go;

void pnk(unsigned char key, int x, int y) {//���� Ű, Ű�� ������ ���� ���콺 ��ǥ
    switch(key){//������ ������ �� �� �ִ� �ൿ��
    case 27:
        menu=(menu+1)%2;
        break;
    case 'r':
    case 'R':
        turn=1;
        p[0]=0;
        p[1]=9;
        state=0;
        break;
    case 'q':
    case 'Q':
        z+=0.01;
        break;
    case 'e':
    case 'E':
        z-=0.01;
        break;
    }
    if(state) return;
    switch(key){//������ ������ �� �� ���� �ൿ��
    case 'w':
    case 'W':
        p[1]++;
        if(p[1]>=ms) p[1]=ms-1;
        if(board[p[0]][p[1]]==-1) p[1]--;
        turn++;
        break;
    case 'a':
    case 'A':
        p[0]--;
        if(p[0]<0) p[0]=0;
        if(board[p[0]][p[1]]==-1) p[0]++;
        turn++;
        break;
    case 's':
    case 'S':
        p[1]--;
        if(p[1]<0) p[1]=0;
        if(board[p[0]][p[1]]==-1) p[1]++;
        turn++;
        break;
    case 'd':
    case 'D':
        p[0]++;
        if(p[0]>=ms) p[0]=ms-1;
        if(board[p[0]][p[1]]==-1) p[0]--;
        turn++;
        break;
    }
}

void ctm(int button, int ud, int x, int y){//pressed button, up or down, x,y
    if(!(menu&&button==GLUT_LEFT_BUTTON&&ud==GLUT_DOWN)) return;
    if(mx>0.435 && mx<0.565 && my>0.465 && my<0.535) {// reset
        turn=1;
        p[0]=0;
        p[1]=9;
        state=0;
    }
    else if(mx>0.435 && mx<0.565 && my>0.59 && my<0.66) {// exit
        exit(0);
    }
    else if((mx<0.318 || mx>0.68) || (my<0.317 || my>0.68)) menu=0;

}

void mtm(int x, int y){
    if(x<0||x>width||y<0||y>height) return;
    if(width>height) x-=(width-height)/2;
    mx=(float)x/height;
    my=(float)y/height;
}

void renderscene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    sprintf(s,"state : %d",state);
    glColor3f(0.0,0.0,0.0);
    renderstring(-1.5,-1.5,1.0,s);

    if(asdf){
        sprintf(s,"MENU!");
        renderstring(-0.5,0.0,1.0,s);
    }

    if(state==1){//����
        glColor3f(1.0,1.0,0.0);
        sprintf(s,"clear!");
        renderstring(-0.3,0.0,0.1,s);
    }
    if(state==-1){//����
        glColor3f(1.0,0.5,0.5);
        sprintf(s,"failed!");
        renderstring(-0.3,0.0,0.1,s);
    }
    if(menu) menuopen();

    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(board[p[0]][p[1]]==-2&&state==0) state=1;
            else if(board[p[0]][p[1]]>0&&board[p[0]][p[1]]<=turn&&state==0) state=-1;
            if(board[i][j]==-2) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 1.0, 1.0, 0.0);//goal
            else if(board[i][j]==-1) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.5,0.5,0.5);//wall
            else if(board[i][j]>0&&board[i][j]<=turn) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 1.0, 0.0 ,0.0);//fire
            else if(i==p[0] && j==p[1]) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.0, 1.0, 0.0);//player
            else sq(-2.0+(i*scale),-2.0+(j*scale), scale, 1.0, 1.0, 1.0);//space
        }
    }

    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char **argv) {

    for(int i=0;i<ms;i++)
        for(int j=0;j<ms;j++)
        if(board[i][j]==1) go.push({i,j,1});

    while(!go.empty()){
        auto[x,y,d]=go.front();
        go.pop();
        if(d>1&&board[x][y]!=0) continue;
        board[x][y]=d;

        for(auto[dx,dy]:dir){
            if(dx+x>=ms||dx+x<0) continue;
            if(dy+y>=ms||dy+y<0) continue;

            go.push({dx+x,dy+y,d+1});
        }
    }


    glutInit(&argc, argv);//�ʱ�ȭ

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//���� ����, RGB����, ���� ���� ��� (�ۼ� �� "or ����" ���)

    glutInitWindowPosition(300,100);//���� �� ���� ���� 100, ���� 100 ���������� â�� ���� ���� ��ġ
    glutInitWindowSize(640,640);//â�� ũ��(����, ����)
    glutCreateWindow("SLIKAR");//â �����(����)

    glutDisplayFunc(renderscene);//�������� �Լ��� ����
    glutIdleFunc(renderscene);//idle ������ �� �������� �Լ�

    glutKeyboardFunc(pnk);
    glutPassiveMotionFunc(mtm);
    glutMouseFunc(ctm);

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

