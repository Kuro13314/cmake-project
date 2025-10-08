#include "slikar.hpp"
#include "mbtw.hpp"
#include "slide.hpp"
#include "base.hpp"
#include <queue>
#include <vector>
#include <tuple>

using namespace std;

extern int menu,p[2],game,dir[4][2];
extern float scale,mx,my,z;
extern int width, height;

int watch;
/*
0 : main menu
1 : slikar
2 : mbtw
*/

void mtm(int x, int y){
    if(x<0||x>width||y<0||y>height) return;
    if(width>height) x-=(width-height)/2;
    mx=(float)x/height;
    my=(float)y/height;
}

void ctm(int button, int ud, int x, int y){//click the mouse, up or down, x, y
    if(button==GLUT_LEFT_BUTTON&&ud==GLUT_UP) return;
    switch(game){
    case 1:
        slikar::ctm(button,ud,x,y);
        return;
    case 2:
        mbtw::ctm(button,ud,x,y);
        return;
    case 3:
        slide::ctm(button,ud,x,y);
        return;
    }

    if(watch>0 && mx>=0.43125 && mx<=0.56875 && my>=0.8 && my<=0.84375){//game selected
        glLoadIdentity();
        gluLookAt(0.0,0.0,5.0,
                  0.0,0.0,0.0,
                  0.0,1.0,0.0);
        game=watch;
    }

    if(mx>=0.015625 && mx<=0.09 && my>=0.4625 && my<=0.5375) watch=(watch+1)%3+1;
    else if(mx>=0.9 && mx<=0.984375 && my>=0.4625 && my<=0.5375) watch=watch%3+1;
}

void renderscene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    switch(watch){
    case 0:
        glColor3f(1.0,1.0,1.0);
        sprintf(s,"GAME SELECTION");
        renderstring(-1.0,0.0,0.0,s);
        break;
    case 1:
        slikar::renderscene();
        glColor3f(1.0,0.0,0.0);
        sprintf(s,"SLIKAR");
        renderstring(-0.4,2.3,0.0,s);
        break;
    case 2:
        mbtw::renderscene();
        glColor3f(0.6,0.3,0.02);
        sprintf(s,"MBTW");
        renderstring(-0.4,2.3,0.0,s);
        break;
    case 3:
        slide::renderscene();
        glColor3f(0.4,0.0,1.0);
        sprintf(s,"SLIDE");
        renderstring(-0.4,2.3,0.0,s);
        break;
    }

    if(game){
        glPopMatrix();
        glutSwapBuffers();
        return;
    }

    glColor3f(1.0,1.0,1.0);
    sprintf(s,"GAME SELECT");
    renderstring(-0.8,3.0,1.0,s);
    glColor3f(0.0,0.0,0.0);
    sprintf(s,"SELECT");
    renderstring(-0.4,-2.5,1.0,s);

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f( 3.15, 0.2,1.0);
        glVertex3f( 3.5 , 0.0,1.0);
        glVertex3f( 3.15,-0.2,1.0);
        glVertex3f(-3.15,-0.2,1.0);
        glVertex3f(-3.5 , 0.0,1.0);
        glVertex3f(-3.15, 0.2,1.0);
    glEnd();
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);
        glVertex3f( 3.05, 0.3 ,1.0);
        glVertex3f( 3.6 , 0.3 ,1.0);
        glVertex3f( 3.6 ,-0.3 ,1.0);
        glVertex3f( 3.05,-0.3 ,1.0);

        glVertex3f(-3.05, 0.3 ,1.0);
        glVertex3f(-3.6 , 0.3 ,1.0);
        glVertex3f(-3.6 ,-0.3 ,1.0);
        glVertex3f(-3.05,-0.3 ,1.0);

        glColor3f(1.0,1.0,0.2);
        glVertex3f( 0.5,-2.25,1.0);
        glVertex3f(-0.5,-2.25,1.0);
        glVertex3f(-0.5,-2.6 ,1.0);
        glVertex3f( 0.5,-2.6 ,1.0);

    glEnd();

    glPopMatrix();

    glutSwapBuffers();
}

void pnk(unsigned char key, int x, int y) {//���� Ű, Ű�� ������ ���� ���콺 ��ǥ
    switch(game){
    case 1:
        slikar::pnk(key,x,y);
        return;
    case 2:
        mbtw::pnk(key,x,y);
        return;
    case 3:
        slide::pnk(key,x,y);
        return;
    }

    switch(key){
    case 27:
        exit(0);
        break;
    case 'a':
    case 'A':
        watch=(watch+1)%3+1;
        break;
    case 'd':
    case 'D':
        watch=watch%3+1;
        break;
    case 32:
        glLoadIdentity();
        gluLookAt(0.0,0.0,5.0,
                  0.0,0.0,0.0,
                  0.0,1.0,0.0);
        game=watch;
        break;
    }
}

int main(int argc, char **argv) {
    game=0;
    scale=0.4;
    watch=0;
    slikar::init(1);
    mbtw::init(1);
    slide::init(1);

    glutInit(&argc, argv);//�ʱ�ȭ

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//���� ����, RGB����, ���� ���� ��� (�ۼ� �� "or ����" ���)

    glutInitWindowPosition(300,100);//���� �� ���� ���� 100, ���� 100 ���������� â�� ���� ���� ��ġ
    glutInitWindowSize(640,640);//â�� ũ��(����, ����)
    glutCreateWindow("CMAKE GAMES by Kuro13314");//â �����(����)

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
