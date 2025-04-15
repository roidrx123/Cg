#include <GL/glut.h>
#include <cmath>
using namespace std;

int xa = 200, ya = 100;
int xb = 400, yb = 250;

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
            glVertex2i(x,y);
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
            glVertex2i(x,y);
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

// Bresenham's Circle Drawing
void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
        x++;
        if (d > 0) { y--; d += 4 * (x - y) + 10; }
        else d += 4 * x + 6;
    }
}

void drawHouse() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    // House base (rectangle)
    drawLine(xa, ya, xb, ya);         // bottom
    drawLine(xb, ya, xb, yb);         // right
    drawLine(xb, yb, xa, yb);         // top
    drawLine(xa, yb, xa, ya);         // left
   // Roof (triangle)
    int midX = (xa + xb) / 2;
    int roofPeakY = yb + 100;
    drawLine(xa, yb, midX, roofPeakY);
    drawLine(xb, yb, midX, roofPeakY);

    //back
        drawLine(xb, ya+150, xb+200, yb);
        drawLine(xb+200, ya, xb+200, yb+100);
        drawLine(xa+100, ya, xb+200, ya);
        drawLine( midX, roofPeakY,xa+400, yb+100);


    // Circle (window in roof)
    drawCircle(midX, yb + 40, 20);
 // Door (in the middle of house base)
    int doorWidth = 40;
    int doorHeight = 80;
    int doorLeft = (xa + xb) / 2 - doorWidth / 2;
    int doorRight = doorLeft + doorWidth;
    int doorTop = ya + doorHeight;

    drawLine(doorLeft, ya, doorLeft, doorTop);     // left door edge
    drawLine(doorRight, ya, doorRight, doorTop);   // right door edge
    drawLine(doorLeft, doorTop, doorRight, doorTop); // top of door

    glEnd();
    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
    glColor3f(0.0, 0.0, 0.0);          // black drawing
    gluOrtho2D(0, 640, 0, 480);        // coordinate system
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Simple House with Bresenham Algorithms");
    init();
    glutDisplayFunc(drawHouse);
    glutMainLoop();
    return 0;
}
