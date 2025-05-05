#include <GL/glut.h>
int fillType = 0; // 1 = Flood Fill, 2 = Boundary Fill

// Set one pixel color
void drawPixel(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// Get pixel color at x, y
void getPixelColor(int x, int y, float color[3]) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

// Check if two colors are equal
bool isColorMatch(float a[3], float b[3]) {
    for (int i = 0; i < 3; i++) {
        if ((int)(a[i]*255) != (int)(b[i]*255))
            return false;
    }
    return true;
}

// Flood fill algorithm
void floodFill(int x, int y, float oldColor[3], float newColor[3]) {
    float current[3];
    getPixelColor(x, y, current);

    if (isColorMatch(current, oldColor)) {
        drawPixel(x, y, newColor[0], newColor[1], newColor[2]);
        floodFill(x+1, y, oldColor, newColor);
        floodFill(x-1, y, oldColor, newColor);
        floodFill(x, y+1, oldColor, newColor);
        floodFill(x, y-1, oldColor, newColor);
    }
}

// Boundary fill algorithm
void boundaryFill(int x, int y, float fillColor[3], float borderColor[3]) {
    float current[3];
    getPixelColor(x, y, current);

    if (!isColorMatch(current, borderColor) && !isColorMatch(current, fillColor)) {
        drawPixel(x, y, fillColor[0], fillColor[1], fillColor[2]);
        boundaryFill(x+1, y, fillColor, borderColor);
        boundaryFill(x-1, y, fillColor, borderColor);
        boundaryFill(x, y+1, fillColor, borderColor);
        boundaryFill(x, y-1, fillColor, borderColor);
    }
}

// Mouse click event
void mouse(int button, int state, int x, int y) {
    y = 600 - y; // Flip y to OpenGL coordinates
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float clickedColor[3];
        float fillColor[] = {0, 1, 0}; // Green
        float borderColor[] = {1, 0, 0}; // Red
        getPixelColor(x, y, clickedColor);

        if (fillType == 1)
            floodFill(x, y, clickedColor, fillColor);
        else if (fillType == 2)
            boundaryFill(x, y, fillColor, borderColor);
    }
}

// Draw a simple polygon
void drawPolygon() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0); // Red border
    glBegin(GL_LINE_LOOP);
    glVertex2i(300, 300);
    glVertex2i(500, 320);
    glVertex2i(450, 500);
    glVertex2i(320, 460);
    glEnd();
    glFlush();
}

// Menu handler
void handleMenu(int option) {
    fillType = option;
    if (option == 3) exit(0);
}

// Initialization
void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 800, 0, 600); // 2D view
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Easy Fill Algorithm");
    init();
    glutDisplayFunc(drawPolygon);
    glutMouseFunc(mouse);
    glutCreateMenu(handleMenu);
    glutAddMenuEntry("Flood Fill", 1);
    glutAddMenuEntry("Boundary Fill", 2);
    glutAddMenuEntry("Exit", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}
