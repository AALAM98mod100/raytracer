#include "regular.hpp"
#include "../utilities/Ray.hpp"
#include "../world/ViewPlane.hpp"
#include "../cameras/Camera.hpp"


// regular::regular(): 
//     Sampler(camera_ptr, viewplane_ptr)
// {} 

regular::regular(Camera *c_ptr, ViewPlane *v_ptr){
    
    camera_ptr = c_ptr;
    viewplane_ptr = v_ptr;
}

regular::regular(const regular &camera)
    : Sampler(camera.camera_ptr, camera.viewplane_ptr)
{}



regular& regular::operator=(const regular& rhs){
    if(this == &rhs){
        return (*this);
    }
    camera_ptr = rhs.camera_ptr;
    viewplane_ptr = rhs.viewplane_ptr;
    return (*this);
    
}

std::vector<Ray> regular::get_rays(int px, int py) const {

    std::vector<Ray> rays;
    int n = 8;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Ray ray;  
            float xwidth = viewplane_ptr->bottom_right.x - viewplane_ptr->top_left.x;
            float ywidth = viewplane_ptr->bottom_right.y - viewplane_ptr->top_left.y;
            float x = viewplane_ptr->top_left.x + ((double)px/(double)(viewplane_ptr->hres) * xwidth + (j+1.0)/n);
            float y = viewplane_ptr->top_left.y + ((double)py/(double)(viewplane_ptr->vres) * ywidth + (i+1.0)/n);
            float z = viewplane_ptr -> top_left.z;
            ray.w = 1;
            ray.o = Point3D(x,y,z);
            ray.d = camera_ptr -> get_direction(Point3D(x,y,z));
            ray.d.normalize();
            rays.push_back(ray);
        }
    }
    
    return rays;

}
