#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            std::vector<int> vtf;
            std::vector<int> vnf;
            int itrash, idx;
            int uvf;
            int nmf;
            iss >> trash;
            while (iss >> idx >> trash >> uvf >> trash >> nmf) {
                idx--; 
                uvf--;
                nmf--;
                vtf.push_back(uvf);
                vnf.push_back(nmf);
                f.push_back(idx);
            }
            for(int i=0;i<vtf.size();++i)f.push_back(vtf[i]);
            for(int i=0;i<vnf.size();++i)f.push_back(vnf[i]);
            //f: 0-2/3-5/6-8
            faces_.push_back(f);
        }else if(!line.compare(0,3,"vt ")){
            iss>>trash;
            iss>>trash;
            Vec3f vt;
            for(int i=0;i<3;++i)iss>>vt[i];
            texture_verts_.push_back(vt);
        }else if(!line.compare(0,3,"vn ")){
            iss>>trash>>trash;
            Vec3f n;
            for(int i=0;i<3;++i)iss>>n[i];
            norms_.push_back(n);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " <<texture_verts_.size()<<" vn# "<<norms_.size()<<'\n';
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

int Model::nvts(){
    return (int)texture_verts_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec3f Model::getUV(int i){
    return texture_verts_[i];
}

Vec3f Model::getNorm(int iface,int nvert){
    int idx=faces_[iface][nvert+6];
    //nvert should be 0,1,2
    return norms_[idx];
}