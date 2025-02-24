#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "global.h"
#include <vector>

class hittable_list:public hittable{
    public:
    std::vector<shared_ptr<hittable>> objects;
    hittable_list(){}
    hittable_list(shared_ptr<hittable> object){add(object);}
    void clear(){objects.clear();}
    void add(shared_ptr<hittable> object){
        objects.push_back(object);
    }
    bool hit(const ray&r,interval ray_t,hit_record& rec)const override{
        hit_record temp_rec;
        bool hit_anything=0;
        auto closet_so_far=ray_t.max;
        for(const auto&object:objects){
            if(object->hit(r,interval(ray_t.min,closet_so_far),temp_rec)){
                hit_anything=1;
                closet_so_far=temp_rec.t;
                rec=temp_rec;
            }
        }
        return hit_anything;
    }
};
#endif
//C++ New Features:
// shared_ptr<type> is a pointer to some allocated type, with reference-counting semantics.
// Every time you assign its value to another shared pointer (usually with a simple
// assignment), the reference count is incremented. As shared pointers go out of scope
// (like at the end of a block or function), the reference count is decremented. Once the
// count goes to zero, the object is safely deleted.

// make_shared<thing>(thing_constructor_params ...) allocates a new instance of type thing, 
// using the constructor parameters. It returns a shared_ptr<thing>.

//std::shared_ptr is included with the <memory> header.