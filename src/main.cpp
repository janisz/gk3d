#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "Fog.h"

void Display();

void Reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);

void KeyboardUp(unsigned char key, int x, int y);

void MouseMotion(int x, int y);

void Mouse(int button, int state, int x, int y);

void Timer(int value);

void Idle();

Camera g_camera;
bool g_key[256];
bool g_shift_down = false;

bool g_fps_mode = false;
bool g_fog_mode = false;

int g_viewport_width = 0;
int g_viewport_height = 0;

bool g_mouse_left_down = false;
bool g_mouse_right_down = false;

// Movement settings
const float g_translation_speed = 0.2;
const float g_rotation_speed = M_PI / 180 * 0.1;

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage:  " << argv[0] << "input0.obj input1.obj" << std::endl;
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        LoadObj(argv[i]);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GK3D");
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);

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

void Display(void) {
    glClearColor(0.0, 0.0, 0.0, 10.0); //clear the screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();

    g_camera.Refresh();

    glColor3f(0, 1, 0);

    Grid();
    Hall();
    Net();
    People();
    Light();
    Fog();


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
            g_fog_mode = false;
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(g_viewport_width / 2, g_viewport_height / 2);
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }
    if (toupper(key) == 'F') {
        g_fog_mode = true;
        g_fps_mode = false;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }

    g_shift_down = glutGetModifiers() == GLUT_ACTIVE_SHIFT;

    g_key[key] = true;
}

void KeyboardUp(unsigned char key, int x, int y) {
    g_key[key] = false;
}

void configureCameraGlobals() {
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

void Timer(int value) {
    if (g_fps_mode) {
        configureCameraGlobals();
    }
    if (g_fog_mode) {
        ConfigureFogGlobals();
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