#include <iostream>
#include <GL/glut.h>
#include <cmath>

int xa = 100, ya = 100;
int xb = 500, yb = 300;  // Rectangle with a longer width

int clickCount = 0;

// ------------------ Pixel Drawing ------------------
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
    } else {
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

void drawCircle(int xc, int yc, float r) {
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

void outerRectangle() {
    drawLine(xa, ya, xb, ya); // Top edge
    drawLine(xb, ya, xb, yb); // Right edge
    drawLine(xb, yb, xa, yb); // Bottom edge
    drawLine(xa, yb, xa, ya); // Left edge
}

void innerSquare() {


    // Draw a square with its sides touching the middle of the rectangle's edges
    drawLine((xa + xb) / 2, ya, xb, (ya + yb) / 2);
    drawLine(xb, (ya + yb) / 2, (xa + xb) / 2, yb);
    drawLine((xa + xb) / 2, yb, xa, (ya + yb) / 2);
    drawLine(xa, (ya + yb) / 2, (xa + xb) / 2, ya);
}

void insideCircle() {
    // Calculate the side of the square (distance between two opposite corners)
    int side = (yb - ya);

    // Radius of the circle will be half of the side of the square
    float radius = side / 2.2;

    // Calculate center of the square
    int centerX = (xa + xb) / 2;
    int centerY = (ya + yb) / 2;

    // Draw the circle inside the square
    drawCircle(centerX, centerY, radius);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (clickCount >= 1) outerRectangle();    // Draw rectangle
    if (clickCount >= 2) innerSquare();       // Draw inner square
    if (clickCount >= 3) insideCircle();      // Draw circle inside the square

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickCount < 3) {
        clickCount++;
        glutPostRedisplay();
    }
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
    glutCreateWindow("Rectangle, Square, Circle");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
