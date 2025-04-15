#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

int centerX = 300, centerY = 300, radius = 100; // Set values manually

// ---------------- Draw Circle ----------------
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
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

// ---------------- Draw Intersecting Circles ----------------
void drawIntersectingCircles() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0); // Red circles

    drawCircle(centerX, centerY, radius);                          // Center
    drawCircle(centerX + radius, centerY, radius);                // Right
    drawCircle(centerX - radius, centerY, radius);                // Left
    drawCircle(centerX, centerY + radius, radius);                // Top
    drawCircle(centerX, centerY - radius, radius);                // Bottom

    glFlush();
}

// ---------------- Init & Display ----------------
void init() {
    glClearColor(1, 1, 1, 1);      // White background
    glColor3f(0, 0, 0);            // Default drawing color
    glPointSize(1.0);
    gluOrtho2D(0, 600, 0, 600);    // Set coordinate system
}

void display() {
    drawIntersectingCircles();     // Draw directly without mouse
}

// ---------------- Main ----------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Intersecting Circles");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
