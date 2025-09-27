#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <GL/glut.h>
#include <vector>
#include <queue>'

using namespace std;

namespace slikar
{
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
        switch(key){//게임이 끝나도 할 수 있는 행동들
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
        if(state) return;
        switch(key){//게임이 끝나면 할 수 없는 행동들
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
}
