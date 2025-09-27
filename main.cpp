#include "base.hpp"
#include "slikar.hpp"
#include <queue>
#include <vector>
#include <tuple>
///glut 상수들은 glut.h의 232번줄부터 있다.
using namespace std;

extern int width, height;

int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
float angle=0.0;
float r=0.0,g=0.0,b=0.0;                 //왼쪽
vector<vector<int>> board={{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                           { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                           {-1,-1,-1,-1, 0, 0,-1, 0,-1, 0},
                           { 0, 0, 0,-1, 0, 0,-1, 0,-1, 0},//상단
                           { 0,-1, 0,-1, 0, 0,-1, 1,-1, 0},
                           { 0,-1, 0,-1, 0, 0, 0,-1, 0, 0},
                           { 0,-1,-2,-1, 0, 0, 0, 0, 0, 0},
                           { 0,-1,-1,-1, 0, 0, 0, 0, 0, 0},
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int p[2]={0,9};                          //오른쪽
float scale=0.4;
int ms=10;
int turn=1;
int menu=0;
float mx,my;
int state=0;
char s[100];
/*
state에 따른 상태
-1: stage failed
0 : normal
1 : stage clear
*/

queue<tuple<int,int,int>> go;

void pnk(unsigned char key, int x, int y) {//눌린 키, 키가 눌렸을 때의 마우스 좌표
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

    if(state==1){//성공
        glColor3f(1.0,1.0,0.0);
        sprintf(s,"clear!");
        renderstring(-0.3,0.0,0.1,s);
    }
    if(state==-1){//실패
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


    glutInit(&argc, argv);//초기화

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//이중 버퍼, RGB색상, 깊이 버퍼 사용 (작성 시 "or 연산" 사용)

    glutInitWindowPosition(300,100);//왼쪽 위 기준 가로 100, 세로 100 떨어진곳에 창의 온쪽 위가 위치
    glutInitWindowSize(640,640);//창의 크기(가로, 세로)
    glutCreateWindow("SLIKAR");//창 만들기(제목)

    glutDisplayFunc(renderscene);//렌더링할 함수를 설정
    glutIdleFunc(renderscene);//idle 상태일 때 렌더링할 함수

    glutKeyboardFunc(pnk);
    glutPassiveMotionFunc(mtm);
    glutMouseFunc(ctm);

    glutReshapeFunc(changesize);//창의 크기가 바뀌었을 때, 어떻게 할 것인가를 설정

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();//이벤트가 생길때까지 루프

    return 0;
}

