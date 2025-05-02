#include <GL/glut.h>
#include <cmath>
#include<iostream>
using namespace std;
int clickCount=0;

int side = 300;

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}
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
            drawPixel(x, y);
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
            drawPixel(x, y);
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
        if (d < 0)
            d = d + 4 * x + 6;
        else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
void drawCicle()
{
    float height = side * sqrt(3) / 2;
    float radius = height / 3;
// Incenter of triangle (circle center)
    int xc = 0;
    int yc = -height / 3 + radius;
    // Draw inscribed circle
    drawCircle(xc, yc, radius);
}
void drawTriangle(){
 float height = side * sqrt(3) / 2;
    float radius = height / 3;
    // Triangle vertices
    int x1 = -side / 2, y1 = -height / 3;
    int x2 = side / 2, y2 = -height / 3;
    int x3 = 0, y3 = 2 * height / 3;
    // Draw triangle
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if(clickCount>=1) drawTriangle();
    if(clickCount>=2) drawCicle();

    glFlush();
}
void mouse(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&clickCount<2){
        clickCount++;
    }
    glutPostRedisplay();
}
void init() {
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    glPointSize(1);
    gluOrtho2D(-400, 400, -400, 400);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Inscribed Circle in Triangle - Bresenham");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
