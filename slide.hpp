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

extern int height,width,game,dir[4][2];
extern float scale,mx,my;

namespace slide
{                                              //좌측
    vector<vector<int>> board={{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {1, 0, 0, 0, 0, 0, 1, 0, 3, 1},
                               {1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
                               {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
                               {1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
                               {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                               {0, 0, 0, 0, 1, 2, 1, 1, 1, 1},
                               {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
    int state=0,ms=10,menu;                    //우측
    vector<tuple<int,int,int>> ball={{1,8,3},{8,8,4}};

    void init(int f){///나중에 여기다가 맵 랜덤 생성 알고리즘 넣기
        state=0;
        for(int i=0;i<ms;i++)
            for(int j=0;j<ms;j++)
                if(board[i][j]>=3) board[i][j]=0;
        for(auto[x,y,c]:ball)
            board[x][y]=c;

        if(!f) return;//처음에만 실행 될 것들
    }

    void ctm(int button, int ud, int x, int y){//click the mouse, up or down, x, y
        if(!menu) return;
        if(mx>0.435 && mx<0.565 && my>0.34 && my<0.41) {// menu
            glLoadIdentity();
            gluLookAt(0.0,0.0,10.0,
                      0.0,0.0,0.0,
                      0.0,1.0,0.0);
            game=0;
            menu=0;
        }
        else if(mx>0.435 && mx<0.565 && my>0.465 && my<0.535) {// reset
            init(0);
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
            init(0);
            break;
        }
        if(state||menu) return;
        int p=0;
        switch(key){
        case 'w':
        case 'W':
            for(int i=0;i<ms;i++){
                p=ms-1;
                for(int j=ms-1;j>=0;j--){
                    if(board[i][j]==1)
                        p=j-1;
                    else if(board[i][j]==2)
                        p=j;
                    else if(board[i][j]==3){
                        board[i][j]=0;
                        if(board[i][p]!=2) board[i][p--]=3;
                        else state=1;
                    }
                    else if(board[i][j]==4){
                        board[i][j]=0;
                        if(board[i][p]!=2) board[i][p--]=4;
                        else state=-1;
                    }
                }
            }
            break;
        case 's':
        case 'S':
            for(int i=0;i<ms;i++){
                p=0;
                for(int j=0;j<ms;j++){
                    if(board[i][j]==1)
                        p=j+1;
                    else if(board[i][j]==2)
                        p=j;
                    else if(board[i][j]==3){
                        board[i][j]=0;
                        if(board[i][p]!=2) board[i][p++]=3;
                        else state=1;
                    }
                    else if(board[i][j]==4){
                        board[i][j]=0;
                        if(board[i][p]!=2) board[i][p++]=4;
                        else state=-1;
                    }
                }
            }
            break;
        case 'a':
        case 'A':
            for(int j=0;j<ms;j++){
                p=0;
                for(int i=0;i<ms;i++){
                    if(board[i][j]==1)
                        p=i+1;
                    else if(board[i][j]==2)
                        p=i;
                    else if(board[i][j]==3){
                        board[i][j]=0;
                        if(board[p][j]!=2) board[p++][j]=3;
                        else state=1;
                    }
                    else if(board[i][j]==4){
                        board[i][j]=0;
                        if(board[p][j]!=2) board[p++][j]=4;
                        else state=-1;
                    }
                }
            }
            break;
        case 'd':
        case 'D':
            for(int j=0;j<ms;j++){
                p=ms-1;
                for(int i=ms-1;i>=0;i--){
                    if(board[i][j]==1)
                        p=i-1;
                    else if(board[i][j]==2)
                        p=i;
                    else if(board[i][j]==3){
                        board[i][j]=0;
                        if(board[p][j]!=2) board[p--][j]=3;
                        else state=1;
                    }
                    else if(board[i][j]==4){
                        board[i][j]=0;
                        if(board[p][j]!=2) board[p--][j]=4;
                        else state=-1;
                    }
                }
            }
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
                if(board[i][j]==2) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.0, 0.0, 0.0);//goal
                else if(board[i][j]==3) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 1.0, 0.25, 0.2);//red ball
                else if(board[i][j]==4) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.2, 0.25, 1.0);//blue ball
                else if(board[i][j]==1) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.15, 0.2, 0.2);//wall
                else sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.9, 0.95, 1.0);//space
            }
        }
    }
}
/*
##########
#R#...##B#
#...#.##.#
#####.##.#
#......#.#
#.######.#
#.#....#.#
#.#.#.#..#
#...#.O#.#
##########

1 1 1 1 1 1 1 1 1 1
1 0 0 0 0 0 1 0 R 1
1 0 1 1 1 0 1 0 1 1
1 0 0 0 1 0 1 0 0 1
1 1 1 0 1 0 1 1 0 1
1 0 0 0 1 0 0 0 0 1
1 G 1 0 1 0 1 1 1 1
1 1 0 1 1 1 1 1 1 1
1 0 0 0 0 0 0 0 B 1
1 1 1 1 1 1 1 1 1 1

{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {1, 0, 0, 0, 0, 0, 1, 0, 3, 1},
                               {1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
                               {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
                               {1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
                               {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                               {1, 2, 0, 0, 1, 0, 1, 1, 1, 1},
                               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {1, 0, 0, 0, 0, 0, 0, 0, 4, 1},
                               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

*/
