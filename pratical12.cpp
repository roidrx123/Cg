#include <GL/glut.h>
float fillColor[]     = {0.0, 1.0, 0.0}; // Green
float boundaryColor[] = {1.0, 0.0, 0.0}; // Red

// Set a pixel
void setPixel(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// Get pixel color
void getPixelColor(int x, int y, float color[3]) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

// Compare colors
bool isSameColor(float a[3], float b[3]) {
    for (int i = 0; i < 3; ++i)
        if ((int)(a[i]*255) != (int)(b[i]*255))
            return false;
    return true;
}

// Flood Fill
void floodFill(int x, int y, float oldColor[3]) {
    float current[3];
    getPixelColor(x, y, current);

    if (isSameColor(current, oldColor)) {
        setPixel(x, y, fillColor[0], fillColor[1], fillColor[2]);
        floodFill(x+1, y, oldColor);
        floodFill(x-1, y, oldColor);
        floodFill(x, y+1, oldColor);
        floodFill(x, y-1, oldColor);
    }
}

// Boundary Fill
void boundaryFill(int x, int y) {
    float current[3];
    getPixelColor(x, y, current);

    if (!isSameColor(current, boundaryColor) && !isSameColor(current, fillColor)) {
        setPixel(x, y, fillColor[0], fillColor[1], fillColor[2]);
        boundaryFill(x+1, y);
        boundaryFill(x-1, y);
        boundaryFill(x, y+1);
        boundaryFill(x, y-1);
    }
}

// Draw polygon (red border)
void drawPolygon() {
    glColor3f(boundaryColor[0], boundaryColor[1], boundaryColor[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2i(300, 300);
    glVertex2i(500, 320);
    glVertex2i(450, 500);
    glVertex2i(320, 460);
    glEnd();
    glFlush();
}

// Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawPolygon();
}

// Menu handler
void menu(int option) {
    int seedX = 400, seedY = 400; // Point inside polygon
    float oldColor[3];
    getPixelColor(seedX, seedY, oldColor);
    if (option == 1) {
        floodFill(seedX, seedY, oldColor);
    } else if (option == 2) {
        boundaryFill(seedX, seedY);
    } else if (option == 3) {
        exit(0);
    }
}
void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 800, 0, 600);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Polygon Fill without Mouse");
    init();
    glutDisplayFunc(display);

    glutCreateMenu(menu);
    glutAddMenuEntry("Flood Fill", 1);
    glutAddMenuEntry("Boundary Fill", 2);
    glutAddMenuEntry("Exit", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
