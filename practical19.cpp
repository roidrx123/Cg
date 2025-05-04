
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
using namespace std;

int x1_saved, y1_saved, x2_saved, y2_saved;
bool lineDrawn = false;
int currentStyle = 1;

// Draw axes
void drawAxes() {
    glColor3f(0, 0, 0); // Black axes
    glBegin(GL_LINES);
    glVertex2i(300, 0); glVertex2i(300, 600); // Vertical
    glVertex2i(0, 300); glVertex2i(600, 300); // Horizontal
    glEnd();
}

// Draw a pixel
void drawPoint(int x, int y, float size = 2.0f) {
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// DDA line drawing with styles
void drawDDA(int x1, int y1, int x2, int y2, int style) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    int steps = (int)max(abs(dx), abs(dy));
    float xInc = dx / steps;
    float yInc = dy / steps;
    float x = x1, y = y1;

  for (int i = 0; i <= steps; ++i) {
    bool draw = false;

    if (style == 1) { // Simple
        draw = true;
    } else if (style == 2) { // Dotted
        draw = (i % 8 == 0);
    } else if (style == 3) { // Dashed
        draw = ((i % 15) < 8);
    } else if (style == 4) { // Solid (thick red)
        draw = true;
    }

    if (draw) {
        if (style == 4) {
            glColor3f(1, 0, 0);
            drawPoint(round(x), round(y), 4.0f);
        } else {
            glColor3f(0, 0, 1);
            drawPoint(round(x), round(y));
        }
    }

    x += xInc;
    y += yInc;
}

}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    if (lineDrawn)
        drawDDA(x1_saved, y1_saved, x2_saved, y2_saved, currentStyle);
    glFlush();
}

// Mouse input
void mouseClick(int button, int state, int x, int y) {
    if (lineDrawn) return;
    static int count = 0;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int yClick = 600 - y;
        if (count == 0) {
            x1_saved = x;
            y1_saved = yClick;
            count++;
        } else {
            x2_saved = x;
            y2_saved = yClick;
            lineDrawn = true;
            glutPostRedisplay();
        }
    }
}

// Menu
void menuHandler(int option) {
    currentStyle = option;
    glutPostRedisplay();
}

void createMenu() {
    glutCreateMenu(menuHandler);
    glutAddMenuEntry("Simple Line", 1);
    glutAddMenuEntry("Dotted Line", 2);
    glutAddMenuEntry("Dashed Line", 3);
    glutAddMenuEntry("Solid Line", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Init
void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 600, 0, 600); // 2D projection
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("DDA Line Styles with Menu");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    createMenu();
    glutMainLoop();
    return 0;
}
