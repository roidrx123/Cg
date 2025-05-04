#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

// Global rectangle corners
float rectX1 = 100, rectY1 = 100;
float rectX2 = 200, rectY2 = 200;

// DDA Line Drawing Algorithm
void drawDDA(float xStart, float yStart, float xEnd, float yEnd) {
    float dx = xEnd - xStart;
    float dy = yEnd - yStart;

    int steps = max(abs(dx),abs(dy));
    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = xStart;
    float y = yStart;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(round(x), round(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}

// Draw rectangle using DDA
void drawRectangle() {
    drawDDA(rectX1, rectY1, rectX2, rectY1); // Bottom
    drawDDA(rectX2, rectY1, rectX2, rectY2); // Right
    drawDDA(rectX2, rectY2, rectX1, rectY2); // Top
    drawDDA(rectX1, rectY2, rectX1, rectY1); // Left
}

// Translation
void translateRectangle(float tx, float ty) {
    rectX1 += tx;
    rectY1 += ty;
    rectX2 += tx;
    rectY2 += ty;
}

// Scaling
void scaleRectangle(float sx, float sy) {
    rectX1 *= sx;
    rectY1 *= sy;
    rectX2 *= sx;
    rectY2 *= sy;
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1); // Blue color
    drawRectangle();
    glFlush();
}

// Menu function
void menu(int choice) {
    switch (choice) {
        case 1: {
            float tx, ty;
            cout << "Enter translation values (tx ty): ";
            cin >> tx >> ty;
            translateRectangle(tx, ty);
            break;
        }
        case 2: {
            float sx, sy;
            cout << "Enter scaling factors (sx sy): ";
            cin >> sx >> sy;
            scaleRectangle(sx, sy);
            break;
        }
    }
    glutPostRedisplay();
}

// Initialization
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glColor3f(0.0, 0.0, 1.0); // Blue color
    glPointSize(2);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500); // Set the coordinate system
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D Rectangle: Translate & Scale");

    init();

    glutDisplayFunc(display);

    // Create menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Translate", 1);
    glutAddMenuEntry("Scale", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
