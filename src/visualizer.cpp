#include "visualizer.hpp"
#include "device.hpp"

void* gl_main(void* arg) {
    int argc = 0;
    glutInit(&argc, NULL);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(500 , 500);
    glutCreateWindow("visualizer");
    glutDisplayFunc(display);
    glutIdleFunc(redisp);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutMainLoop();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3d(1.0,0.0,0.0);
    for (int i = 0; i < points.size(); i++) {
        glVertex2f(points[i][0], points[i][1]);
    }
    glEnd();
    glFlush();
}

void redisp() {
    glutPostRedisplay();
}