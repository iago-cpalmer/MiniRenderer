#ifndef MODEL_H
#define MODEL_H

#include "Vector3f.h"
#include "Vector3Int.h"
#include "Vector2f.h"

struct Model {
    Vector3f* vertexs;
    Vector2f* textures;
    Vector3f* normals;
    Vector3Int** faces;
    unsigned int n_vertices;
    unsigned int n_textures;
    unsigned int n_normals;
    unsigned int n_faces;
};

#endif