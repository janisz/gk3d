#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <GL/gl.h>

#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "Fog.h"
#include "Texture.h"
#include "Shader.h"

void Display();

void Reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);

void KeyboardUp(unsigned char key, int x, int y);

void MouseMotion(int x, int y);

void Mouse(int button, int state, int x, int y);

void Timer(int value);

void Idle();

void drawScene();

Camera g_camera;
bool g_key[256];
bool g_shift_down = false;

bool g_fps_mode = false;
bool g_fog_mode = false;
bool g_clip_mode = false;

int g_viewport_width = 0;
int g_viewport_height = 0;

bool g_mouse_left_down = false;
bool g_mouse_right_down = false;

// Movement settings
const float g_translation_speed = 0.2;
const float g_rotation_speed = M_PI / 180 * 0.1;

float g_clip_x = 1.0;

GLuint g_texture_wood;
GLuint g_texture_check;
GLuint g_texture_net;


int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GK3D");
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

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

    LoadObj("./resources/models/bench.obj");
    LoadObj("./resources/models/female.obj");

    g_texture_check = LoadTexture("./resources/textures/check.png");
    g_texture_wood = LoadTexture("./resources/textures/wood.png");
    g_texture_net = LoadTexture("./resources/textures/net.png");

    GLuint program = LoadShader("./src/SpotLight.vert", "./src/SpotLight.frag");
    glUseProgram(program);

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

    GLdouble plane_eq0[4] = {g_clip_x, 0.0, 1.0, 0.0};
    GLdouble plane_eq1[4] = {-g_clip_x, 0.0, -1.0, 0.0};

    glClipPlane(GL_CLIP_PLANE0, plane_eq0);
    glClipPlane(GL_CLIP_PLANE1, plane_eq1);

    /* Draw reflected scene first */

    glPushMatrix ();
    /* Mirrors lies in YZ plane, so scale by -1.0 in X axis */
    glScalef (-1., 1., 1.);
    /* Mirror is 2.0 units from origin, so translate by 4.0 */
    glTranslatef (4., 0., 0.);
    drawScene();
    glPopMatrix ();

    /* draw mirror */
    glClear (GL_DEPTH_BUFFER_BIT);
    glPushAttrib (0xffffffff);
    glDisable (GL_LIGHTING);
    /* Create imperfect reflector effect by blending a black
       mirror over the reflected scene with alpha of 0.05 */
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f (0., 0., 0., 0.05);
    glBegin (GL_QUADS);
    glVertex3f (-2., 1., 3.);
    glVertex3f (-2., -1., 3.);
    glVertex3f (-2., -1., -2.5);
    glVertex3f (-2., 1., -2.5);
    glEnd ();
    glPopAttrib();

    /* Draw the real scene */
    drawScene();


    glutSwapBuffers(); //swap the buffers
}

void drawScene() {
    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // włączenie płaszczyzny obcinającej
        glEnable((GLenum) (GL_CLIP_PLANE0 + i));
        // wyłączenie płaszczyzny obcinającej
        glDisable((GLenum) (GL_CLIP_PLANE1 - i));

        Grid();
        Hall();
        Net();
        People();
        Light();
        Fog();
    }
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
            g_clip_mode = false;
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
        g_clip_mode = false;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
    if (toupper(key) == 'C') {
        g_clip_mode = true;
        g_fog_mode = false;
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

void configureClipGlobals() {
    if (g_key['w'] || g_key['W']) {
        g_clip_x += 0.1;
    }
    else if (g_key['s'] || g_key['S']) {
        g_clip_x -= 0.1;
    }
}

void Timer(int value) {
    if (g_fps_mode) {
        configureCameraGlobals();
    }
    else if (g_fog_mode) {
        ConfigureFogGlobals();
    }
    else if (g_clip_mode) {
        configureClipGlobals();
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