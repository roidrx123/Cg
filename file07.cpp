// chit 7

/*
 * CONCEPT: 2D Transformations - Translation and Scaling
 * -------------------------------------------------
 * This program demonstrates basic 2D transformations:
 * 1. Translation: Moving an object in the x and y directions
 * 2. Scaling: Changing the size of an object
 * 
 * The transformations are applied to a user-drawn polygon using the DDA line algorithm.
 */

#include <GL/glut.h>
#include <vector>
#include <cmath>
using namespace std;

struct Point {
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

vector<Point> points;
float tx = 0, ty = 0;  // Translation factors
float sx = 1, sy = 1;  // Scaling factors
bool isDrawing = false;

void drawPoint(int x, int y) {
    glVertex2i(x, y);
}

/*
 * CONCEPT: Digital Differential Analyzer (DDA) Line Algorithm
 * -------------------------------------------------------
 * This algorithm draws a line by:
 * 1. Calculating the number of steps based on the greater of dx or dy
 * 2. Computing x and y increments per step
 * 3. Plotting points at each step with rounding to get integer coordinates
 * 
 * DDA uses floating-point arithmetic but is simpler to implement than Bresenham's.
 */
void DDALine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    
    float x = x1, y = y1;
    
    glBegin(GL_POINTS);
    for(int i = 0; i <= steps; i++) {
        drawPoint(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
}

/*
 * CONCEPT: 2D Transformations Implementation
 * ---------------------------------------
 * This function applies transformations to the drawn object:
 * 1. Scaling: Multiplying coordinates by scaling factors (sx, sy)
 * 2. Translation: Adding translation values (tx, ty) to coordinates
 * 
 * The transformations are applied to each vertex of the polygon before drawing.
 */
void drawObject() {
    if (points.size() < 2) return;
    
    glColor3f(0, 0, 1);
    for (size_t i = 0; i < points.size(); i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];
        
        // Apply transformations
        float x1 = (p1.x * sx) + tx;
        float y1 = (p1.y * sy) + ty;
        float x2 = (p2.x * sx) + tx;
        float y2 = (p2.y * sy) + ty;
        
        DDALine(round(x1), round(y1), round(x2), round(y2));
    }
}

void drawAxisLines() {
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2i(-300, 0); glVertex2i(300, 0);
    glVertex2i(0, -300); glVertex2i(0, 300);
    glEnd();
}

/*
 * CONCEPT: User Interaction for Object Creation
 * -----------------------------------------
 * Mouse clicks are used to define the object:
 * - Left clicks add vertices to the polygon
 * - Right-click completes the polygon
 * 
 * The program connects the vertices in order, with the last vertex connecting to the first.
 */
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (!isDrawing) {
                points.clear();
                isDrawing = true;
            }
            points.push_back(Point(x - 300, 300 - y));
            glutPostRedisplay();
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        isDrawing = false;
        glutPostRedisplay();
    }
}

/*
 * CONCEPT: Keyboard Controls for Transformations
 * ------------------------------------------
 * Keyboard input allows applying transformations:
 * - w/a/s/d: Translation in up/left/down/right directions
 * - +/-: Scaling up/down
 * - r: Reset all transformations
 * - c: Clear the drawing
 */
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        // Translation
        case 'w': ty += 10; break;  // Up
        case 's': ty -= 10; break;  // Down
        case 'a': tx -= 10; break;  // Left
        case 'd': tx += 10; break;  // Right
        
        // Scaling
        case '+': sx *= 1.1; sy *= 1.1; break;  // Scale up
        case '-': sx *= 0.9; sy *= 0.9; break;  // Scale down
        
        // Reset transformations
        case 'r':
            tx = ty = 0;
            sx = sy = 1;
            break;
        
        // Clear drawing
        case 'c':
            points.clear();
            isDrawing = false;
            break;
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxisLines();
    drawObject();
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
    glutCreateWindow("chit 7 - 2D Transformations");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

// Controls:
// Left click to add points for the object
// Right click to finish drawing
// w/a/s/d - Translation (up/left/down/right)
// +/- - Scale up/down
// r - Reset transformations
// c - Clear drawing
