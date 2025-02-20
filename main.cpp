#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "render.h"
#include <fstream>
#include <sstream>

Model* model = NULL;
const int width = 1024;
const int height = 1024;
const int depth = 255;
Vec3f light_dir=Vec3f(1,-1,1).normalize();
Vec3f camera(-1,1,3);
Vec3f centre(0,0,0);

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("../obj/Head.obj");
    }
    TGAImage *uv = new TGAImage;
    uv->read_tga_file("../texture/uv.tga");
    uv->flip_vertically();
    
    TGAImage scene;
    Shader myShader;
    myShader.SetParams(800,800,model,uv,light_dir,centre,camera);
    scene=*myShader.Render();
    scene.flip_vertically();
    scene.write_tga_file("output.tga");
    return 0;
}  