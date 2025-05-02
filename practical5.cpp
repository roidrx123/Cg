#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

int xmin, ymin, xmax, ymax;
int clickCount = 0;
bool flag = false;

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;

    int x = x1;
    int y = y1;

    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            drawPixel(x, y);
            x += sx;
            if (p >= 0) {
                y += sy;
                p += 2 * (dy - dx);
            } else {
                p += 2 * dy;
            }
        }
    } else {
        int p = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            drawPixel(x, y);
            y += sy;
            if (p >= 0) {
                x += sx;
                p += 2 * (dx - dy);
            } else {
                p += 2 * dx;
            }
        }
    }
}

void chessboard(){

    int dx = (xmax - xmin) / 4;
    int dy = (ymax - ymin) / 4;

    // Draw squares (alternate color)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x1 = xmin + j * dx;
            int y1 = ymin + i * dy;
            int x2 = x1 + dx;
            int y2 = y1 + dy;

            if ((i + j) % 2 == 0)
                glColor3f(1.0, 1.0, 1.0); // white
            else
                glColor3f(0.0, 0.0, 0.0); // black

            glBegin(GL_POLYGON);
            glVertex2i(x1, y1);
            glVertex2i(x2, y1);
            glVertex2i(x2, y2);
            glVertex2i(x1, y2);
            glEnd();
        }
    }
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0; i <= 4; i++) {
        int x = xmin + i * dx;
        drawLine(x, ymin, x, ymax);
    }
    for (int i = 0; i <= 4; i++) {
        int y = ymin + i * dy;
        drawLine(xmin, y, xmax, y);
    }
}

// Display Callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (flag)
        chessboard();
    glFlush();
}

// Mouse Input
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = 600 - y;

        if (clickCount == 0) {
            xmin = x;
            ymax = y;
            clickCount++;
            cout << "First corner (xmin, ymax): (" << xmin << ", " << ymax << ")\n";
        } else if (clickCount == 1) {
            xmax = x;
            ymin = y;
            cout << "Second corner (xmax, ymin): (" << xmax << ", " << ymin << ")\n";
            flag = true;
            clickCount = 0;
            glutPostRedisplay();
        }
    }
}

// OpenGL Init
void init() {
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    glPointSize(1);
    gluOrtho2D(0, 600, 0, 600);
}

// Main Function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Chessboard 4x4 with Bresenham & Mouse Input");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
