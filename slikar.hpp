#pragma once

#include "base.hpp"
#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <GL/glut.h>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

extern int menu,ms,height,width,p[2],game,dir[4][2];
extern float scale,mx,my;

namespace slikar
{                                            //왼쪽
    vector<vector<int>> board={{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                               { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                               { 0, 0, 0, 0, 0, 0,-1, 0,-1, 0},
                               {-1,-1,-1,-1, 0, 0,-1, 0,-1, 0},
                        /*하단*/{ 0, 0, 0,-1, 0, 0,-1, 0,-1, 0},//상단
                               { 0,-1, 0,-1, 0, 0,-1, 1,-1, 0},
                               { 0,-1, 0,-1, 0, 0, 0,-1, 0, 0},
                               { 0,-1,-2,-1, 0, 0, 0, 0, 0, 0},
                               { 0,-1,-1,-1, 0, 0, 0, 0, 0, 0},
                               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
                                             //오른쪽

    queue<tuple<int,int,int>> go;
    int state=0,turn=1,p[2]={0,9};

    void init(int f){///나중에 여기다가 맵 랜덤 생성 알고리즘 넣기
        p[0]=0;p[1]=9;
        turn=1;
        for(int i=0;i<ms;i++)
            for(int j=0;j<ms;j++)
                if(board[i][j]==1) go.push(make_tuple(i,j,1));

        if(f) return;
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
    }

    void ctm(int button, int ud, int x, int y){//click the mouse, up or down, x, y
        if(!menu) return;
        if(mx>0.435 && mx<0.565 && my>0.34 && my<0.41) {// menu
            z=10.0;
            glLoadIdentity();
            gluLookAt(0.0,0.0,10.0,
                      0.0,0.0,0.0,
                      0.0,1.0,0.0);
            game=0;
            menu=0;
        }
        else if(mx>0.435 && mx<0.565 && my>0.465 && my<0.535) {// reset
            init(1);
        }
        else if(mx>0.435 && mx<0.565 && my>0.59 && my<0.66) {// exit
            exit(0);
        }
        else if((mx<0.318 || mx>0.68) || (my<0.317 || my>0.68)) menu=0;
    }

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
        }
        if(state||menu) return;
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

    void renderscene(void){

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
    }
}
