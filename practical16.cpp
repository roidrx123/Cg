#include <GL/glut.h>
#include <cmath>

void drawDottedLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int count = 0;

    glBegin(GL_POINTS);
    while (true) {
        if (count % 3 == 0) // Dotted style
            glVertex2i(x1, y1);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }

        count++;
    }
    glEnd();
}

void drawHouse() {
    // House Base
    drawDottedLine(150, 100, 150, 250); // Left wall
    drawDottedLine(150, 100, 350, 100); // Bottom wall
    drawDottedLine(350, 100, 350, 250); // Right wall
    drawDottedLine(150, 250, 350, 250); // Top wall (under triangle)

    // Roof
    drawDottedLine(150, 250, 250, 400); // Left slope
    drawDottedLine(250, 400, 350, 250); // Right slope

    // Door
    drawDottedLine(220, 100, 220, 180); // Left side
    drawDottedLine(220, 180, 280, 180); // Top
    drawDottedLine(280, 180, 280, 100); // Right side

    // Window Rectangle
    drawDottedLine(200, 280, 300, 280); // Bottom
    drawDottedLine(200, 340, 300, 340); // Top
    drawDottedLine(200, 280, 200, 340); // Left
    drawDottedLine(300, 280, 300, 340); // Right

    // Window Horizontal lines
    drawDottedLine(200, 290, 300, 290);
    drawDottedLine(200, 310, 300, 310);
    drawDottedLine(200, 330, 300, 330);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0); // Black color
    drawHouse();
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 500, 0, 500);
    glPointSize(2.0); // Nice visible dots
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q16 - Dotted House using Bresenham Line Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
