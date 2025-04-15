#include <GL/glut.h>

void drawPixel(int x, int y) {
    glVertex2i(x, y);
}

void bresenhamDottedLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int pixelCount = 0;
    
    glBegin(GL_POINTS);
    while (true) {
        if (pixelCount % 5 == 0) {  // Draw every 5th pixel for dotted effect
            drawPixel(x1, y1);
        }
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
        pixelCount++;
    }
    glEnd();
}

void drawHouse() {
    glColor3f(0, 0, 0);
    
    // Roof
    bresenhamDottedLine(-60, 100, 0, 150);   // Left roof
    bresenhamDottedLine(0, 150, 60, 100);    // Right roof
    
    // Main house structure
    bresenhamDottedLine(-60, 100, 60, 100);  // Top
    bresenhamDottedLine(-60, 0, 60, 0);      // Bottom
    bresenhamDottedLine(-60, 100, -60, 0);   // Left wall
    bresenhamDottedLine(60, 100, 60, 0);     // Right wall
    
    // Window
    bresenhamDottedLine(-30, 80, -10, 80);   // Top
    bresenhamDottedLine(-30, 60, -10, 60);   // Bottom
    bresenhamDottedLine(-30, 80, -30, 60);   // Left
    bresenhamDottedLine(-10, 80, -10, 60);   // Right
    
    // Window cross
    bresenhamDottedLine(-30, 70, -10, 70);   // Horizontal
    bresenhamDottedLine(-20, 80, -20, 60);   // Vertical
    
    // Door
    bresenhamDottedLine(-15, 0, -15, 30);    // Left
    bresenhamDottedLine(15, 0, 15, 30);      // Right
    bresenhamDottedLine(-15, 30, 15, 30);    // Top
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawHouse();
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-200, 200, -50, 200);
    glPointSize(2);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Practical 16 - Dotted House using Bresenham's Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}