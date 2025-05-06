#include <GL/glut.h>
#include <cmath>
#include <vector>

// Control points for Bezier
float bezierPoints[4][2] = {
    {100, 100},
    {150, 300},
    {250, 300},
    {300, 100}
};

// Utility to draw a line
void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

// 1) ---------------- BEZIER FRACTAL ----------------

void drawBezier(float t, float &x, float &y, float ctrl[4][2]) {
    float u = 1 - t;
    x = pow(u, 3) * ctrl[0][0] + 3 * t * pow(u, 2) * ctrl[1][0] +
        3 * pow(t, 2) * u * ctrl[2][0] + pow(t, 3) * ctrl[3][0];
    y = pow(u, 3) * ctrl[0][1] + 3 * t * pow(u, 2) * ctrl[1][1] +
        3 * pow(t, 2) * u * ctrl[2][1] + pow(t, 3) * ctrl[3][1];
}

void drawBezierFractal(float ctrl[4][2], int depth) {
    if (depth == 0) {
        float prevX = ctrl[0][0];
        float prevY = ctrl[0][1];
        for (float t = 0.01; t <= 1.0; t += 0.01) {
            float x, y;
            drawBezier(t, x, y, ctrl);
            drawLine(prevX, prevY, x, y);
            prevX = x;
            prevY = y;
        }
    } else {
        // Subdivide the Bezier curve
        float left[4][2], right[4][2];

        left[0][0] = ctrl[0][0];
        left[0][1] = ctrl[0][1];

        left[1][0] = (ctrl[0][0] + ctrl[1][0]) / 2;
        left[1][1] = (ctrl[0][1] + ctrl[1][1]) / 2;

        float m1x = (ctrl[1][0] + ctrl[2][0]) / 2;
        float m1y = (ctrl[1][1] + ctrl[2][1]) / 2;

        left[2][0] = (left[1][0] + m1x) / 2;
        left[2][1] = (left[1][1] + m1y) / 2;

        float m2x = (ctrl[2][0] + ctrl[3][0]) / 2;
        float m2y = (ctrl[2][1] + ctrl[3][1]) / 2;

        right[3][0] = ctrl[3][0];
        right[3][1] = ctrl[3][1];

        right[2][0] = (ctrl[2][0] + ctrl[3][0]) / 2;
        right[2][1] = (ctrl[2][1] + ctrl[3][1]) / 2;

        right[1][0] = (m1x + m2x) / 2;
        right[1][1] = (m1y + m2y) / 2;

        left[3][0] = (left[2][0] + right[1][0]) / 2;
        left[3][1] = (left[2][1] + right[1][1]) / 2;

        right[0][0] = left[3][0];
        right[0][1] = left[3][1];

        drawBezierFractal(left, depth - 1);
        drawBezierFractal(right, depth - 1);
    }
}

// 2) ---------------- KOCH CURVE FRACTAL ----------------

void drawKoch(float x1, float y1, float x2, float y2, int depth) {
    if (depth == 0) {
        drawLine(x1, y1, x2, y2);
        return;
    }

    float dx = (x2 - x1) / 3.0;
    float dy = (y2 - y1) / 3.0;

    float xA = x1 + dx;
    float yA = y1 + dy;

    float xB = x1 + 2 * dx;
    float yB = y1 + 2 * dy;

    float xM = (x1 + x2) / 2.0;
    float yM = (y1 + y2) / 2.0;

    float px = x1 + dx * 0.5 - sqrt(3) * dy / 2.0;
    float py = y1 + dy * 0.5 + sqrt(3) * dx / 2.0;

    drawKoch(x1, y1, xA, yA, depth - 1);
    drawKoch(xA, yA, px, py, depth - 1);
    drawKoch(px, py, xB, yB, depth - 1);
    drawKoch(xB, yB, x2, y2, depth - 1);
}

// ----------- DISPLAY -----------

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 0, 0); // Red
    drawBezierFractal(bezierPoints, 3);

    glColor3f(0, 0, 1); // Blue
    drawKoch(100, 400, 500, 400, 4);  // Start with depth 4

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, 600, 0, 600);
}

// ----------- MAIN -----------

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Fractal Patterns: Bezier and Koch Curve");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
