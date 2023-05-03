#include "tgaimage.h"
#include <iostream>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void drawLine(int xi, int yi, int xf, int yf, TGAImage &image, TGAColor color)
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
            image.set(x, (int)(m * (x-xi) + yi), color);
        }
    }
    else
    {
        for (int y = yi; y <= yf; y++)
        {
            image.set((int)((y - yi) / m) + xi, y, color);
        }
    }
}
int main(int argc, char **argv)
{
    TGAImage image(100, 100, TGAImage::RGB);

    // Draw 3 test lines
    drawLine(0, 0, 30, 40, image, red);
    drawLine(30,40, 50, 50, image, white);
    drawLine(50, 50, 99, 99, image, red);

    image.flip_vertically(); // set the origin to top-bottom
    image.write_tga_file("output.tga");
    return 0;
}
