#include <GL/glut.h>
#include <iostream>
using namespace std;
int clickCount=0;
// ---------------- Bresenham Circle Drawing ----------------
void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;

    glBegin(GL_POINTS);
    while (x <= y) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
    glEnd();
}

// ---------------- Draw Quadrant Axes ----------------
void drawAxes() {
    glColor3f(0, 1, 0); // Green lines
    glBegin(GL_LINES);
    glVertex2i(300, 0);     // Vertical line (Y-axis)
    glVertex2i(300, 600);
    glVertex2i(0, 300);     // Horizontal line (X-axis)
    glVertex2i(600, 300);
    glEnd();
}

// ---------------- Draw 5 Concentric Circles ----------------
void drawConcentricCircles() {

    glColor3f(0, 0, 0); // Black circles
    for (int r = 30; r <= 150; r += 30) {
        drawCircle(300, 300, r);  // Center at (300, 300)
    }
}

// ---------------- Display Function ----------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
        drawAxes();

    if(clickCount>=1) drawConcentricCircles();
    glFlush();
}

// ---------------- Init ----------------
void init() {
    glClearColor(1, 1, 1, 1);        // White background
    glColor3f(0, 0, 0);              // Default color
    glPointSize(1.0);
    gluOrtho2D(0, 600, 0, 600);      // 2D coordinates
}
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            clickCount++;
    }
}

// ---------------- Main ----------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("5 Concentric Circles at Center - Bresenham");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMainLoop();
    return 0;
}
