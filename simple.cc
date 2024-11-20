#include <GL/glut.h>
#include<iostream>
using namespace std;
void display() {
    glClearColor(1,1 , 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
   
    glBegin(GL_TRIANGLES);//pink
    glColor4f(1, 0, 1, 0);
    glVertex2f(40, 40);
    glVertex2f(80, 40);
    glVertex2f(60, 60);
    
    glEnd();

    glTranslatef(0, 120, 0);
    glScalef(1, -1, 1);
    
    glBegin(GL_TRIANGLES);//pink
    
    glColor4f(1, 1, 0, 0);
    glVertex2f(40, 40);
    glVertex2f(80, 40);
    glVertex2f(60, 60);

    glEnd();
    
  
    glFlush();

}
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Line");
    gluOrtho2D(0, 200, 0, 200);
    glutDisplayFunc(display);
    glutMainLoop();



    return 0;
}
