#include "color.h"
#include "vec3.h"
#include "ray.h"
#include <iostream>

color ray_color(const ray& r){
    vec3 unit_direction=unit_vector(r.direction());
    auto a=0.5*(unit_direction.y()+1.0);
    return (1.0-a)*color(1.0,1.0,1.0)+a*color(0.5,0.7,1.0);
}

int main(){
    auto aspect_ratio=16.0/9.0;
    int image_width=400;
    int image_height=(int)(image_width/aspect_ratio);
    if(image_height<1)image_height=1;

    auto focal_length=1.0;
    auto viewport_height=2.0;
    auto viewport_width=viewport_height*(double)(image_width/image_height);
    auto camera_center=point3(0,0,0);

    auto viewport_u=vec3(viewport_width,0,0);
    auto viewport_v=vec3(0,-viewport_height,0);
    auto pixel_delta_u=viewport_u/image_width;
    auto pixel_delta_v=viewport_v/image_height;

    auto viewport_upper_left=camera_center-vec3(0,0,focal_length)-viewport_u/2-viewport_v/2;
    auto pixel00_loc=viewport_upper_left+0.5*(pixel_delta_u+pixel_delta_v);
    
    std::cout<<"P3\n"<<image_width<<' '<<image_height<<"\n255\n";

    for(int y=0;y<image_height;++y){
        std::clog<<"\rScanlines remaining: "<<(image_height-y)<<' '<<std::flush;
        for(int x=0;x<image_width;++x){
            auto pixel_center=pixel00_loc+(x*pixel_delta_u)+(y*pixel_delta_v);
            auto ray_direction=pixel_center-camera_center;
            ray r(camera_center,ray_direction);
            color pixel_color=ray_color(r);
            write_color(std::cout,pixel_color);
        }
    }
    std::clog<<"\rDone\n";
    return 0;
}