#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int radius = 50; // radius of each small and center circle
const float PI = 3.14159265;

int centerX = 300, centerY = 300; // default center

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

// Draw complete flower structure
void drawFlowerStructure(int xc, int yc) {
    // Draw center circle
    drawCircle(xc, yc, radius);

    // Draw 6 surrounding circles
    int petalCenters[6][2];
    for (int i = 0; i < 6; i++) {
        float angle = 2 * PI * i / 6;
        int px = xc + 2 * radius * cos(angle); // 2 * radius to touch center circle
        int py = yc + 2 * radius * sin(angle);
        petalCenters[i][0] = px;
        petalCenters[i][1] = py;
        drawCircle(px, py, radius);
    }

    // Draw outermost circle that passes through all 6 petal centers
    drawCircle(xc, yc, 2 * radius); // Its radius is 2 * radius
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    drawFlowerStructure(centerX, centerY);
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 600, 0, 600);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Circle Flower Structure - Bresenham");
    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
