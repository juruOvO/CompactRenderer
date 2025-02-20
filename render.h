#pragma once
#include <vector>
#include <cmath>
#include <climits>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "render.h"
#include <fstream>
#include <sstream>

class Shader{
private:
    TGAImage* layer;
    int *zbuffer;
    int width;
    int height;
    int depth;
    Model* model;
    TGAImage* uv;
    Vec3f light_dir;
    Vec3f centre;
    Vec3f camera;
public:
    Shader();
    ~Shader();
    void SetParams(int uWidth,int uHeight,Model* uModel,TGAImage* uUV,Vec3f uLight,Vec3f uCenter,Vec3f uCamera);
    TGAImage* Render();
};