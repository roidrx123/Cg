#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

int x1_click, y1_click, x2_click, y2_click;
bool firstClick = true;
int lineType = 1; // 1: Simple, 2: Dashed, 3: Dotted, 4: Dash-Dot

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawAxis() {
    glColor3f(0.0, 1.0, 0.0); // Green axis
    for (int i = -500; i <= 500; i++) {
        drawPixel(i, 0); // X-axis
        drawPixel(0, i); // Y-axis
    }
}

// Bresenham's Line Algorithm with style
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int p = 2 * dy - dx;

    int x, y;
    int inc1 = 2 * dy;
    int inc2 = 2 * (dy - dx);

    int step = 0;

    int sx = x2 > x1 ? 1 : -1;
    int sy = y2 > y1 ? 1 : -1;

    x = x1;
    y = y1;

    for (int i = 0; i <= dx; i++) {
        // Based on lineType
        switch (lineType) {
            case 1: drawPixel(x, y); break; // Simple
            case 2: if (i % 10 < 6) drawPixel(x, y); break; // Dashed
            case 3: if (i % 6 == 0) drawPixel(x, y); break; // Dotted
            case 4: if (i % 12 < 6 || i % 12 == 8) drawPixel(x, y); break; // Dash-dot
        }

        if (p < 0) {
            p += inc1;
        } else {
            y += sy;
            p += inc2;
        }
        x += sx;
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int mouseX = x - 250;
        int mouseY = 250 - y;

        if (firstClick) {
            x1_click = mouseX;
            y1_click = mouseY;
            firstClick = false;
        } else {
            x2_click = mouseX;
            y2_click = mouseY;
            firstClick = true;
            glutPostRedisplay();
        }
    }
}

void menu(int option) {
    lineType = option;
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxis();

    glColor3f(0.0, 0.0, 0.0); // Black lines
    if (firstClick == true) { // Only draw if 2 clicks are complete
        drawLine(x1_click, y1_click, x2_click, y2_click);
    }

    glFlush();
}

void init() {
    gluOrtho2D(-250, 250, -250, 250); // Origin at center
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glPointSize(2.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Bresenham Line Drawing with Styles");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);

    // Create Right-Click Menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Simple Line", 1);
    glutAddMenuEntry("Dashed Line", 2);
    glutAddMenuEntry("Dotted Line", 3);
    glutAddMenuEntry("Dash-Dotted Line", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
