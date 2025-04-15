/*
 * CONCEPT: Polygon Filling Algorithms
 * -------------------------------
 * This program implements two polygon filling methods:
 * 1. Flood Fill / Seed Fill: Fills from a point, spreading to adjacent pixels
 * 2. Boundary Fill: Fills from a point until reaching boundary color
 */

#include <GL/glut.h>
#include <vector>
#include <stack>
#include <iostream>  // Add this line
using namespace std;

struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

vector<Point> points;
bool isDrawing = false;
bool isFilling = false;
int fillMethod = 0;  // 0: Boundary Fill, 1: Flood Fill

// Colors
float boundaryColor[3] = {0, 0, 1};    // Blue
float fillColor[3] = {1, 0, 0};        // Red
float backgroundColor[3] = {1, 1, 1};  // White

/*
 * CONCEPT: Color Reading and Comparison
 * ---------------------------------
 * These functions handle color operations:
 * 1. Reading pixel color from the framebuffer
 * 2. Comparing colors for equality within a tolerance
 * 3. Setting pixel colors for drawing
 */
void getPixelColor(int x, int y, float* color) {
    glReadPixels(x + 300, y + 300, 1, 1, GL_RGB, GL_FLOAT, color);
}

bool compareColors(float* c1, float* c2) {
    float tolerance = 0.01;
    return (abs(c1[0] - c2[0]) < tolerance &&
            abs(c1[1] - c2[1]) < tolerance &&
            abs(c1[2] - c2[2]) < tolerance);
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

/*
 * CONCEPT: Boundary Fill Algorithm
 * ----------------------------
 * This algorithm fills a region by:
 * 1. Starting from a seed point
 * 2. Recursively filling in all directions
 * 3. Stopping when it hits the boundary color
 */
void boundaryFill(int x, int y) {
    float currentColor[3];
    getPixelColor(x, y, currentColor);
    
    if (!compareColors(currentColor, boundaryColor) && 
        !compareColors(currentColor, fillColor)) {
        glColor3fv(fillColor);
        setPixel(x, y);
        
        boundaryFill(x + 1, y);
        boundaryFill(x - 1, y);
        boundaryFill(x, y + 1);
        boundaryFill(x, y - 1);
    }
}

/*
 * CONCEPT: Flood Fill Algorithm
 * -------------------------
 * This algorithm fills a region by:
 * 1. Starting from a seed point
 * 2. Recursively filling in all directions
 * 3. Stopping when it hits a different color
 */
void floodFill(int x, int y) {
    float currentColor[3];
    getPixelColor(x, y, currentColor);
    
    if (compareColors(currentColor, backgroundColor)) {
        glColor3fv(fillColor);
        setPixel(x, y);
        
        floodFill(x + 1, y);
        floodFill(x - 1, y);
        floodFill(x, y + 1);
        floodFill(x, y - 1);
    }
}

void drawPolygon() {
    if (points.size() < 2) return;
    
    glColor3fv(boundaryColor);
    glBegin(GL_LINE_LOOP);
    for (const Point& p : points) {
        glVertex2i(p.x, p.y);
    }
    glEnd();
}

void drawAxisLines() {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2i(-300, 0); glVertex2i(300, 0);
    glVertex2i(0, -300); glVertex2i(0, 300);
    glEnd();
}

/*
 * CONCEPT: User Interaction
 * ----------------------
 * Mouse controls:
 * - Left click: Add polygon vertices or select fill point
 * - Right click: Complete polygon
 * - Middle click: Start filling from clicked point
 */
void mouseClick(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        int mouseX = x - 300;
        int mouseY = 300 - y;
        
        if (button == GLUT_LEFT_BUTTON) {
            if (!isDrawing) {
                points.clear();
                isDrawing = true;
            }
            points.push_back(Point(mouseX, mouseY));
            glutPostRedisplay();
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (isDrawing && points.size() >= 3) {
                isDrawing = false;
                glutPostRedisplay();
            }
        }
        else if (button == GLUT_MIDDLE_BUTTON) {
            if (!isDrawing && points.size() >= 3) {
                if (fillMethod == 0) {
                    boundaryFill(mouseX, mouseY);
                } else {
                    floodFill(mouseX, mouseY);
                }
            }
        }
    }
}

/*
 * CONCEPT: Keyboard Controls
 * ----------------------
 * Keyboard input allows:
 * - Switching between fill methods
 * - Clearing the polygon
 * - Changing colors
 */
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'm':  // Switch fill method
            fillMethod = 1 - fillMethod;
            std::cout << "Fill method: " << (fillMethod == 0 ? "Boundary Fill" : "Flood Fill") << std::endl;
            break;
            
        case 'c':  // Clear polygon
            points.clear();
            isDrawing = false;
            glutPostRedisplay();
            break;
            
        case 'r':  // Red fill color
            fillColor[0] = 1.0f;
            fillColor[1] = 0.0f;
            fillColor[2] = 0.0f;
            break;
            
        case 'g':  // Green fill color
            fillColor[0] = 0.0f;
            fillColor[1] = 1.0f;
            fillColor[2] = 0.0f;
            break;
            
        case 'b':  // Blue fill color
            fillColor[0] = 0.0f;
            fillColor[1] = 0.0f;
            fillColor[2] = 1.0f;
            break;
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxisLines();
    drawPolygon();
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-300, 300, -300, 300);
    glPointSize(2);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Practical 12 - Polygon Filling");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

// Controls:
// Left click to add polygon vertices
// Right click to complete polygon
// Middle click to fill from point
// 'm' - Switch between Boundary Fill and Flood Fill
// 'r/g/b' - Change fill color
// 'c' - Clear polygon