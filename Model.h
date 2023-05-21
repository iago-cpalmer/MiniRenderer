#ifndef MODEL_H
#define MODEL_H
#include "Vector3f.h"
#include "Vector3Int.h"

struct Model {
    Vector3f* vertexs;
    Vector3f* textures;
    Vector3f* normals;
    Vector3Int** faces;
    unsigned int n_vertices;
    unsigned int n_textures;
    unsigned int n_normals;
    unsigned int n_faces;
};

#endif