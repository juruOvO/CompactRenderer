#include <algorithm>
#include "render.h"
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

Vec3f m2v(Matrix m){
    Vec3f res;
    res.x = m[0][0]/m[3][0];
    res.y = m[1][0]/m[3][0];
    res.z = m[2][0]/m[3][0];
    return res;
}

Matrix v2m(Vec3f v){
    Matrix m(4,1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

Matrix viewport(int x,int y,int width,int height,int depth){
    Matrix m = Matrix::identity(4);
    m[0][3] = x+width/2.f;
    m[1][3] = y+height/2.f;
    m[2][3] = depth/2.f;
    m[0][0] = width/2.f;
    m[1][1] = height/2.f;
    m[2][2] = depth/2.f;
    return m;
}

Matrix lookat(Vec3f camera,Vec3f centre,Vec3f upVec){
    Vec3f z = (camera-centre).normalize();
    Vec3f x = (upVec^z).normalize();
    Vec3f y = (z^x).normalize();
    Matrix res=Matrix::identity(4);
    for(int i=0;i<3;++i){
        res[0][i]=x[i];
        res[1][i]=y[i];
        res[2][i]=z[i];
        res[i][3]=-centre[i];
    }
    return res;
}

void triangle(Vec3i* screen_coords, TGAImage &image,int* zbuffer,int width,int height,TGAImage *uv,Vec3f*vts,float* intensity){
	if (screen_coords[0].y==screen_coords[1].y && screen_coords[0].y==screen_coords[2].y){
		return;
	}
	if (screen_coords[0].y>screen_coords[1].y) { std::swap(screen_coords[0], screen_coords[1]); std::swap(intensity[0], intensity[1]); std::swap(vts[0],vts[1]);}
    if (screen_coords[0].y>screen_coords[2].y) { std::swap(screen_coords[0], screen_coords[2]); std::swap(intensity[0], intensity[2]); std::swap(vts[0],vts[2]);}
    if (screen_coords[1].y>screen_coords[2].y) { std::swap(screen_coords[1],screen_coords[2]); std::swap(intensity[1], intensity[2]); std::swap(vts[1],vts[2]);}

	int total_height = screen_coords[2].y-screen_coords[0].y;
	TGAColor* colors=new TGAColor[3];
	for(int i=0;i<3;++i){
		colors[i]=uv->get(1024*vts[i].x,1024*vts[i].y);
	}

	for(int i=0;i<total_height;++i){
		bool upper_half=i>screen_coords[1].y-screen_coords[0].y||screen_coords[1].y==screen_coords[0].y;
		int segment_height=upper_half?screen_coords[2].y-screen_coords[1].y:screen_coords[1].y-screen_coords[0].y;
		float alpha=(float)i/total_height;
		float beta=(float)(i-(upper_half?screen_coords[1].y-screen_coords[0].y:0))/segment_height;
		Vec3i A=screen_coords[0]+(Vec3f)(screen_coords[2]-screen_coords[0])*alpha;
		Vec3i B=upper_half?screen_coords[1]+(Vec3f)(screen_coords[2]-screen_coords[1])*beta:screen_coords[0]+(Vec3f)(screen_coords[1]-screen_coords[0])*beta;
		float intensityA=intensity[0]+(intensity[2]-intensity[0])*alpha;
		TGAColor colorA;
		for(int j=0;j<4;++j){
			colorA.bgra[j]=colors[0].bgra[j]+(colors[2].bgra[j]-colors[0].bgra[j])*alpha;
		}
		float intensityB=upper_half?intensity[1]+(intensity[2]-intensity[1])*beta:
			intensity[0]+(intensity[1]-intensity[0])*beta;
		TGAColor colorB;
		for(int j=0;j<4;++j){
			colorB.bgra[j]=upper_half?colors[1].bgra[j]+(colors[2].bgra[j]-colors[1].bgra[j])*beta:
			colors[0].bgra[j]+(colors[1].bgra[j]-colors[0].bgra[j])*beta;
		}
		if(A.x>B.x){
			std::swap(A,B);
			std::swap(intensityA,intensityB);
			std::swap(colorA,colorB);
		}
		for(int j=A.x;j<=B.x;++j){
			float phi= A.x==B.x?1.:(float)(j-A.x)/(B.x-A.x);
			Vec3i target=Vec3f(A)+Vec3f(B-A)*phi;
			float Intensity=intensityA+(intensityB-intensityA)*phi;
			TGAColor Color;
			for(int k=0;k<4;++k){
				Color.bgra[k]=colorA.bgra[k]+(colorB.bgra[k]-colorA.bgra[k])*phi;
			}
			int idx=target.x+target.y*width;
			if(target.x>=width||target.x<0||target.y>=height||target.y<0)continue;
			if(zbuffer[idx]<target.z){
				zbuffer[idx]=target.z;
				image.set(target.x,target.y,Color*Intensity);
			}
		}
	}
}

void triangle(Vec3i* screen_coords, TGAImage &image,int* zbuffer,int width,int height,Vec3f*vts,float* intensity){
	if (screen_coords[0].y==screen_coords[1].y && screen_coords[0].y==screen_coords[2].y){
		return;
	}
	if (screen_coords[0].y>screen_coords[1].y) { std::swap(screen_coords[0], screen_coords[1]); std::swap(intensity[0], intensity[1]); std::swap(vts[0],vts[1]);}
    if (screen_coords[0].y>screen_coords[2].y) { std::swap(screen_coords[0], screen_coords[2]); std::swap(intensity[0], intensity[2]); std::swap(vts[0],vts[2]);}
    if (screen_coords[1].y>screen_coords[2].y) { std::swap(screen_coords[1],screen_coords[2]); std::swap(intensity[1], intensity[2]); std::swap(vts[1],vts[2]);}

	int total_height = screen_coords[2].y-screen_coords[0].y;

	for(int i=0;i<total_height;++i){
		bool upper_half=i>screen_coords[1].y-screen_coords[0].y||screen_coords[1].y==screen_coords[0].y;
		int segment_height=upper_half?screen_coords[2].y-screen_coords[1].y:screen_coords[1].y-screen_coords[0].y;
		float alpha=(float)i/total_height;
		float beta=(float)(i-(upper_half?screen_coords[1].y-screen_coords[0].y:0))/segment_height;
		Vec3i A=screen_coords[0]+(Vec3f)(screen_coords[2]-screen_coords[0])*alpha;
		Vec3i B=upper_half?screen_coords[1]+(Vec3f)(screen_coords[2]-screen_coords[1])*beta:screen_coords[0]+(Vec3f)(screen_coords[1]-screen_coords[0])*beta;
		float intensityA=intensity[0]+(intensity[2]-intensity[0])*alpha;
		float intensityB=upper_half?intensity[1]+(intensity[2]-intensity[1])*beta:
			intensity[0]+(intensity[1]-intensity[0])*beta;
		if(A.x>B.x){
			std::swap(A,B);
			std::swap(intensityA,intensityB);
		}
		for(int j=A.x;j<=B.x;++j){
			float phi= A.x==B.x?1.:(float)(j-A.x)/(B.x-A.x);
			Vec3i target=Vec3f(A)+Vec3f(B-A)*phi;
			float Intensity=intensityA+(intensityB-intensityA)*phi;
			int idx=target.x+target.y*width;
			if(target.x>=width||target.x<0||target.y>=height||target.y<0)continue;
			if(zbuffer[idx]<target.z){
				zbuffer[idx]=target.z;
				image.set(target.x,target.y,TGAColor(255*Intensity,255*Intensity,255*Intensity));
			}
		}
	}
}

Shader::Shader(){
	layer = new TGAImage;
	width=0;
	height=0;
	depth=255;
	model=NULL;
	uv=NULL;
	light_dir={0,0,0};
	centre={0,0,0};
	camera={0,0,0};
}

Shader::~Shader(){
	delete layer;
	delete model;
	delete uv;
}

void Shader::SetParams(int uWidth=0,int uHeight=0,Model* uModel=NULL,TGAImage* uUV=NULL,Vec3f uLight={0,0,0},Vec3f uCenter={0,0,0},Vec3f uCamera={0,0,0}){
	layer=NULL;
	width=uWidth;
	height=uHeight;
	model=uModel;
	uv=uUV;
	light_dir=uLight;
	centre=uCenter;
	camera=uCamera;
}

TGAImage* Shader::Render(){
	Matrix Projection = Matrix::identity(4);
    Matrix ViewPort = viewport(width/8,height/8,width*3/4,height*3/4,depth);
    Projection[3][2] = -1.f/camera.z;
    Matrix ModelView=lookat(camera,centre,Vec3f(0,1,0));
	layer=new TGAImage(width, height, TGAImage::RGB);
	zbuffer = new int[width*height];
	for(int i=0;i<width*height;++i)zbuffer[i]=-std::numeric_limits<int>::max();
	for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec3f vts[3];
        Vec3i screen_coords[3];
        Vec3f world_coords[3];
        float intensity[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] =  m2v(ViewPort*Projection*ModelView*v2m(v));
            world_coords[j]  = v;
            intensity[j] = model->getNorm(i, j)*light_dir;
        }
        for(int i=0;i<3;++i)vts[i]=model->getUV(face[i+3]);
		if(uv!=NULL)
        	triangle(screen_coords,*layer,zbuffer,width,height,uv,vts,intensity);
		else {
			std::cerr<<"[WARNING] "<<"No UV file provided. A grey-scale image will be generated.\n";
			triangle(screen_coords,*layer,zbuffer,width,height,vts,intensity);
		}
	}
	return layer;
}