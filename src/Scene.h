#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <tiny_obj_loader.h>

typedef struct {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
} model_t;

void LoadObj(const char *filename);

void Grid();

void Hall();

void Net();

void People();

#endif