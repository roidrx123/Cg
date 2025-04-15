#include <GL/glut.h>
#include <cmath>
#include<iostream>
using namespace std;
int clickCount = 0;

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}
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
/*

*/
void drawOuterSquare() {
    int cx = 320, cy = 240, h = 100;
    drawLine(cx - h, cy + h, cx + h, cy + h);
    drawLine(cx + h, cy + h, cx + h, cy - h);
    drawLine(cx + h, cy - h, cx - h, cy - h);
    drawLine(cx - h, cy - h, cx - h, cy + h);
}

void drawDiamondAndInnerSquare() {
    int cx = 320, cy = 240, h = 100;
    int topX = cx, topY = cy + h;
    int rightX = cx + h, rightY = cy;
    int bottomX = cx, bottomY = cy - h;
    int leftX = cx - h, leftY = cy;

    // Diamond
    drawLine(topX, topY, rightX, rightY);
    drawLine(rightX, rightY, bottomX, bottomY);
    drawLine(bottomX, bottomY, leftX, leftY);
    drawLine(leftX, leftY, topX, topY);

    // Inner square (midpoints)
    int itx = (topX + rightX) / 2, ity = (topY + rightY) / 2;
    int irx = (rightX + bottomX) / 2, iry = (rightY + bottomY) / 2;
    int ibx = (bottomX + leftX) / 2, iby = (bottomY + leftY) / 2;
    int ilx = (leftX + topX) / 2, ily = (leftY + topY) / 2;

    drawLine(itx, ity, irx, iry);
    drawLine(irx, iry, ibx, iby);
    drawLine(ibx, iby, ilx, ily);
    drawLine(ilx, ily, itx, ity);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (clickCount >= 1) drawOuterSquare();
    if (clickCount >= 2) drawDiamondAndInnerSquare();
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickCount < 2) {
        clickCount++;
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    gluOrtho2D(0, 640, 0, 480);
    glPointSize(2);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Click to Draw Shapes");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}