#pragma once

#include "base.hpp"
#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <GL/glut.h>
#include <vector>
#define t(x) ((x)-'a')

using namespace std;

extern int height,width,game,dir[4][2];
extern float scale,mx,my;

namespace sditc
{                                               //좌측
    vector<vector<char>> board={{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ','#','#','#','#','#','#','.','#',' '},
                                {' ','#','$','.','.','.','#','.','#',' '},
                                {' ','#','#','#','#','B','#','.','#',' '},
                                {' ','#','a','#','.','.','.','.','#',' '},
                                {' ','#','.','#','.','.','#','#','#',' '},
                                {' ','#','.','#','.','.','#','b','#',' '},
                                {' ','#','.','.','.','.','#','.','#',' '},
                                {' ','#','#','#','#','#','#','A','#',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};
    int menu,state=0,ms=10;              //우측
    int p[2];
    int key[27];
    int pocket[27],d;
    int left=1;

    void init(int f){///나중에 여기다가 맵 랜덤 생성 알고리즘 넣기
        p[0]=0;
        p[1]=0;
        memset(key,0,sizeof(key));
        left=1;

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

    void pnk(unsigned char k, int x, int y) {//눌린 키, 키가 눌렸을 때의 마우스 좌표
        switch(k){//게임이 끝나도 할 수 있는 행동들
        case 27:
            menu=(menu+1)%2;
            break;
        case 'r':
        case 'R':
            init(0);
            break;
        }
        if(state||menu) return;
        switch(k){
        case 'w':
        case 'W':
            p[1]++;
            if(p[1]>=ms || board[p[0]][p[1]]=='#') p[1]--;
            if(board[p[0]][p[1]]>='A' && board[p[0]][p[1]]<='Z'){
                if(key[board[p[0]][p[1]]-'A']){
                    board[p[0]][p[1]]='.';
                }
                else p[1]--;
            }
            break;
        case 'a':
        case 'A':
            p[0]--;
            if(p[0]<0 || board[p[0]][p[1]]=='#') p[0]++;
            if(board[p[0]][p[1]]>='A' && board[p[0]][p[1]]<='Z'){
                if(key[board[p[0]][p[1]]-'A']){
                    board[p[0]][p[1]]='.';
                }
                else p[0]++;
            }
            break;
        case 's':
        case 'S':
            p[1]--;
            if(p[1]<0 || board[p[0]][p[1]]=='#') p[1]++;
            if(board[p[0]][p[1]]>='A' && board[p[0]][p[1]]<='Z'){
                if(key[board[p[0]][p[1]]-'A']){
                    board[p[0]][p[1]]='.';
                }
                else p[1]++;
            }
            break;
        case 'd':
        case 'D':
            p[0]++;
            if(p[0]>=ms || board[p[0]][p[1]]=='#') p[0]--;
            if(board[p[0]][p[1]]>='A' && board[p[0]][p[1]]<='Z'){
                if(key[board[p[0]][p[1]]-'A']){
                    board[p[0]][p[1]]='.';
                }
                else p[0]--;
            }
            break;
        }
        if(board[p[0]][p[1]]>='a'){
            key[board[p[0]][p[1]]-'a']++;
            board[p[0]][p[1]]='.';
        }
        else if(board[p[0]][p[1]]=='$'){
            left--;
            board[p[0]][p[1]]='.';
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
                if(i==p[0] && j==p[1]) sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.0, 0.4, 0.0);//player
                else if(board[i][j]==' ') sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.05, 0.05, 0.05);//space(outside)
                else if(board[i][j]=='$') sq(-2.0+(i*scale),-2.0+(j*scale), scale, 1.0, 0.8, 0.0);//document
                else if(board[i][j]=='#') sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.1, 0.1, 0.1);//wall
                else if(board[i][j]=='.') sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.3, 0.3, 0.3);//space(inside)
                else if(board[i][j]<='Z'){//door
                    glColor3f(0.0,0.0,0.0);
                    sprintf(s,"%c",board[i][j]);
                    renderstring(-1.98+(i*scale),-1.98+(j*scale),0.0001,s);
                    sq(-2.0+(i*scale),-2.0+(j*scale), scale, 0.2, 0.25, 1.0);
                }
                else if(board[i][j]<='z'){//key
                    glColor3f(0.0,0.0,0.0);
                    sprintf(s,"%c",board[i][j]);
                    renderstring(-1.98+(i*scale),-1.98+(j*scale),0.0001,s);
                    sq(-2.0+(i*scale),-2.0+(j*scale), scale, 1.0, 0.25, 0.2);
                }
                else sq(-2.0+(i*scale),-2.0+(j*scale), scale, 1.0, 1.0, 1.0);//???
            }
        }
    }
}

//int judge(char c){
//    if(c=='.') return 0;
//    if(c=='*') return 1;
//    if(c=='$') return 2;
//    if(c<='Z') return 3;
//    if(c<='z') return 4;
//    return 0;
//}
