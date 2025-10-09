#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <cstdio>
#include <GL/glut.h>

///glut 상수들은 glut.h의 232번줄부터 있다.

int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};

int width, height;
int game;
float scale=0.4;
float mx,my;
char s[100];

void renderstring(float x, float y, float z, char* str){
    char *c;
    glRasterPos3f(x, y, z);
    for (c=str; *c != '\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void changesize(int w, int h) {
    if(h==0) h=1;
    float rt= 1.0* w / h;
    width=w;
    height=h;
    glutReshapeWindow( 640, 640);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

    gluPerspective(45,rt,1,1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,5.0+(5.0)*(!game),
              0.0,0.0,0.0,
              0.0,1.0,0.0);
}

void menuopen(){
    glColor3f(0.1f,0.1f,1.0f);
    glBegin(GL_QUADS);
        glVertex3f(-0.6,-0.6, 1.0);
        glVertex3f( 0.6,-0.6, 1.0);
        glVertex3f( 0.6, 0.6, 1.0);
        glVertex3f(-0.6, 0.6, 1.0);
    glEnd();
    glColor3f(0.0f,0.0f,0.0f);
    glLineWidth(5.0f);

    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.2, 0.5, 1.1);
        glVertex3f( 0.2, 0.5, 1.1);
        glVertex3f( 0.2, 0.3, 1.1);
        glVertex3f(-0.2, 0.3, 1.1);
    glEnd();
    sprintf(s,"menu");
    renderstring(-0.11,0.375,1.1,s);

    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.2, 0.1, 1.1);
        glVertex3f( 0.2, 0.1, 1.1);
        glVertex3f( 0.2,-0.1, 1.1);
        glVertex3f(-0.2,-0.1, 1.1);
    glEnd();
    sprintf(s,"reset");
    renderstring(-0.11,-0.025,1.1,s);

    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.2,-0.3, 1.1);
        glVertex3f( 0.2,-0.3, 1.1);
        glVertex3f( 0.2,-0.5, 1.1);
        glVertex3f(-0.2,-0.5, 1.1);
    glEnd();
    sprintf(s,"exit");
    renderstring(-0.08,-0.425,1.1,s);
}

void sq(float x, float y, float scale, float red, float green, float blue){
    glColor3f(red,green,blue);
    glBegin(GL_QUADS);
        glVertex3f(x, y, 0.0);
        glVertex3f(x+scale, y, 0.0);
        glVertex3f(x+scale, y+scale, 0.0);
        glVertex3f(x, y+scale, 0.0);
    glEnd();
}
