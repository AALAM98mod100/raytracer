#include <iostream>
#include "../cameras/Perspective.hpp"
#include "../tracer/Basic.hpp"
#include "../geometry/Sphere.hpp"
#include "../materials/Cosine.hpp"
#include "../samplers/Simple.hpp"
#include "../samplers/Regular.hpp"
#include "../materials/Matte.hpp"
#include "../materials/Reflective.hpp"
#include "../utilities/Point3D.hpp"
#include "../utilities/Constants.hpp"
#include "../utilities/Vector3D.hpp"
#include "../light/Ambient.hpp"
#include "../light/Point.hpp"
#include "../light/Directional.hpp"
#include "../tracer/whitted.hpp"
#include "../world/World.hpp"

void World::build(void)
{
    // view plane
    vplane.top_left = Point3D(-6, 6, 6);
    vplane.bottom_right = Point3D(6, -6, 6);
    vplane.hres = 600;
    vplane.vres = 600;

    // Background color.
    bg_color = black;

    // camera and sampler.
    set_camera(new Perspective(0, 0, 20));

    // Whitted Tracer
    set_tracer(new whitted(this));
    sampler_ptr = new Simple(camera_ptr, &vplane);

    //Lighting
    Ambient *ambient_ptr = new Ambient;
    ambient_ptr->set_ls(1.0);
    set_ambient(ambient_ptr);

    Point *light_ptr = new Point;
    light_ptr->set_position(50, 50, 10);
    add_light(light_ptr);

    Reflective *reflective = new Reflective;
    reflective->set_ka(0.7);
    reflective->set_kd(0.7);
    reflective->set_cd(RGBColor(0.2, 0.02, 0.15));
    reflective->set_ks(0.15);
    reflective->set_exp(25.0);
    reflective->set_kr(1.0);

    Sphere *sphere_ptr2 = new Sphere(Point3D(-3, 0.0, -0.5), 2.0);
    sphere_ptr2->set_material(reflective);
    add_geometry(sphere_ptr2);

    Phong *Phong_ptr = new Phong;
    Phong_ptr->set_ka(0.7);
    Phong_ptr->set_kd(0.7);
    Phong_ptr->set_cd(RGBColor(0.095, 0.15, 0.245));
    Phong_ptr->set_ks(0.15);
    Phong_ptr->set_exp(25.0);

    Matte *matte_ptr = new Matte;
    matte_ptr->set_ka(0.7);
    matte_ptr->set_kd(0.7);
    matte_ptr->set_cd(RGBColor(0.0, 0.7, 0.7));

    Reflective *reflective2 = new Reflective;
    reflective2->set_ka(0.7);
    reflective2->set_kd(0.7);
    reflective2->set_cd(RGBColor(0.0, 0.7, 0.7));
    reflective2->set_ks(0.15);
    reflective2->set_exp(25.0);
    reflective2->set_kr(1.0);

    Sphere *sphere_ptr3 = new Sphere(Point3D(3, 0.0, -0.5), 2.0);

    // Set material to one of Phong_ptr, matte_ptr or reflective2 to see interactions among different materials.
    sphere_ptr3->set_material(Phong_ptr);
    add_geometry(sphere_ptr3);
}