#include <GL/glut.h>

// Circle radius and spacing
const int radius = 100;
const int gapX = 220; // horizontal gap between circles
const int gapY = 120; // vertical gap between rows

// Window dimensi
// ---------------- Bresenham’s Circle ----------------
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

// ---------------- Draw Olympic Rings ----------------
void drawOlympicRings() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Top row
    glColor3f(0, 0, 1); // Blue
    drawCircle(200, 400, radius);

    glColor3f(0, 0, 0); // Black
    drawCircle(200 + gapX, 400, radius);

    glColor3f(1, 0, 0); // Red
    drawCircle(200 + 2 * gapX, 400, radius);

    // Bottom row
    glColor3f(1, 1, 0); // Yellow
    drawCircle(200 + gapX / 2, 400 - gapY, radius);

    glColor3f(0, 1, 0); // Green
    drawCircle(200 + 3 * gapX / 2, 400 - gapY, radius);

    glFlush();
}

// ---------------- Init & Main ----------------
void init() {
     glClearColor(1, 1, 1, 1);      // White background
    glColor3f(0, 0, 0);            // Default drawing color
    glPointSize(1.0);
    gluOrtho2D(0, 800, 0, 600);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bresenham's Circle - Olympic Rings");
    init();
    glutDisplayFunc(drawOlympicRings);
    glutMainLoop();
    return 0;
}
