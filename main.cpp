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

        DrawLine(vi1.x, vi1.y, vi2.x, vi2.y, image, color);
        DrawLine(vi2.x, vi2.y, vi3.x, vi3.y, image, color);
        DrawLine(vi3.x, vi3.y, vi1.x, vi1.y, image, color);
    }
}

int main(int argc, char **argv)
{
    TGAImage image(2000, 2000, TGAImage::RGB);

    Model *model = ParseObj("obj/african_head.obj");
    DrawWireframe(model, image, white);

    image.flip_vertically(); // set the origin to top-bottom
    image.write_tga_file("output.tga");
    return 0;
}
