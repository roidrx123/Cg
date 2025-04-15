/*
 * CONCEPT: Sutherland-Hodgeman Polygon Clipping
 * ------------------------------------------
 * This program implements the Sutherland-Hodgeman polygon clipping algorithm:
 * 1. The algorithm clips a polygon against a rectangular window
 * 2. It processes each edge of the clipping window one at a time
 * 3. For each edge, it determines which vertices are inside and which are outside
 * 4. It adds intersection points where polygon edges cross the clipping boundary
 */

#include <GL/glut.h>
#include <vector>
#include <cmath>
using namespace std;

struct Point {
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

vector<Point> polygonVertices;  // Original polygon vertices
vector<Point> clippedVertices;  // Clipped polygon vertices
bool isDrawing = false;         // Flag to track drawing state

// Clipping window boundaries
float xmin = -150, ymin = -150, xmax = 150, ymax = 150;
bool windowDefined = true;

/*
 * CONCEPT: Inside-Outside Test
 * -------------------------
 * These functions determine if a point is inside or outside a clipping edge:
 * - For left edge: point is inside if x >= xmin
 * - For right edge: point is inside if x <= xmax
 * - For bottom edge: point is inside if y >= ymin
 * - For top edge: point is inside if y <= ymax
 */
bool insideLeft(Point p) { return p.x >= xmin; }
bool insideRight(Point p) { return p.x <= xmax; }
bool insideBottom(Point p) { return p.y >= ymin; }
bool insideTop(Point p) { return p.y <= ymax; }

/*
 * CONCEPT: Line Intersection
 * -----------------------
 * These functions compute the intersection point of a line segment with a clipping edge:
 * - For vertical edges (left/right): interpolate y based on x
 * - For horizontal edges (top/bottom): interpolate x based on y
 */
Point computeIntersection(Point p1, Point p2, int edge) {
    Point intersection(0, 0);
    float m;
    
    if (p1.x != p2.x) {
        m = (p2.y - p1.y) / (p2.x - p1.x);
    }
    
    switch (edge) {
        case 0: // Left edge
            intersection.x = xmin;
            intersection.y = p1.y + (xmin - p1.x) * m;
            break;
        case 1: // Right edge
            intersection.x = xmax;
            intersection.y = p1.y + (xmax - p1.x) * m;
            break;
        case 2: // Bottom edge
            intersection.y = ymin;
            if (p1.x != p2.x) {
                intersection.x = p1.x + (ymin - p1.y) / m;
            } else {
                intersection.x = p1.x;
            }
            break;
        case 3: // Top edge
            intersection.y = ymax;
            if (p1.x != p2.x) {
                intersection.x = p1.x + (ymax - p1.y) / m;
            } else {
                intersection.x = p1.x;
            }
            break;
    }
    
    return intersection;
}

/*
 * CONCEPT: Sutherland-Hodgeman Algorithm
 * -----------------------------------
 * This function implements the main clipping algorithm:
 * 1. Start with the original polygon vertices
 * 2. For each edge of the clipping window:
 *    a. Process each edge of the polygon
 *    b. Add vertices that are inside the current clipping edge
 *    c. Add intersection points where edges cross the clipping boundary
 * 3. The result is the clipped polygon
 */
void clipPolygon() {
    if (polygonVertices.size() < 3) return;
    
    // Functions to check if a point is inside each edge
    bool (*inside[4])(Point) = { insideLeft, insideRight, insideBottom, insideTop };
    
    // Start with the original polygon
    clippedVertices = polygonVertices;
    
    // Process each edge of the clipping window
    for (int edge = 0; edge < 4; edge++) {
        vector<Point> inputVertices = clippedVertices;
        clippedVertices.clear();
        
        // No vertices to process
        if (inputVertices.empty()) break;
        
        Point s = inputVertices.back();  // Start with the last vertex
        
        // Process each edge of the polygon
        for (Point e : inputVertices) {
            if (inside[edge](e)) {  // End point is inside
                if (!inside[edge](s)) {  // Start point is outside
                    // Add intersection point
                    clippedVertices.push_back(computeIntersection(s, e, edge));
                }
                // Add the end point
                clippedVertices.push_back(e);
            } else if (inside[edge](s)) {  // End point is outside, start is inside
                // Add intersection point
                clippedVertices.push_back(computeIntersection(s, e, edge));
            }
            // Update start point for next edge
            s = e;
        }
    }
}

/*
 * CONCEPT: Drawing Functions
 * -----------------------
 * These functions handle the visual representation of:
 * 1. The original polygon (in red)
 * 2. The clipped polygon (in blue)
 * 3. The clipping window (in green)
 */
void drawPolygon(const vector<Point>& vertices, float r, float g, float b) {
    if (vertices.size() < 3) return;
    
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (const Point& p : vertices) {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawClippingWindow() {
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
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
 * Mouse clicks are used to define the polygon:
 * - Left clicks add vertices to the polygon
 * - Right-click completes the polygon and performs clipping
 */
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mouseX = x - 300;
        float mouseY = 300 - y;
        
        if (!isDrawing) {
            polygonVertices.clear();
            clippedVertices.clear();
            isDrawing = true;
        }
        
        polygonVertices.push_back(Point(mouseX, mouseY));
        glutPostRedisplay();
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (isDrawing && polygonVertices.size() >= 3) {
            isDrawing = false;
            clipPolygon();
            glutPostRedisplay();
        }
    }
}

/*
 * CONCEPT: Keyboard Controls
 * -----------------------
 * Keyboard input allows:
 * - Adjusting the clipping window size and position
 * - Clearing the polygon
 * - Resetting the clipping window
 */
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        // Adjust clipping window
        case 'w': ymax += 10; break;  // Increase top
        case 's': ymin -= 10; break;  // Decrease bottom
        case 'a': xmin -= 10; break;  // Decrease left
        case 'd': xmax += 10; break;  // Increase right
        case 'q': xmin += 10; break;  // Increase left
        case 'e': xmax -= 10; break;  // Decrease right
        case 'z': ymin += 10; break;  // Increase bottom
        case 'x': ymax -= 10; break;  // Decrease top
        
        // Reset clipping window
        case 'r':
            xmin = -150; ymin = -150;
            xmax = 150; ymax = 150;
            break;
        
        // Clear polygon
        case 'c':
            polygonVertices.clear();
            clippedVertices.clear();
            isDrawing = false;
            break;
            
        // Perform clipping
        case ' ':
            if (polygonVertices.size() >= 3) {
                clipPolygon();
            }
            break;
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawAxisLines();
    drawClippingWindow();
    
    // Draw original polygon in red
    drawPolygon(polygonVertices, 1, 0, 0);
    
    // Draw clipped polygon in blue
    drawPolygon(clippedVertices, 0, 0, 1);
    
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-300, 300, -300, 300);
    glPointSize(2);
    glLineWidth(2);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Practical 11 - Sutherland-Hodgeman Polygon Clipping");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

// Controls:
// Left click to add polygon vertices
// Right click to complete polygon and perform clipping
// w/s/a/d/q/e/z/x - Adjust clipping window
// r - Reset clipping window
// c - Clear polygon
// Space - Perform clipping