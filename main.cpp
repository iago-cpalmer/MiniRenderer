#include "main.h"

void DrawLine(int xi, int yi, int xf, int yf, TGAImage &image, TGAColor color)
{   
    if (xf < xi && std::abs(xf - xi) > std::abs(yf - yi))
    {
        int aux = xi;
        xi = xf;
        xf = aux;
        aux = yi;
        yi = yf;
        yf = aux;
    }
    else if (yf < yi && std::abs(xf - xi) < std::abs(yf - yi))
    {
        int aux = yi;
        yi = yf;
        yf = aux;
        aux = xi;
        xi = xf;
        xf = aux;
    }

    float m = static_cast<float>(yf - yi) / (xf - xi);
    if ((xf - xi) > (yf - yi))
    {
        int y = 0;
        for (int x = xi; x <= xf; x++)
        {
            y = (int)(m * (x - xi) + yi);
            image.set(x, y, color);
        }
    }
    else
    {
        int x = 0;
        for (int y = yi; y <= yf; y++)
        {
            x = (int)((y - yi) / m) + xi;
            image.set(x, y, color);             
        }
    }
}
void DrawTriangle(Vector3Int v0, Vector3Int v1, Vector3Int v2, TGAImage &image, TGAColor color)
{
    DrawLine(v0.x, v0.y, v1.x, v1.y, image, color);
    DrawLine(v1.x, v1.y, v2.x, v2.y, image, color);
    DrawLine(v2.x, v2.y, v0.x, v0.y, image, color);
}
void DrawWireframe(Model *model, TGAImage &image, TGAColor color)
{
    for (unsigned int i = 0; i < model->n_faces; i++)
    {
        Vector3Int vertices = model->faces[i][0];

        Vector3f v1 = model->vertexs[vertices.x - 1];
        Vector3f v2 = model->vertexs[vertices.y - 1];
        Vector3f v3 = model->vertexs[vertices.z - 1];

        Vector3Int vi1 = {(int)((v1.x + 1.) * 0.5 * image.get_width()), (int)((1. + v1.y) * 0.5 * image.get_height()), 0};
        Vector3Int vi2 = {(int)((v2.x + 1.) * 0.5 * image.get_width()), (int)((1. + v2.y) * 0.5 * image.get_height()), 0};
        Vector3Int vi3 = {(int)((v3.x + 1.) * 0.5 * image.get_width()), (int)((1. + v3.y) * 0.5 * image.get_height()), 0};

        DrawTriangle(vi1, vi2, vi3, image, color);
    }
}

void FillFlatBottomTriangle(Vector3Int v0, Vector3Int v1, Vector3Int v2, TGAImage &image, TGAColor color)
{
    float m1 = static_cast<float>(v2.y - v0.y) / (v2.x == v0.x ? 1 : (v2.x - v0.x));
    float m2 = static_cast<float>(v2.y - v1.y) / (v2.x == v1.x ? 1 : (v2.x - v1.x));
    for (int y = v0.y; y <= v2.y; y++)
    {
        int x1 = (int)(m1 == 0 ? v0.x : (y - v0.y) / m1) + v0.x;
        int x2 = (int)(m2 == 0 ? v1.x : (y - v1.y) / m2) + v1.x;
        DrawLine(x1, y, x2, y, image, color);
    }
}

void FillFlatTopTriangle(Vector3Int v0, Vector3Int v1, Vector3Int v2, TGAImage &image, TGAColor color)
{
    float m1 = static_cast<float>(v1.y - v0.y) / (v1.x == v0.x ? 1 : (v1.x - v0.x));
    float m2 = static_cast<float>(v2.y - v1.y) / (v2.x == v1.x ? 1 : (v2.x - v1.x));
    for (int y = v1.y; y <= v0.y; y++)
    {
        int x1 = (int)(m1 == 0 ? v0.x : (y - v0.y) / m1) + v0.x;
        int x2 = (int)(m2 == 0 ? v1.x : (y - v1.y) / m2) + v1.x;
        DrawLine(x1, y, x2, y, image, color);
    }
}

void FillTriangle(Vector3Int v0, Vector3Int v1, Vector3Int v2, TGAImage &image, TGAColor color)
{
    if((v0.x == v1.x && v0.x==v2.x) || (v0.y == v1.y && v0.y==v2.y)) {
        return;
    }
    Vector3Int vmax, vmin, vother;
    if (v0.y >= v1.y && v0.y >= v2.y)
    {
        // v0 max
        if (v1.y > v2.y)
        {
            vmin = v2;
            vother = v1;
        }
        else
        {
            vmin = v1;
            vother = v2;
        }
        vmax = v0;
    }
    else if (v0.y < v1.y && v2.y > v1.y)
    {
        // v2 max
        // v0 min
        vmax = v2;
        vmin = v0;
        vother = v1;
    }
    else if (v0.y < v2.y && v1.y >= v2.y)
    {
        // v1 max
        // v0 min
        vmax = v1;
        vmin = v0;
        vother = v2;
    }
    else if (v2.y > v1.y)
    {
        // v2 max
        // v1 min
        vmax = v2;
        vmin = v1;
        vother = v0;
    }
    else
    {
        // v1 max
        // v2 min
        vmax = v1;
        vmin = v2;
        vother = v0;
    }
    

    if (vother.y == vmin.y)
    {
        // Flat bottom triangle
        FillFlatBottomTriangle(vother, vmin, vmax, image, color);
    }
    else if (vother.y == vmax.y)
    {
        // flat top triangle
        FillFlatTopTriangle(vother, vmin, vmax, image, color);
    }
    else
    {
        // Do flat bottom triangle
        FillFlatBottomTriangle(vother, vmin, vmax, image, color);
        // Do flat top triangle
        FillFlatTopTriangle(vother, vmin, vmax, image, color);
    }
}

void DrawModel(Model *model, TGAImage &image, TGAColor color)
{
    // Temporary light source
    Vector3f lightDirection = {0,0,-1};
    for (unsigned int i = 0; i < model->n_faces; i++)
    {
        Vector3Int vertices = model->faces[i][0];
        Vector3Int normals = model->faces[i][2];

        Vector3f v1 = model->vertexs[vertices.x - 1];
        Vector3f v2 = model->vertexs[vertices.y - 1];
        Vector3f v3 = model->vertexs[vertices.z - 1];

        Vector3Int vi1 = {(int)((v1.x + 1.) * 0.5 * image.get_width()), (int)((1. + v1.y) * 0.5 * image.get_height()), 0};
        Vector3Int vi2 = {(int)((v2.x + 1.) * 0.5 * image.get_width()), (int)((1. + v2.y) * 0.5 * image.get_height()), 0};
        Vector3Int vi3 = {(int)((v3.x + 1.) * 0.5 * image.get_width()), (int)((1. + v3.y) * 0.5 * image.get_height()), 0};

        // Calculate the normal of the face, ignoring for now the normals of each vertex. 
        // This is temporary to test flat shading.
        Vector3f vc = {v3.x - v1.x, v3.y-v1.y, v3.z-v1.z};
        Vector3f n = vc.Cross({v2.x - v1.x, v2.y-v1.y, v2.z-v1.z});
        n.Normalize();
        float lightIntensity = n.DotProduct(lightDirection);
        // Checking if lightIntensity is higher than 0 to do a back face culling
        if(lightIntensity>0) {
            FillTriangle(vi1, vi2, vi3, image, TGAColor((int) (color.r*lightIntensity), (int) (color.g*lightIntensity), (int) (color.b*lightIntensity), color.a));
        }
    }
}

int main(int argc, char **argv)
{
    TGAImage image(4000,4000, TGAImage::RGB);

    Model *model = ParseObj("obj/sphere.obj");
    DrawModel(model, image, white);
    //DrawWireframe(model, image, white);
    image.flip_vertically(); // set the origin to top-bottom
    image.write_tga_file("output.tga");
    return 0;
}
