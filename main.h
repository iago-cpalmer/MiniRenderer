#ifndef MAIN_H
#define MAIN_H

#include <fstream>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <cmath>
#include <cfloat>

#include "tgaimage.h"
#include "Parser.h"
#include "Model.h"

// Constants
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

// Global variables
float **zBuffer; // Stores the depth of each pixel drawn in screen to do the z-buffer algorithm.

// Functions
void DrawModel(Model *model, TGAImage &image, TGAColor color);
void DrawWireframe(Model *model, TGAImage &image, TGAColor color);
void FillTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color);
void FillFlatTopTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color);
void FillFlatBottomTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color);
void DrawTriangle(Vector3f v0, Vector3f v1, Vector3f v2, TGAImage &image, TGAColor color);
void DrawLine(float xif, float yif, float xff, float yff, float zi, float zf, TGAImage &image, TGAColor color);
int WorldCoordsToCamera(float worldCoord, int screenSize);
float getZOfPixel(float xi, float zi, float xf, float zf, int x);
float GetDistanceFromCamera(Vector3f position);

#endif