#include "base.hpp"
#include "slikar.hpp"
#include <queue>
#include <vector>
#include <tuple>
///glut ������� glut.h�� 232���ٺ��� �ִ�.
using namespace std;

extern int width, height;

int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
float angle=0.0;
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
    slikar::pnk(key,x,y);
}

void ctm(int button, int ud, int x, int y){//pressed button, up or down, x,y
    slikar::ctm(button,ud,x,y);
}

void mtm(int x, int y){
    slikar::mtm(x,y);
}

void renderscene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    sprintf(s,"state : %d",state);
    glColor3f(0.0,0.0,0.0);
    renderstring(-1.5,-1.5,1.0,s);

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

