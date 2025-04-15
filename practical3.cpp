#include <GL/glut.h>
#include <cmath>
#include<iostream>
#define M_PI 3.14159265358979323846

int xa = 200, ya = 400;  // Hardcoded base point A
int xb = 400, yb = 400;  // Hardcoded base point B
int clickCount=0;
// ------------------ Pixel Drawing ------------------
void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// ------------------ Bresenham Circle ------------------
void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (x <= y) {
        glBegin(GL_POINTS);
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
        glEnd();
        x++;
        if (d > 0) {
            y--;
            d += 4 * (x - y) + 10;
        } else {
            d += 4 * x + 6;
        }
    }
}

// ------------------ Bresenham Line ------------------
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int x = x1;
    int y = y1;

    // Line is more horizontal
    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            drawPixel(x, y);
            if (p >= 0) {
                y += sy;
                p += 2 * (dy - dx);
            } else {
                p += 2 * dy;
            }
            x += sx;
        }
    }
    // Line is more vertical
    else {
        int p = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            drawPixel(x, y);
            if (p >= 0) {
                x += sx;
                p += 2 * (dx - dy);
            } else {
                p += 2 * dx;
            }
            y += sy;
        }
    }
}
// ------------------ Triangle Drawing ------------------
void drawEquilateralTriangle() {
    int xc = (xa + xb) / 2;
    double height = ((xb - xa) / 2.0) * sqrt(3);
    int topX = xc;
    int topY = ya - height;

    drawLine(xa, ya, xb, ya);
    drawLine(xa, ya, topX, topY);
    drawLine(xb, ya, topX, topY);
}

// ------------------ Inner Circle ------------------
void drawIncircle() {
    int xc = (xa + xb) / 2;
    double r = ((xb - xa) * sqrt(3)) / 6;
    int yc = ya - r;
    drawCircle(xc, yc, r);
}

// ------------------ Outer Circle ------------------
void drawCircumcircle() {
    int xc = (xa + xb) / 2;
    double r = (xb - xa) / sqrt(3);
    int yc = ya - ((xb - xa) * sqrt(3)) / 6;
    drawCircle(xc, yc, r);
}
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickCount < 3) {
        clickCount++;
        glutPostRedisplay();
    }
}
// ------------------ Display Function ------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if(clickCount>=1) drawCircumcircle();
    if(clickCount>=2) drawEquilateralTriangle();
    if(clickCount>=3) drawIncircle();
    glFlush();
}

// ------------------ Initialization ------------------
void init() {
    glClearColor(1, 1, 1, 1);         // White background
    glColor3f(0, 0, 0);               // Black drawing
    glPointSize(1.5);
    gluOrtho2D(0, 600, 600, 0);       // Origin at top-left
}

// ------------------ Main ------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Triangle with Incircle and Circumcircle");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}