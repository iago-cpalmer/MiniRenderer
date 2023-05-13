#include "main.h"

Vector3 ParseVector(std::string line)
{
    int lastSpace = 1;
    int nNumbers = 0;
    Vector3 v;
    for (long unsigned int i = lastSpace; i < line.length(); i++)
    {
        if (line.at(i) == ' ')
        {
            lastSpace = i;
            nNumbers++;
            switch (nNumbers)
            {
            case 1:
                v.x = stof(line.substr(lastSpace + 1, i - lastSpace - 1));
                break;
            case 2:
                v.y = stof(line.substr(lastSpace + 1, i - lastSpace - 1));
                break;
            case 3:
                v.z = stof(line.substr(lastSpace + 1, i - lastSpace - 1));
                break;
            }
        }
    }
    return v;
}

Vector3Int *ParseFace(std::string line)
{
    Vector3Int *face = new Vector3Int[3];
    std::vector<std::string> vertices;
    std::stringstream ss(line.substr(2));
    std::string vertex;
    while (std::getline(ss, vertex, ' '))
    {
        vertices.push_back(vertex);
    }
    for (int i = 0; i < 3; i++)
    {
        std::stringstream vss(vertices[i]);
        std::string v;
        int j = 0;
        while (std::getline(vss, v, '/'))
        {
            if (i == 0)
            {
                face[j].x = std::stoi(v);
            }
            else if (i == 1)
            {
                face[j].y = std::stoi(v);
            }
            else if (i == 2)
            {
                face[j].z = std::stoi(v);
            }
            j++;
        }
    }
    return face;
}

Model ParseObj(std::string filePath)
{
    std::ifstream file(filePath);
    std::string line;
    std::list<Vector3> vertexs;
    std::list<Vector3> textures;
    std::list<Vector3> normals;
    std::list<Vector3Int *> faces;
    while (std::getline(file, line))
    {
        if (line.find("v ") != std::string::npos)
        {
            // Vertexs
            vertexs.push_back(ParseVector(line));
        }
        else if (line.find("vt ") != std::string::npos)
        {
            // Texture coords
            textures.push_back(ParseVector(line));
        }
        else if (line.find("vn ") != std::string::npos)
        {
            // Vertex normals
            normals.push_back(ParseVector(line));
        }
        else if (line.find("f ") != std::string::npos)
        {
            // Faces
            faces.push_back(ParseFace(line));
        }
    }
    Vector3 *vertexArr = new Vector3[vertexs.size()];
    std::copy(vertexs.begin(), vertexs.end(), vertexArr);

    Vector3 *textureArr = new Vector3[textures.size()];
    std::copy(textures.begin(), textures.end(), textureArr);

    Vector3 *normalArr = new Vector3[normals.size()];
    std::copy(normals.begin(), normals.end(), normalArr);

    // Get the number of elements in the list
    int numElements = faces.size();

    // Create a double array of Vector3Int
    Vector3Int **faceArray = new Vector3Int *[numElements];
    for (int i = 0; i < numElements; i++)
    {
        faceArray[i] = new Vector3Int[3];
    }

    // Copy the data from the list to the double array
    int i = 0;
    for (auto &facePtr : faces)
    {
        faceArray[i][0] = *facePtr++;
        faceArray[i][1] = *facePtr++;
        faceArray[i][2] = *facePtr++;

        i++;
    }

    return Model(vertexArr, textureArr, normalArr, faceArray, vertexs.size(), textures.size(), normals.size(), faces.size());
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}
void DrawLine(int xi, int yi, int xf, int yf, TGAImage &image, TGAColor color)
{
    // Swap x's & y's that xf>xi & yf>yi
    if (xf < xi)
    {
        int aux = xi;
        xi = xf;
        xf = aux;
    }
    if (yf < yi)
    {
        int aux = yi;
        yi = yf;
        yf = aux;
    }
    float m = static_cast<float>(yf - yi) / (xf - xi);
    if ((xf - xi) > (yf - yi))
    {
        for (int x = xi; x <= xf; x++)
        {
            int y = (int)(m * (x-xi) + yi);
            image.set(x, y, color);
        }
    }
    else
    {
        for (int y = yi; y <= yf; y++)
        {
            int x = (int)((y - yi) / m) + xi;
            image.set(x, y, color);
        }
    }
}

void DrawWireframe(Model model, TGAImage &image, TGAColor color)
{
    for (int i = 0; i < model.n_faces; i++)
    {
        Vector3Int vertices = model.faces[i][0];

        Vector3 v1 = model.vertexs[vertices.x - 1];
        Vector3 v2 = model.vertexs[vertices.y - 1];
        Vector3 v3 = model.vertexs[vertices.z - 1];

        Vector3Int vi1 = {(int)((v1.x + 1.) * 0.5 * image.get_width()), (int)((1. + v1.y) * 0.5 * image.get_height()), 0};
        Vector3Int vi2 = {(int)((v2.x + 1.) * 0.5 * image.get_width()), (int)((1. + v2.y) * 0.5 * image.get_height()), 0};
        Vector3Int vi3 = {(int)((v3.x + 1.) * 0.5 * image.get_width()), (int)((1. + v3.y) * 0.5 * image.get_height()), 0};

       // std::cout << "Face: " << model.faces[i][0].x << ", " << model.faces[i][0].y << ", " << model.faces[i][0].z << std::endl;
        //std::cout << v2.x << ", " << v2.y << "," << v2.z << std::endl
        //          << std::endl;
        DrawLine(vi1.x, vi1.y, vi2.x, vi2.y, image, color);
        DrawLine(vi2.x, vi2.y, vi3.x, vi3.y, image, color);
        DrawLine(vi3.x, vi3.y, vi1.x, vi1.y, image, color);
    }
}

int main(int argc, char **argv)
{
    TGAImage image(2000, 2000, TGAImage::RGB);

    // Draw 3 test lines
    
    line(0, 0, 30, 40, image, red);
    line(30, 50, 50, 50, image, white);
    line(50, 50, 99, 99, image, red);

    Model model = ParseObj("obj/african_head.obj");
    std::cout << "Model Vertices: " << model.n_vertices << std::endl;
    std::cout << "Model Texutres: " << model.n_textures << std::endl;
    std::cout << "Model Normals: " << model.n_normals << std::endl;
    std::cout << "Model Faces: " << model.n_faces << std::endl;

    DrawWireframe(model, image, white);

    image.flip_vertically(); // set the origin to top-bottom
    image.write_tga_file("output.tga");
    return 0;
}
