#include <GL/glut.h>
#include <cmath>

// Draw pixel
void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham Circle Algorithm
void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (x <= y) {
        drawPixel(xc + x, yc + y);
        drawPixel(xc - x, yc + y);
        drawPixel(xc + x, yc - y);
        drawPixel(xc - x, yc - y);
        drawPixel(xc + y, yc + x);
        drawPixel(xc - y, yc + x);
        drawPixel(xc + y, yc - x);
        drawPixel(xc - y, yc - x);
        if (d < 0) d += 4 * x + 6;
        else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// Bresenham Line Algorithm
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x2 > x1) ? 1 : -1;
    int sy = (y2 > y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

void drawFigure() {
    // Head
    drawCircle(250, 400, 40);

    // Eyes
    drawCircle(235, 415, 5);
    drawCircle(265, 415, 5);

    // Nose (triangle)
    drawLine(250, 405, 245, 395);
    drawLine(250, 405, 255, 395);
    drawLine(245, 395, 255, 395);

    // Mouth (rectangle)
    drawLine(240, 380, 260, 380);
    drawLine(240, 380, 240, 370);
    drawLine(260, 380, 260, 370);
    drawLine(240, 370, 260, 370);

    // Neck
    drawLine(245, 350, 245, 360);
    drawLine(255, 350, 255, 360);

    // Body (rectangle)
    drawLine(220, 350, 280, 350);
    drawLine(220, 350, 220, 290);
    drawLine(280, 350, 280, 290);
    drawLine(220, 290, 280, 290);

    // Left Arm (diagonal line)
    drawLine(220, 350, 190, 320); // upper arm
    drawLine(190, 320, 200, 310); // forearm
    drawLine(220, 330, 200, 310); // lower arm

    // Right Arm (diagonal line)
    drawLine(280, 350, 310, 320);
    drawLine(310, 320, 300, 310);
    drawLine(280, 330, 300, 310);

    // Left Leg (rectangle)
    drawLine(230, 290, 230, 230);
    drawLine(230, 230, 240, 230);
    drawLine(240, 230, 240, 290);

    // Right Leg (rectangle)
    drawLine(260, 290, 260, 230);
    drawLine(260, 230, 270, 230);
    drawLine(270, 230, 270, 290);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    drawFigure();
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 500, 0, 500); // Coordinate system
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Bresenham Stick Figure");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}