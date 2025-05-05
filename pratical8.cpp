#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

struct Point {
    float x, y;
};

// Rectangle corners
Point rect[4] = {
    {100, 100}, // Bottom-left
    {200, 100}, // Bottom-right
    {200, 200}, // Top-right
    {100, 200}  // Top-left
};

// DDA Line Drawing
void drawDDA(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps = max(abs(dx), abs(dy));
    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1, y = y1;
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(round(x), round(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}

// Draw rectangle by connecting 4 points
void drawRectangle() {
    for (int i = 0; i < 4; i++) {
        drawDDA(rect[i].x, rect[i].y, rect[(i + 1) % 4].x, rect[(i + 1) % 4].y);
    }
}

// Translate all 4 points
void translateRectangle(float tx, float ty) {
    for (int i = 0; i < 4; i++) {
        rect[i].x += tx;
        rect[i].y += ty;
    }
}

// Rotate around center
void rotateRectangle(float angle) {
      float ang = angle * 3.14 / 180.0;


    for (int j = 0; j < 4; j++) {
        rect[j].x = rect[j].x * cos(ang) - rect[j].y * sin(ang);
        rect[j].y = rect[j].x * sin(ang) + rect[j].y * cos(ang);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);
    drawRectangle();
    glFlush();
}

// Right-click menu
void menu(int choice) {
    switch (choice) {
        case 1: {
            float tx, ty;
            cout << "Enter translation (tx ty): ";
            cin >> tx >> ty;
            translateRectangle(tx, ty);
            break;
        }
        case 2: {
            float angle;
            cout << "Enter rotation angle (degrees): ";
            cin >> angle;
            rotateRectangle(angle);
            break;
        }
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glPointSize(2);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 800);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Rectangle - Translate & Rotate");

    init();
    glutDisplayFunc(display);

    glutCreateMenu(menu);
    glutAddMenuEntry("Translate", 1);
    glutAddMenuEntry("Rotate", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
