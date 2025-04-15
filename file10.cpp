// chit 10 - Sutherland Line Clipping

#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

/*
 * CONCEPT: Sutherland Line Clipping Algorithm
 * ---------------------------------------
 * This algorithm determines which portions of a line are visible within a 
 * rectangular clipping window. It uses region codes to efficiently determine
 * if a line is completely inside, completely outside, or needs clipping.
 */

// Region codes for Cohen-Sutherland algorithm
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

// Clipping window coordinates
int xmin = -100, ymin = -100, xmax = 100, ymax = 100;
bool windowDefined = true;

// Line endpoints
vector<Point> lines;
int clickCount = 0;
bool drawingLine = false;

/*
 * CONCEPT: Region Code Computation
 * ------------------------------
 * Each point is assigned a 4-bit code that indicates its position relative to the
 * clipping window boundaries. This helps quickly determine if a point is inside or
 * outside the window and which boundaries it crosses.
 */
int computeCode(int x, int y) {
    int code = INSIDE;
    
    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;
    
    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;
    
    return code;
}

/*
 * CONCEPT: Sutherland Line Clipping Algorithm Implementation
 * ------------------------------------------------------
 * The algorithm works by:
 * 1. Computing region codes for both endpoints
 * 2. If both points have code 0, the line is completely inside (accept)
 * 3. If bitwise AND of codes is not 0, the line is completely outside (reject)
 * 4. Otherwise, the line needs clipping - find intersection with a boundary
 * 5. Replace the outside point with the intersection point
 * 6. Repeat until the line is accepted or rejected
 */
void sutherlandLineClip(int x1, int y1, int x2, int y2, int& x1_clip, int& y1_clip, int& x2_clip, int& y2_clip, bool& visible) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;
    
    while (true) {
        if (!(code1 | code2)) { // Both endpoints inside window
            accept = true;
            break;
        } else if (code1 & code2) { // Both endpoints outside same region
            accept = false;
            break;
        } else {
            // Line needs clipping - at least one endpoint is outside
            int code_out = code1 ? code1 : code2;
            int x, y;
            
            // Find intersection point using parametric line equations
            if (code_out & TOP) { // Point is above the clip window
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (code_out & BOTTOM) { // Point is below the clip window
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (code_out & RIGHT) { // Point is to the right of clip window
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else if (code_out & LEFT) { // Point is to the left of clip window
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }
            
            // Replace outside point with intersection point
            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    
    if (accept) {
        x1_clip = x1;
        y1_clip = y1;
        x2_clip = x2;
        y2_clip = y2;
        visible = true;
    } else {
        visible = false;
    }
}

/*
 * CONCEPT: Drawing Functions
 * -----------------------
 * These functions handle the visual representation of lines and the clipping window.
 * Original lines are drawn in red, clipped lines in blue, and the clipping window in green.
 */
void drawLine(int x1, int y1, int x2, int y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

void drawClippingWindow() {
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
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
 * CONCEPT: Display Function
 * ----------------------
 * This function is called by GLUT to render the scene. It draws:
 * 1. The coordinate axes
 * 2. The clipping window
 * 3. All original lines (in red)
 * 4. All clipped lines (in blue)
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawAxisLines();
    drawClippingWindow();
    
    // Draw all lines (original in red, clipped in blue)
    for (size_t i = 0; i < lines.size(); i += 2) {
        if (i + 1 < lines.size()) {
            int x1 = lines[i].x;
            int y1 = lines[i].y;
            int x2 = lines[i+1].x;
            int y2 = lines[i+1].y;
            
            // Draw original line
            drawLine(x1, y1, x2, y2, 1, 0, 0);
            
            // Clip and draw clipped line
            int x1_clip, y1_clip, x2_clip, y2_clip;
            bool visible;
            sutherlandLineClip(x1, y1, x2, y2, x1_clip, y1_clip, x2_clip, y2_clip, visible);
            
            if (visible) {
                drawLine(x1_clip, y1_clip, x2_clip, y2_clip, 0, 0, 1);
            }
        }
    }
    
    glFlush();
}

/*
 * CONCEPT: User Interaction
 * ----------------------
 * These functions handle mouse and keyboard input to:
 * 1. Create lines by clicking two points
 * 2. Adjust the clipping window size and position
 * 3. Clear all lines
 */
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int mouseX = x - 300;
        int mouseY = 300 - y;
        
        if (!drawingLine) {
            lines.push_back(Point(mouseX, mouseY));
            drawingLine = true;
        } else {
            lines.push_back(Point(mouseX, mouseY));
            drawingLine = false;
            glutPostRedisplay();
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'c':
            lines.clear();
            glutPostRedisplay();
            break;
        case 'w':
            ymax += 10;
            glutPostRedisplay();
            break;
        case 's':
            ymax -= 10;
            glutPostRedisplay();
            break;
        case 'a':
            xmin -= 10;
            glutPostRedisplay();
            break;
        case 'd':
            xmax += 10;
            glutPostRedisplay();
            break;
        case 'q':
            xmin += 10;
            glutPostRedisplay();
            break;
        case 'e':
            xmax -= 10;
            glutPostRedisplay();
            break;
        case 'z':
            ymin -= 10;
            glutPostRedisplay();
            break;
        case 'x':
            ymin += 10;
            glutPostRedisplay();
            break;
        case 'r':
            xmin = -100; ymin = -100;
            xmax = 100; ymax = 100;
            glutPostRedisplay();
            break;
    }
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
    glutCreateWindow("Practical 10 - Sutherland Line Clipping");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

// Controls:
// Left click twice to define a line
// 'c' - Clear all lines
// 'w/s/a/d/q/e/z/x' - Adjust clipping window
// 'r' - Reset clipping window