#include "main.h"
// Method that returns the z coordinate of 'x' pixel of a line 
float getZOfPixel(float xi, float zi, float xf, float zf, int x)
{
    return static_cast<float>(((zf - zi) * (x - xi)) / ((xf - xi) == 0 ? 1 : (xf - xi)) + zi);
}
int WorldCoordsToCamera(float worldCoord, int screenSize)
{
    return (worldCoord + 1.) * 0.5f * screenSize;
}
// It will compute the distance from the position of a pixel to the camera.
// For now, as we do not have a camera yet, the distance will be = 1 - the z world coord of the pixel, 
// so a couple of warnings will appear temporarily until a camera is implemented
float GetDistanceFromCamera(Vector3f position) {
    return 1 - position.z;
}

void DrawLine(float xif, float yif, float xff, float yff, float zi, float zf, TGAImage &image, TGAColor color)
{

    if (xff < xif && std::abs(xff - xif) > std::abs(yff - yif))
    {
        float aux = xif;
        xif = xff;
        xff = aux;
        aux = yif;
        yif = yff;
        yff = aux;
        aux = zi;
        zi = zf;
        zf = aux;
    }
    else if (yff < yif && std::abs(xff - xif) < std::abs(yff - yif))
    {
        float aux = xif;
        xif = xff;
        xff = aux;
        aux = yif;
        yif = yff;
        yff = aux;
        aux = zi;
        zi = zf;
        zf = aux;
    }

    float m = static_cast<float>(yff - yif) / (xff - xif);
    if ((xff - xif) > (yff - yif))
    {
        int y = 0;
        for (int x = xif; x <= xff; x++)
        {
            y = (int)(m * (x - xif) + yif);

            if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
            {
                float distanceFromCamera = GetDistanceFromCamera({x, y, getZOfPixel(xif, zi, xff, zf, x)});
                if (distanceFromCamera < zBuffer[x][y])
                {
                    image.set(x, y, color);
                    zBuffer[x][y] = distanceFromCamera;
                }
            }
        }
    }
    else
    {
        int x = 0;
        for (int y = yif; y <= yff; y++)
        {
            x = (int)((y - yif) / m) + xif;
            if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
            {
                float distanceFromCamera = GetDistanceFromCamera({x, y, getZOfPixel(xif, zi, xff, zf, x)});
                if (distanceFromCamera < zBuffer[x][y])
                {
                    image.set(x, y, color);
                    zBuffer[x][y] = distanceFromCamera;
                }
            }
        }
    }
}
void DrawTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color)
{
    DrawLine(v0.x, v0.y, v1.x, v1.y, v0.z, v1.z, image, color);
    DrawLine(v1.x, v1.y, v2.x, v2.y, v1.z, v2.z, image, color);
    DrawLine(v2.x, v2.y, v0.x, v0.y, v0.z, v2.z, image, color);
}

void FillFlatBottomTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color)
{
    float m1 = static_cast<float>(v2.y - v0.y) / (v2.x == v0.x ? 1 : (v2.x - v0.x));
    float m2 = static_cast<float>(v2.y - v1.y) / (v2.x == v1.x ? 1 : (v2.x - v1.x));
    for (int y = v0.y; y <= v2.y; y++)
    {
        float x1 = (m1 == 0 ? v0.x : (y - v0.y) / m1) + v0.x;
        float x2 = (m2 == 0 ? v1.x : (y - v1.y) / m2) + v1.x;
        DrawLine(x1, y, x2, y, v0.z, v1.z, image, color);
    }
}

void FillFlatTopTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color)
{
    float m1 = static_cast<float>(v1.y - v0.y) / (v1.x == v0.x ? 1 : (v1.x - v0.x));
    float m2 = static_cast<float>(v2.y - v1.y) / (v2.x == v1.x ? 1 : (v2.x - v1.x));
    for (int y = v1.y; y <= v0.y; y++)
    {
        float x1 = (m1 == 0 ? v0.x : (y - v0.y) / m1) + v0.x;
        float x2 = (m2 == 0 ? v1.x : (y - v1.y) / m2) + v1.x;
        DrawLine(x1, y, x2, y, v0.z, v1.z, image, color);
    }
}

void FillTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color)
{
    if ((v0.x == v1.x && v0.x == v2.x) || (v0.y == v1.y && v0.y == v2.y))
    {
        return;
    }
    Vector3f vmax, vmin, vother;
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
    // Convert X & Y World coordinates to Camera position
    // We want to keep the Z coordinate as World coordinates 
    vother.x = WorldCoordsToCamera(vother.x, image.get_width());
    vother.y = WorldCoordsToCamera(vother.y, image.get_height());
    vmin.x = WorldCoordsToCamera(vmin.x, image.get_width());
    vmin.y = WorldCoordsToCamera(vmin.y, image.get_height());
    vmax.x = WorldCoordsToCamera(vmax.x, image.get_width());
    vmax.y = WorldCoordsToCamera(vmax.y, image.get_height());

    // Check which kind of triangle we have
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

void DrawWireframe(Model *model, TGAImage &image, TGAColor color)
{
    for (unsigned int i = 0; i < model->n_faces; i++)
    {
        Vector3Int vertices = model->faces[i][0];

        Vector3f v1 = model->vertexs[vertices.x - 1];
        Vector3f v2 = model->vertexs[vertices.y - 1];
        Vector3f v3 = model->vertexs[vertices.z - 1];

        DrawTriangle(v1, v2, v3, image, color);
    }
}
void DrawModel(Model *model, TGAImage &image, TGAColor color)
{
    // Temporary light source
    Vector3f lightDirection = {0, 0, -1};
    for (unsigned int i = 0; i < model->n_faces; i++)
    {
        Vector3Int vertices = model->faces[i][0];
        Vector3Int normals = model->faces[i][2];

        Vector3f v1 = model->vertexs[vertices.x - 1];
        Vector3f v2 = model->vertexs[vertices.y - 1];
        Vector3f v3 = model->vertexs[vertices.z - 1];

        // Calculate the normal of the face, ignoring for now the normals of each vertex.
        // This is temporary to test flat shading.
        Vector3f vc = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
        Vector3f n = vc.Cross({v2.x - v1.x, v2.y - v1.y, v2.z - v1.z});
        n.Normalize();
        float lightIntensity = n.DotProduct(lightDirection);
        FillTriangle(v1, v2, v3, image, TGAColor((int)(color.r * lightIntensity), (int)(color.g * lightIntensity), (int)(color.b * lightIntensity), color.a));
    }
}

int main(int argc, char **argv)
{
    // Init image
    TGAImage image(4000, 4000, TGAImage::RGB);
    // Init depth buffer
    zBuffer = new float *[image.get_width()];
    for (int i = 0; i < image.get_width(); i++)
    {
        zBuffer[i] = new float[image.get_height()];
        for (int j = 0; j < image.get_height(); j++)
        {
            zBuffer[i][j] = FLT_MAX;
        }
    }
    // Read and parse the model
    Model *model = ParseObj("obj/sphere.obj");
    DrawModel(model, image, white);

    // Setting the origin to the top bottom
    image.flip_vertically(); 
    image.write_tga_file("output.tga");
    return 0;
}
