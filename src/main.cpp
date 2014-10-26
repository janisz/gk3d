#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <tiny_obj_loader.h>
#include <cstdio>

#define GLM_COMPILER 0

#include <glm.hpp>

#include "Camera.h"

void Display();

void Reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);

void KeyboardUp(unsigned char key, int x, int y);

void MouseMotion(int x, int y);

void Mouse(int button, int state, int x, int y);

void Timer(int value);

void Idle();

void Grid();

void LoadObj(const char *filename);

void loadFromMesh(std::vector<tinyobj::shape_t> shapes);

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

typedef struct {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
} model_t;

std::vector<model_t> models;

void Light() {
    const GLfloat mat_ambient[4] = {0.247250, 0.224500, 0.064500, 1.000000};
    const GLfloat mat_diffuse[4] = {0.346150, 0.314300, 0.090300, 1.000000};
    const GLfloat mat_specular[4] = {0.797357, 0.723991, 0.208006, 1.000000};
    const GLfloat mat_shininess = 83.2;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    GLfloat ambient[] = {1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    GLfloat light_position[] = {0, 15.0, 0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15);
    GLfloat light_direction[] = {0, -1, 0};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);   // Hidden surface removal
    glFrontFace(GL_CCW);       // Counterclockwise polygons face out
    glEnable(GL_CULL_FACE);    // Do not try to display the back sides
}

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

    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);

    Light();

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

void LoadObj(const char *filename) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err = tinyobj::LoadObj(shapes, materials, filename);
    if (!err.empty()) {
        std::cerr << err << std::endl;
        exit(1);
    }
    model_t model;
    model.materials = materials;
    model.shapes = shapes;
    models.push_back(model);
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
    glNormal3f(0, 0, 1);
    glVertex3f(20.0, 0.0, -10.0);
    glNormal3f(0, 0, 1);
    glVertex3f(20.0, 15.0, -10.0);
    glNormal3f(0, 0, 1);
    glVertex3f(-10.0, 15.0, -10.0);
    glNormal3f(0, 0, 1);
    glVertex3f(-10.0, 0.0, -10.0);

    glEnd();

    //BACK
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5);
    glNormal3f(0, 0, -1);
    glVertex3f(-10.0, 0.0, 10.0);
    glNormal3f(0, 0, -1);
    glVertex3f(-10.0, 15.0, 10.0);
    glNormal3f(0, 0, -1);
    glVertex3f(20.0, 15.0, 10.0);
    glNormal3f(0, 0, -1);
    glVertex3f(20.0, 0.0, 10.0);
    glEnd();

    //RIGHT
    glBegin(GL_POLYGON);
    glColor3f(0.75, 0.75, 0.75);
    glNormal3f(-1, 0, 0);
    glVertex3f(20.0, 15.0, -10.0);
    glNormal3f(-1, 0, 0);
    glVertex3f(20.0, 0.0, -10.0);
    glNormal3f(-1, 0, 0);
    glVertex3f(20.0, 0.0, 10.0);
    glNormal3f(-1, 0, 0);
    glVertex3f(20.0, 15.0, 10.0);
    glEnd();

    //LEFT
    glBegin(GL_POLYGON);
    glColor3f(0.75, 0.75, 0.75);
    glNormal3f(1, 0, 0);
    glVertex3f(-10.0, 0.0, 10.0);
    glNormal3f(1, 0, 0);
    glVertex3f(-10.0, 0.0, -10.0);
    glNormal3f(1, 0, 0);
    glVertex3f(-10.0, 15.0, -10.0);
    glNormal3f(1, 0, 0);
    glVertex3f(-10.0, 15.0, 10.0);
    glEnd();

    //TOP
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glNormal3f(0, -1, 0);
    glVertex3f(20.0, 15.0, -10.0);
    glNormal3f(0, -1, 0);
    glVertex3f(20.0, 15.0, 10.0);
    glNormal3f(0, -1, 0);
    glVertex3f(-10.0, 15.0, 10.0);
    glNormal3f(0, -1, 0);
    glVertex3f(-10.0, 15.0, -10.0);
    glEnd();

    //BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glNormal3f(0, 1, 0);
    glVertex3f(-10.0, 0.0, 10.0);
    glNormal3f(0, 1, 0);
    glVertex3f(20.0, 0.0, 10.0);
    glNormal3f(0, 1, 0);
    glVertex3f(20.0, 0.0, -10.0);
    glNormal3f(0, 1, 0);
    glVertex3f(-10.0, 0.0, -10.0);
    glEnd();

    //PITCH
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.5, 0.0);
    glNormal3f(0, 1, 0);
    glVertex3f(0.0, 0.0001, -4.0);
    glNormal3f(0, 1, 0);
    glVertex3f(0.0, 0.0001, 4.0);
    glNormal3f(0, 1, 0);
    glVertex3f(10.0, 0.0001, 4.0);
    glNormal3f(0, 1, 0);
    glVertex3f(10.0, 0.0001, -4.0);
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
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glVertex3f(5, 0.5, 3);
    glVertex3f(5, 1, 3);
    glVertex3f(5, 1, -3);
    glVertex3f(5, 0.5, -3);
    glEnd();

    glPopMatrix();
}

void People() {
    GLuint bench = glGenLists(1);
    glNewList(bench, GL_COMPILE);
    glScalef(0.008, 0.008, 0.008);
    glTranslatef(0, 13, 0);
    glColor3f(0.5, 0.5, 0.5);
    loadFromMesh(models[0].shapes);
    glEndList();

    glPushMatrix();
    glTranslatef(4, 0, -2);
    glRotatef(120, 0, 1.0, 0);
    glScalef(0.01, 0.01, 0.01);
    glTranslatef(0, 45, 0);
    glColor3f(1, 0.9, 0.8);
    loadFromMesh(models[1].shapes);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, 0, -4);
    glCallList(bench);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6, 0, -4);
    glCallList(bench);
    glPopMatrix();

    glDeleteLists(bench, 1);
}

void loadFromMesh(std::vector<tinyobj::shape_t> shapes) {
    for (size_t i = 0; i < shapes.size(); ++i) {
        glBegin(GL_TRIANGLES);
        std::vector<unsigned int> indices = shapes[i].mesh.indices;
        for (size_t f = 0; f < indices.size(); ++f) {
            glNormal3f(shapes[i].mesh.normals[indices[f] * 3 + 0],
                    shapes[i].mesh.normals[indices[f] * 3 + 1],
                    shapes[i].mesh.normals[indices[f] * 3 + 2]);
            glVertex3f(shapes[i].mesh.positions[indices[f] * 3 + 0],
                    shapes[i].mesh.positions[indices[f] * 3 + 1],
                    shapes[i].mesh.positions[indices[f] * 3 + 2]);
        }
        glEnd();
    }
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