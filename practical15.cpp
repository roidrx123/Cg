#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int radius = 50;
const float PI = 3.14159265;
int centerX = 300, centerY = 300;

int currentCircle = 0; // Tracks how many circles are drawn
int petalCenters[6][2]; // Store petal center coordinates
bool petalsComputed = false; // Flag to avoid recomputation

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
void drawFlowerStructureStepByStep(int step) {
    // Compute petal centers once
    if (!petalsComputed) {
        for (int i = 0; i < 6; i++) {
            float angle = 2 * PI * i / 6;
            int px = centerX + 2 * radius * cos(angle);
            int py = centerY + 2 * radius * sin(angle);
            petalCenters[i][0] = px;
            petalCenters[i][1] = py;
        }
        petalsComputed = true;
    }

    if (step >= 1) {
        // Step 1: draw center circle
        drawCircle(centerX, centerY, radius);
    }

    // Step 2-7: Draw petal i-2 (i from 2 to 7)
    if (step >= 2 && step <= 7) {
        for (int i = 0; i < step - 1; i++) {
            drawCircle(petalCenters[i][0], petalCenters[i][1], radius);
        }
    }

    if (step == 8) {
        // Step 8: Draw all petals + outermost circle
        for (int i = 0; i < 6; i++) {
            drawCircle(petalCenters[i][0], petalCenters[i][1], radius);
        }
        drawCircle(centerX, centerY, 2 * radius);
    }
}

// ---------------- Display ----------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    drawFlowerStructureStepByStep(currentCircle);
    glFlush();
}

// ---------------- Mouse Click Handler ----------------
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (currentCircle < 8) {
            currentCircle++;
            glutPostRedisplay();
        }
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
    glutCreateWindow("Click to Draw Flower Circle - Bresenham");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
