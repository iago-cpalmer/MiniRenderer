#include "Geometry.h"
class Model
{
public:
    Vector3 *vertexs;
    Vector3 *textures;
    Vector3 *normals;
    Vector3Int **faces;

    int n_vertices;
    int n_textures;
    int n_normals;
    int n_faces;

    // Constructors
    Model(Vector3 *_vertexs, Vector3 *_textures, Vector3 *_normals, Vector3Int **_faces, int n_vertices, int n_textures, int n_normals, int n_faces)
    {

        vertexs = _vertexs;
        textures = _textures;
        normals = _normals;
        faces = _faces;
        this->n_normals = n_normals;
        this->n_vertices = n_vertices;
        this->n_textures = n_textures;
        this->n_faces = n_faces;
    }

    Model(Vector3 *_vertexs, Vector3 *_textures, Vector3 *_normals, int n_vertices, int n_textures, int n_normals)
    {
        vertexs = _vertexs;
        textures = _textures;
        normals = _normals;
        this->n_normals = n_normals;
        this->n_vertices = n_vertices;
        this->n_textures = n_textures;
    }

};