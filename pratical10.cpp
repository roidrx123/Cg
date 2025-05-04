#include <iostream>
#include <GL/glut.h>

int xmin = 50, ymin = 50, xmax = 400, ymax = 400;
float x01, y01, x02, y02;
int pt = 0;

void displayPoint(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void SimpleLine(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1, dy = y2 - y1;
    int steps = std::max(abs(dx), abs(dy));
    float Xinc = dx / steps, Yinc = dy / steps;

    for (int i = 0; i <= steps; ++i) {
        displayPoint(x1, y1);
        x1 += Xinc;
        y1 += Yinc;
    }
}

const int L = 1, R = 2, B = 4, T = 8;

int calCode(float x, float y) {
    int code = 0;
    if (x < xmin) code |= L;
    if (x > xmax) code |= R;
    if (y < ymin) code |= B;
    if (y > ymax) code |= T;
    return code;
}

void LineClip(float X1, float Y1, float X2, float Y2) {
    int outcode1 = calCode(X1, Y1), outcode2 = calCode(X2, Y2);

    while (outcode1 | outcode2) {
        if (outcode1 & outcode2) return;

        float x = 0, y = 0;
        int outcodeOut = outcode1 ? outcode1 : outcode2;
        float m = (X2 - X1) ? (Y2 - Y1) / (X2 - X1) : 0;

        if (outcodeOut & T) {
            y = ymax;
            x = X1 + (ymax - Y1) / m;
        } else if (outcodeOut & B) {
            y = ymin;
            x = X1 + (ymin - Y1) / m;
        } else if (outcodeOut & L) {
            x = xmin;
            y = Y1 + m * (xmin - X1);
        } else if (outcodeOut & R) {
            x = xmax;
            y = Y1 + m * (xmax - X1);
        }

        if (outcodeOut == outcode1) {
            X1 = x; Y1 = y; outcode1 = calCode(X1, Y1);
        } else {
            X2 = x; Y2 = y; outcode2 = calCode(X2, Y2);
        }
    }

    glClear(GL_COLOR_BUFFER_BIT); // Make sure the buffer is cleared
    glColor3f(1, 0, 0); // Set color for rectangle (window)
    SimpleLine(xmin, ymin, xmax, ymin);
    SimpleLine(xmax, ymin, xmax, ymax);
    SimpleLine(xmax, ymax, xmin, ymax);
    SimpleLine(xmin, ymax, xmin, ymin);

    glColor3f(0, 0, 1); // Set color for clipped line
    SimpleLine(X1, Y1, X2, Y2);
    glFlush();
}

void myMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        y = glutGet(GLUT_WINDOW_HEIGHT) - y;
        if (button == GLUT_LEFT_BUTTON) {
            if (pt == 0) {
                x01 = x;
                y01 = y;
                pt = 1;
            } else {
                x02 = x;
                y02 = y;
                glColor3f(0, 1, 0);
                SimpleLine(x01, y01, x02, y02);
                pt = 0;
            }
        } else if (button == GLUT_RIGHT_BUTTON) {
            glutKeyboardFunc([](unsigned char key, int, int) {
                if (key == 'c') LineClip(x01, y01, x02, y02);
            });
        }
    }
    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);  // Set background color to white
    glClear(GL_COLOR_BUFFER_BIT); // Clear screen to white initially
    gluOrtho2D(0, 600, 0, 600);
}

void drawWindow() {
    glColor3f(1, 0, 0); // Color of the window boundary (red)
    SimpleLine(xmin, ymin, xmax, ymin);
    SimpleLine(xmax, ymin, xmax, ymax);
    SimpleLine(xmax, ymax, xmin, ymax);
    SimpleLine(xmin, ymax, xmin, ymin);
    glutMouseFunc(myMouse);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cohen-Sutherland Line Clipping");
    init();
    glutDisplayFunc(drawWindow);
    glutMainLoop();
    return 0;
}
