#include <GL/glut.h>
#include <cmath>

// Initial Depth
int bezierDepth = 3;
int kochDepth = 3;

// Control points for Bezier
float bezierPoints[4][2] = {
    {100, 100},
    {150, 300},
    {250, 300},
    {300, 100}
};

// ---------- Utility ----------

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

// ---------- 1. Bezier Fractal ----------

void drawBezier(float t, float& x, float& y, float ctrl[4][2]) {
    float u = 1 - t;
    x = pow(u, 3) * ctrl[0][0] + 3 * t * pow(u, 2) * ctrl[1][0] +
        3 * pow(t, 2) * u * ctrl[2][0] + pow(t, 3) * ctrl[3][0];
    y = pow(u, 3) * ctrl[0][1] + 3 * t * pow(u, 2) * ctrl[1][1] +
        3 * pow(t, 2) * u * ctrl[2][1] + pow(t, 3) * ctrl[3][1];
}

void drawBezierFractal(float ctrl[4][2], int depth) {
    if (depth == 0) {
        float prevX = ctrl[0][0], prevY = ctrl[0][1];
        for (float t = 0.01f; t <= 1.0f; t += 0.01f) {
            float x, y;
            drawBezier(t, x, y, ctrl);
            drawLine(prevX, prevY, x, y);
            prevX = x;
            prevY = y;
        }
        return;
    }

    float left[4][2], right[4][2];
    float mid[7][2]; // To store midpoints

    mid[0][0] = (ctrl[0][0] + ctrl[1][0]) / 2;
    mid[0][1] = (ctrl[0][1] + ctrl[1][1]) / 2;

    mid[1][0] = (ctrl[1][0] + ctrl[2][0]) / 2;
    mid[1][1] = (ctrl[1][1] + ctrl[2][1]) / 2;

    mid[2][0] = (ctrl[2][0] + ctrl[3][0]) / 2;
    mid[2][1] = (ctrl[2][1] + ctrl[3][1]) / 2;

    mid[3][0] = (mid[0][0] + mid[1][0]) / 2;
    mid[3][1] = (mid[0][1] + mid[1][1]) / 2;

    mid[4][0] = (mid[1][0] + mid[2][0]) / 2;
    mid[4][1] = (mid[1][1] + mid[2][1]) / 2;

    mid[5][0] = (mid[3][0] + mid[4][0]) / 2;
    mid[5][1] = (mid[3][1] + mid[4][1]) / 2;

    // Left and right curves
    left[0][0] = ctrl[0][0];         left[0][1] = ctrl[0][1];
    left[1][0] = mid[0][0];          left[1][1] = mid[0][1];
    left[2][0] = mid[3][0];          left[2][1] = mid[3][1];
    left[3][0] = mid[5][0];          left[3][1] = mid[5][1];

    right[0][0] = mid[5][0];         right[0][1] = mid[5][1];
    right[1][0] = mid[4][0];         right[1][1] = mid[4][1];
    right[2][0] = mid[2][0];         right[2][1] = mid[2][1];
    right[3][0] = ctrl[3][0];        right[3][1] = ctrl[3][1];

    drawBezierFractal(left, depth - 1);
    drawBezierFractal(right, depth - 1);
}

// ---------- 2. Koch Curve ----------

void drawKoch(float x1, float y1, float x2, float y2, int depth) {
    if (depth == 0) {
        drawLine(x1, y1, x2, y2);
        return;
    }

    float dx = (x2 - x1) / 3.0f;
    float dy = (y2 - y1) / 3.0f;

    float xA = x1 + dx;
    float yA = y1 + dy;

    float xB = x1 + 2 * dx;
    float yB = y1 + 2 * dy;

    // Peak of the triangle
    float angle = M_PI / 3.0f; // 60 degrees
    float xPeak = xA + cos(angle) * (xB - xA) - sin(angle) * (yB - yA);
    float yPeak = yA + sin(angle) * (xB - xA) + cos(angle) * (yB - yA);

    drawKoch(x1, y1, xA, yA, depth - 1);
    drawKoch(xA, yA, xPeak, yPeak, depth - 1);
    drawKoch(xPeak, yPeak, xB, yB, depth - 1);
    drawKoch(xB, yB, x2, y2, depth - 1);
}

// ---------- Display & Keyboard ----------

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 0, 0); // Red for Bezier
    drawBezierFractal(bezierPoints, bezierDepth);

    glColor3f(0, 0, 1); // Blue for Koch
    drawKoch(100, 400, 500, 400, kochDepth);

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 600, 0, 600);
}

void keyboard(unsigned char key, int, int) {
    if (key == 'w') bezierDepth++;
    else if (key == 's' && bezierDepth > 0) bezierDepth--;
    else if (key == 'e') kochDepth++;
    else if (key == 'd' && kochDepth > 0) kochDepth--;
    glutPostRedisplay();
}

// ---------- Main ----------

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Bezier & Koch Fractal (Press W/S for Bezier, E/D for Koch)");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
