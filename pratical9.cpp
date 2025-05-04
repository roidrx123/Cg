#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

struct Point {
    float x, y;
};

// Original rectangle corners (unchanged reference)
Point originalRect[4] = {
    {100, 100}, // Bottom-left
    {200, 100}, // Bottom-right
    {200, 200}, // Top-right
    {100, 200}  // Top-left
};

// Rectangle to be displayed (modifiable)
Point rect[4];

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

// Draw X and Y axes
void drawAxes() {
    glColor3f(0.5, 0.5, 0.5); // Gray color
    drawDDA(0, 250, 500, 250); // X-axis
    drawDDA(250, 0, 250, 500); // Y-axis
}

// Draw rectangle using DDA lines
void drawRectangle() {
    for (int i = 0; i < 4; i++) {
        drawDDA(rect[i].x, rect[i].y, rect[(i + 1) % 4].x, rect[(i + 1) % 4].y);
    }
}

// Translate rectangle
void translateRectangle(float tx, float ty) {
    for (int i = 0; i < 4; i++) {
        rect[i].x += tx;
        rect[i].y += ty;
    }
}

// Reflect original about X-axis
void reflectX() {
    for (int i = 0; i < 4; i++) {
        rect[i].x = originalRect[i].x;
        rect[i].y = 500 - originalRect[i].y;
    }
}

// Reflect original about Y-axis
void reflectY() {
    for (int i = 0; i < 4; i++) {
        rect[i].x = 500 - originalRect[i].x;
        rect[i].y = originalRect[i].y;
    }
}

// Reset to original rectangle
void resetRectangle() {
    for (int i = 0; i < 4; i++) {
        rect[i] = originalRect[i];
    }
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();           // Draw coordinate axes
    glColor3f(0, 0, 1);    // Blue rectangle
    drawRectangle();      // Draw rectangle
    glFlush();
}

// Menu
void menu(int choice) {
    switch (choice) {
        case 1: {
            float tx, ty;
            cout << "Enter translation (tx ty): ";
            cin >> tx >> ty;
            translateRectangle(tx, ty);
            break;
        }
        case 2:
            reflectX(); // Reflect original over X-axis
            break;
        case 3:
            reflectY(); // Reflect original over Y-axis
            break;
        case 4:
            resetRectangle(); // Restore to original
            break;
    }
    glutPostRedisplay();
}

// Initialize
void init() {
    resetRectangle(); // Start with original
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glPointSize(2);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500); // Coordinate system
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Rectangle: Translate & Reflect");

    init();
    glutDisplayFunc(display);

    glutCreateMenu(menu);
    glutAddMenuEntry("Translate", 1);
    glutAddMenuEntry("Reflect about X-axis", 2);
    glutAddMenuEntry("Reflect about Y-axis", 3);
    glutAddMenuEntry("Reset to Original", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
