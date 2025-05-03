#include <GL/glut.h>

int centerX = 320, centerY = 240; // Center of window
int circleCount = 0; // Number of circles to draw (one per click)

void putPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void bresenhamCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
        putPixel(xc + x, yc + y);
        putPixel(xc - x, yc + y);
        putPixel(xc + x, yc - y);
        putPixel(xc - x, yc - y);
        putPixel(xc + y, yc + x);
        putPixel(xc - y, yc + x);
        putPixel(xc + y, yc - x);
        putPixel(xc - y, yc - x);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw axes (green)
    glColor3f(0, 1, 0);
    drawLine(centerX, 120, centerX, 360);
    drawLine(200, centerY, 440, centerY);

    // Draw up to 'circleCount' circles
    glColor3f(0, 0, 0); // Black
    for (int i = 1; i <= circleCount; i++) {
        int radius = 20 * i + 20; // radii: 40, 60, 80, 100, 120
        bresenhamCircle(centerX, centerY, radius);
    }

    glFlush();
}

void mouseHandler(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (circleCount < 5) { // Only draw up to 5 circles
            circleCount++;
            glutPostRedisplay();
        }
    }
}

void init() {
    glClearColor(1, 1, 1, 1); // White background
    glColor3f(0, 0, 0); // Default black
    glPointSize(1);
    gluOrtho2D(0, 640, 0, 480); // 2D view
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Bresenham Circle - One per Click");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutMainLoop();
    return 0;
}
