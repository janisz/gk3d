#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <glm.hpp>
#include <tiny_obj_loader.h>

#include "Camera.h"

using namespace std;

void Display();

void Reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);

void KeyboardUp(unsigned char key, int x, int y);

void MouseMotion(int x, int y);

void Mouse(int button, int state, int x, int y);

void Timer(int value);

void Idle();

void Grid();

Camera g_camera;
bool g_key[256];
bool g_shift_down = false;
bool g_fps_mode = false;
int g_viewport_width = 0;
int g_viewport_height = 0;
bool g_mouse_left_down = false;
bool g_mouse_right_down = false;

// Movement settings
const float g_translation_speed = 0.2;
const float g_rotation_speed = M_PI / 180 * 0.1;

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GK3D");
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    glutIgnoreKeyRepeat(1);

    glutDisplayFunc(Display);
    glutIdleFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MouseMotion);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutIdleFunc(Idle);

    glutTimerFunc(1, Timer, 0);
    glutMainLoop();

    return 0;
}

void Grid() {
    glPushMatrix();
    glColor3f(1, 1, 1);

    for (int i = -50; i < 50; i++) {
        glBegin(GL_LINES);
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
        glEnd();
    }

    glPopMatrix();
}

void Hall() {
    glPushMatrix();
    glColor3f(1, 1, 1);

    //FRONT
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(20.0, 0.0, -10.0);
    glVertex3f(20.0, 15.0, -10.0);
    glVertex3f(-10.0, 15.0, -10.0);
    glVertex3f(-10.0, 0.0, -10.0);

    glEnd();

    //BACK
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(20.0, 0.0, 10.0);
    glVertex3f(20.0, 15.0, 10.0);
    glVertex3f(-10.0, 15.0, 10.0);
    glVertex3f(-10.0, 0.0, 10.0);
    glEnd();

    //RIGHT
    glBegin(GL_POLYGON);
    glColor3f(0.75, 0.75, 0.75);
    glVertex3f(20.0, 0.0, -10.0);
    glVertex3f(20.0, 15.0, -10.0);
    glVertex3f(20.0, 15.0, 10.0);
    glVertex3f(20.0, 0.0, 10.0);
    glEnd();

    //LEFT
    glBegin(GL_POLYGON);
    glColor3f(0.75, 0.75, 0.75);
    glVertex3f(-10.0, 0.0, 10.0);
    glVertex3f(-10.0, 15.0, 10.0);
    glVertex3f(-10.0, 15.0, -10.0);
    glVertex3f(-10.0, 0.0, -10.0);
    glEnd();

    //TOP
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(20.0, 15.0, 10.0);
    glVertex3f(20.0, 15.0, -10.0);
    glVertex3f(-10.0, 15.0, -10.0);
    glVertex3f(-10.0, 15.0, 10.0);
    glEnd();

    //BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(20.0, 0.0, -10.0);
    glVertex3f(20.0, 0.0, 10.0);
    glVertex3f(-10.0, 0.0, 10.0);
    glVertex3f(-10.0, 0.0, -10.0);
    glEnd();

    //PITCH
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    glVertex3f(10.0, 0.001, -4.0);
    glVertex3f(10.0, 0.001, 4.0);
    glVertex3f(0.0, 0.001, 4.0);
    glVertex3f(0.0, 0.001, -4.0);
    glEnd();

    glPopMatrix();
}

void Net() {
    glPushMatrix();

    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(5, 1, 3);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(0.05, 1, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(5, 1, -3);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(0.05, 1, 10, 10);

    glPopMatrix();

    glPushMatrix();

    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glVertex3f(5, 0.5, -3);
    glVertex3f(5, 1, -3);
    glVertex3f(5, 1, 3);
    glVertex3f(5, 0.5, 3);
    glEnd();

    glPopMatrix();
}

void Display(void) {
    glClearColor(0.0, 0.0, 0.0, 10.0); //clear the screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();

    g_camera.Refresh();

    glColor3f(0, 1, 0);

    Grid();
    Hall();
    Net();

    glutSwapBuffers(); //swap the buffers
}

void Reshape(int w, int h) {
    g_viewport_width = w;
    g_viewport_height = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h); //set the viewport to the current window specifications
    glMatrixMode(GL_PROJECTION); //set the matrix to projection

    glLoadIdentity();
    gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.1, 100.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }

    if (key == ' ') {
        g_fps_mode = !g_fps_mode;

        if (g_fps_mode) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(g_viewport_width / 2, g_viewport_height / 2);
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    g_shift_down = glutGetModifiers() == GLUT_ACTIVE_SHIFT;

    g_key[key] = true;
}

void KeyboardUp(unsigned char key, int x, int y) {
    g_key[key] = false;
}

void Timer(int value) {
    if (g_fps_mode) {
        if (g_key['w'] || g_key['W']) {
            g_camera.Move(g_translation_speed);
        }
        else if (g_key['s'] || g_key['S']) {
            g_camera.Move(-g_translation_speed);
        }
        else if (g_key['a'] || g_key['A']) {
            g_camera.Strafe(g_translation_speed);
        }
        else if (g_key['d'] || g_key['D']) {
            g_camera.Strafe(-g_translation_speed);
        }
        else if (g_mouse_left_down) {
            g_camera.Fly(-g_translation_speed);
        }
        else if (g_mouse_right_down) {
            g_camera.Fly(g_translation_speed);
        }
    }

    glutTimerFunc(1, Timer, 0);
}

void Idle() {
    Display();
}

void Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            g_mouse_left_down = true;
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            g_mouse_right_down = true;
        }
    }
    else if (state == GLUT_UP) {
        if (button == GLUT_LEFT_BUTTON) {
            g_mouse_left_down = false;
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            g_mouse_right_down = false;
        }
    }
}

void MouseMotion(int x, int y) {
    // This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
    // This avoids it being called recursively and hanging up the event loop
    static bool just_warped = false;

    if (just_warped) {
        just_warped = false;
        return;
    }

    if (g_fps_mode) {
        int dx = x - g_viewport_width / 2;
        int dy = y - g_viewport_height / 2;

        if (dx) {
            g_camera.RotateYaw(g_rotation_speed * dx);
        }

        if (dy) {
            g_camera.RotatePitch(-g_rotation_speed * dy);
        }

        glutWarpPointer(g_viewport_width / 2, g_viewport_height / 2);

        just_warped = true;
    }
}