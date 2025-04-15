// chit 8

/*
 * CONCEPT: 2D Transformations - Translation and Rotation
 * --------------------------------------------------
 * This program demonstrates basic 2D transformations:
 * 1. Translation: Moving an object in the x and y directions
 * 2. Rotation: Rotating an object around its center point
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
float tx = 0, ty = 0;     // Translation factors
float angle = 0;          // Rotation angle in degrees
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
 * CONCEPT: 2D Rotation Transformation
 * --------------------------------
 * This function applies rotation and translation to the drawn object:
 * 1. Calculate the center point of the object
 * 2. Translate the object so its center is at the origin
 * 3. Apply rotation using the rotation matrix:
 *    [x']   [cos θ  -sin θ] [x]
 *    [y'] = [sin θ   cos θ] [y]
 * 4. Translate the object back to its original position
 * 5. Apply additional translation if needed
 */
void drawObject() {
    if (points.size() < 2) return;
    
    // Calculate center point for rotation
    float centerX = 0, centerY = 0;
    for (const Point& p : points) {
        centerX += p.x;
        centerY += p.y;
    }
    centerX /= points.size();
    centerY /= points.size();
    
    float radians = angle * M_PI / 180.0;
    float cosA = cos(radians);
    float sinA = sin(radians);
    
    glColor3f(0, 0, 1);
    for (size_t i = 0; i < points.size(); i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];
        
        // Apply rotation around center point and translation
        float x1 = centerX + (p1.x - centerX) * cosA - (p1.y - centerY) * sinA + tx;
        float y1 = centerY + (p1.x - centerX) * sinA + (p1.y - centerY) * cosA + ty;
        float x2 = centerX + (p2.x - centerX) * cosA - (p2.y - centerY) * sinA + tx;
        float y2 = centerY + (p2.x - centerX) * sinA + (p2.y - centerY) * cosA + ty;
        
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
 * - q/e: Rotation counter-clockwise/clockwise
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
        
        // Rotation
        case 'e': angle += 5; break;  // Rotate clockwise
        case 'q': angle -= 5; break;  // Rotate counter-clockwise
        
        // Reset transformations
        case 'r':
            tx = ty = 0;
            angle = 0;
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
    glutCreateWindow("chit 8 - 2D Rotation");
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
// q/e - Rotate counter-clockwise/clockwise
// r - Reset transformations
// c - Clear drawing