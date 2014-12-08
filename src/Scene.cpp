#include <cmath>
#include <cstdio>
#include <iostream>
#include <GL/freeglut.h>

#include "Scene.h"

std::vector<model_t> models;

extern GLuint g_texture_wood;
extern GLuint g_texture_check;
extern GLuint g_texture_net;

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

void loadFromMesh(std::vector<tinyobj::shape_t> shapes) {
    for (size_t i = 0; i < shapes.size(); ++i) {
        glBegin(GL_TRIANGLES);
        tinyobj::mesh_t mesh = shapes[i].mesh;
        std::vector<unsigned int> indices = mesh.indices;
        for (size_t f = 0; f < indices.size(); ++f) {
            glNormal3f(mesh.normals[indices[f] * 3 + 0],
                    mesh.normals[indices[f] * 3 + 1],
                    mesh.normals[indices[f] * 3 + 2]);
            glVertex3f(mesh.positions[indices[f] * 3 + 0],
                    mesh.positions[indices[f] * 3 + 1],
                    mesh.positions[indices[f] * 3 + 2]);
            if (mesh.texcoords.size() > 0) {
                glTexCoord3f(mesh.texcoords[indices[f] * 3 + 0],
                        mesh.texcoords[indices[f] * 3 + 1],
                        mesh.texcoords[indices[f] * 3 + 2]);
            }
        }
        glEnd();
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
    glNormal3f(0, 0, 1);
    glVertex3f(20.0, 0.0, -10.0);
    glVertex3f(20.0, 15.0, -10.0);
    glVertex3f(-10.0, 15.0, -10.0);
    glVertex3f(-10.0, 0.0, -10.0);

    glEnd();

    //BACK
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5);
    glNormal3f(0, 0, -1);
    glVertex3f(-10.0, 0.0, 10.0);
    glVertex3f(-10.0, 15.0, 10.0);
    glVertex3f(20.0, 15.0, 10.0);
    glVertex3f(20.0, 0.0, 10.0);
    glEnd();

    //RIGHT
    glBegin(GL_POLYGON);
    glColor3f(0.75, 0.75, 0.75);
    glNormal3f(-1, 0, 0);
    glVertex3f(20.0, 15.0, -10.0);
    glVertex3f(20.0, 0.0, -10.0);
    glVertex3f(20.0, 0.0, 10.0);
    glVertex3f(20.0, 15.0, 10.0);
    glEnd();

    //LEFT
    glBegin(GL_POLYGON);
    glColor3f(0.75, 0.75, 0.75);
    glNormal3f(1, 0, 0);
    glVertex3f(-10.0, 0.0, 10.0);
    glVertex3f(-10.0, 0.0, -10.0);
    glVertex3f(-10.0, 15.0, -10.0);
    glVertex3f(-10.0, 15.0, 10.0);
    glEnd();

    //TOP
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glNormal3f(0, -1, 0);
    glVertex3f(20.0, 15.0, -10.0);
    glVertex3f(20.0, 15.0, 10.0);
    glVertex3f(-10.0, 15.0, 10.0);
    glVertex3f(-10.0, 15.0, -10.0);
    glEnd();

    //BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glNormal3f(0, 1, 0);
    glVertex3f(-10.0, 0.0, 10.0);
    glVertex3f(20.0, 0.0, 10.0);
    glVertex3f(20.0, 0.0, -10.0);
    glVertex3f(-10.0, 0.0, -10.0);
    glEnd();

    //PITCH
    glBindTexture(GL_TEXTURE_2D, g_texture_wood);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0, 0.0001, -4.0);
    glTexCoord2f(20.0f, 0.0f);
    glVertex3f(0.0, 0.0001, 4.0);
    glTexCoord2f(20.0f, 20.0f);
    glVertex3f(10.0, 0.0001, 4.0);
    glTexCoord2f(0.0f, 20.0f);
    glVertex3f(10.0, 0.0001, -4.0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

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

    glBindTexture(GL_TEXTURE_2D, g_texture_net);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(5, 0.5, -3);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(5, 1, -3);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(5, 1, 3);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(5, 0.5, 3);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(5, 0.5, 3);
    glTexCoord2f(2.0f, 22.0f);
    glVertex3f(5, 1, 3);
    glTexCoord2f(22.0f, 22.0f);
    glVertex3f(5, 1, -3);
    glTexCoord2f(22.0f, 2.0f);
    glVertex3f(5, 0.5, -3);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

void People() {
    GLuint bench = glGenLists(1);
    glNewList(bench, GL_COMPILE);
    glScalef(0.008, 0.008, 0.008);
    glTranslatef(0, 13, 0);
    glColor3f(0.5, 0.5, 0.5);
    glBindTexture(GL_TEXTURE_2D, g_texture_check);
    loadFromMesh(models[0].shapes);
    glBindTexture(GL_TEXTURE_2D, 0);
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