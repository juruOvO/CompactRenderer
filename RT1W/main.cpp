#include <iostream>

int main(){
    int Width=256;
    int Height=256;

    std::cout<<"P3\n"<<Width<<' '<<Height<<"\n255\n";

    for(int y=0;y<Height;++y)
        for(int x=0;x<Width;++x){
            auto r=double(x)/(Width-1);
            auto g=double(y)/(Height-1);
            auto b=0.3;
            int ir=int(255.999*r);
            int ig=int(255.999*g);
            int ib=int(255.999*b);
            std::cout<<ir<<' '<<ig<<' '<<ib<<'\n';
        }
}