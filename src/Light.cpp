#include <GL/glut.h>
#include "Light.h"

float spotLightBlueValue = 0.0;

void SpotLight() {
    GLfloat specular[] = {1.0, 0, 0, 1.0};
//    glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
    spotLightBlueValue += 0.01;
    spotLightBlueValue = spotLightBlueValue > 1 ? 0 : spotLightBlueValue;
    GLfloat ambient[] = {0, 0, spotLightBlueValue, 1.0f};
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
    GLfloat diffuse[] = {0.5, 0.5, 0.5, 1.0};
//    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
    GLfloat light_position[] = {3, 1, -4, 1.0};
    glLightfv(GL_LIGHT2, GL_POSITION, light_position);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, .5);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, .2);
}

void Light0() {
    GLfloat specular[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    GLfloat diffuse[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    GLfloat light_position[] = {19, 14.0, -10, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 5);
    GLfloat light_direction[] = {-19, -14, 10};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
}

void Light1() {
    GLfloat specular[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    GLfloat diffuse[] = {0.5, 0.5, 0.2, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    GLfloat light_position[] = {-10, 14.0, 10, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 5);
    GLfloat light_direction[] = {10, -14, -10};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
}

void Light() {
    GLfloat mat_ambient[4] = {0.247250, 0.224500, 0.064500, 1.000000};
    GLfloat mat_diffuse[4] = {0.346150, 0.314300, 0.090300, 1.000000};
    GLfloat mat_specular[4] = {0.797357, 0.723991, 0.208006, 1.000000};
    GLfloat mat_shininess = 83.2;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    Light0();
    Light1();
    SpotLight();

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_DEPTH_TEST);   // Hidden surface removal
    glFrontFace(GL_CCW);       // Counterclockwise polygons face out
    glEnable(GL_CULL_FACE);    // Do not try to display the back sides
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}