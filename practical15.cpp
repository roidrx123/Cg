#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int radius = 50;
const float PI = 3.14159265;
int centerX = 300, centerY = 300;

int petalCenters[6][2]; // Store petal center coordinates
bool flowerDrawn = false; // Flag to track if flower is already drawn

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

// ---------------- Draw Flower ----------------
void drawFlower() {
    // Compute petal centers
    for (int i = 0; i < 6; i++) {
        float angle = 2 * PI * i / 6;
        int px = centerX + 2 * radius * cos(angle);
        int py = centerY + 2 * radius * sin(angle);
        petalCenters[i][0] = px;
        petalCenters[i][1] = py;
    }

    // Draw center
    drawCircle(centerX, centerY, radius);

    // Draw petals
    for (int i = 0; i < 6; i++) {
        drawCircle(petalCenters[i][0], petalCenters[i][1], radius);
    }

    // Draw outermost circle
    drawCircle(centerX, centerY, 2 * radius);
}

// ---------------- Display ----------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    if (flowerDrawn) {
        drawFlower();
    }
    glFlush();
}

// ---------------- Mouse Click Handler ----------------
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !flowerDrawn) {
        flowerDrawn = true;
        glutPostRedisplay();
    }
}

// ---------------- Init ----------------
void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 600, 0, 600);
}

// ---------------- Main ----------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Click to Draw Full Flower - Bresenham");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
