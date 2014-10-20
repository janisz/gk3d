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
    model_t model;
    model.materials = materials;
    model.shapes = shapes;
    models.push_back(model);
    if (!err.empty()) {
        std::cerr << err << std::endl;
        exit(1);
    }

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
        printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
        printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3 * f + 0], shapes[i].mesh.indices[3 * f + 1], shapes[i].mesh.indices[3 * f + 2], shapes[i].mesh.material_ids[f]);
        }

        printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
                    shapes[i].mesh.positions[3 * v + 0],
                    shapes[i].mesh.positions[3 * v + 1],
                    shapes[i].mesh.positions[3 * v + 2]);
        }
    }

    for (size_t i = 0; i < materials.size(); i++) {
        printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
        printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
        printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
        printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
        printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
        printf("  material.Ns = %f\n", materials[i].shininess);
        printf("  material.Ni = %f\n", materials[i].ior);
        printf("  material.dissolve = %f\n", materials[i].dissolve);
        printf("  material.illum = %d\n", materials[i].illum);
        printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
        printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
        std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
        std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
        for (; it != itEnd; it++) {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }

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
            glVertex3f(shapes[i].mesh.positions[indices[f] * 3 + 0],
                    shapes[i].mesh.positions[indices[f] * 3 + 1],
                    shapes[i].mesh.positions[indices[f] * 3 + 2]);
        }
        glEnd();
    }
}

void Light() {
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
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