#include "../world/ViewPlane.hpp"
#include "../world/World.hpp"

#include "../utilities/RGBColor.hpp"
#include "../geometry/Geometry.hpp"
#include "../samplers/Sampler.hpp"
#include "../utilities/ShadeInfo.hpp"

#include "../cameras/Perspective.hpp"
#include "../cameras/Parallel.hpp"

#include "../geometry/Plane.hpp"
#include "../geometry/Sphere.hpp"

#include "../materials/Cosine.hpp"
#include "../materials/Matte.hpp"
#include "../materials/Reflective.hpp"

#include "../samplers/Simple.hpp"
#include "../samplers/Regular.hpp"

#include "../utilities/Constants.hpp"
#include "../utilities/Point3D.hpp"
#include "../light/Ambient.hpp"
#include "../light/Directional.hpp"
#include "../light/Point.hpp"
#include <ctime>
#include <iostream>

void World::build(void)
{

    // View plane  .
    vplane.top_left.x = -10;
    vplane.top_left.y = 10;
    vplane.top_left.z = 10;
    vplane.bottom_right.x = 10;
    vplane.bottom_right.y = -10;
    vplane.bottom_right.z = 10;
    vplane.hres = 1920;
    vplane.vres = 1080;

    // Background color.
    bg_color = black;

    // Camera and sampler.
    set_camera(new Perspective(10, 5, 20));
    // set_tracer(new whitted(this));
    set_tracer(new Basic(this));
    sampler_ptr = new Simple(camera_ptr, &vplane);

    // Custom Scene to tests times
    // unsigned int rows = 40;
    // unsigned int columns = 40;
    // float radius = 2;
    // Cosine *colors[] = {new Cosine(red), new Cosine(blue), new Cosine(white)};
    // for (unsigned int i = 0; i < rows; i++)
    // {
    //     for (unsigned int j = 0; j < columns; j++)
    //     {
    //         Sphere *sphere_ptr = new Sphere(Point3D(j * radius * 2 - (columns / 2.0f * radius * 2), i * radius * 2 - (rows / 2.0f * radius * 2), 0), radius);
    //         sphere_ptr->set_material(colors[(i + j * columns) % 3]);
    //         add_geometry(sphere_ptr);
    //     }
    // }

    // sphere
    //   Sphere* sphere_ptr = new Sphere(Point3D(-12, 2, 0), 5);
    //   sphere_ptr->set_material(new Cosine(red));
    //   add_geometry(sphere_ptr);

    //   Sphere* sphere_ptr2 = new Sphere(Point3D(0, 2, 0), 5);
    //   sphere_ptr2->set_material(new Cosine(white));
    //   add_geometry(sphere_ptr2);

    //   Sphere* sphere_ptr3 = new Sphere(Point3D(6, 2, 0), 5);
    //   sphere_ptr3->set_material(new Cosine(blue));
    //   add_geometry(sphere_ptr3);

    // //   Sphere* sphere_ptr4 = new Sphere(Point3D(, 2, 0), 5);
    // //   sphere_ptr4->set_material(new Cosine(white));
    // //   add_geometry(sphere_ptr4);

    // // triangle
    // Point3D a(2.5, -5, 1);
    // Point3D b(14, -1, 0);
    // Point3D c(8.5, 5, 0.5);
    // Triangle *triangle_ptr = new Triangle(a, b, c);
    // triangle_ptr->set_material(new Cosine(blue));
    //add_geometry(triangle_ptr);

    Ambient *ambient_ptr = new Ambient;
    ambient_ptr->scale_radiance(2.0);
    set_ambient_light(ambient_ptr);

    Point *point_ptr = new Point;
    point_ptr->set_position(Point3D(0, 0, 10));
    point_ptr->scale_radiance(4.0);
    point_ptr->set_shadows(true);
    add_light(point_ptr);

    //load mesh
    Reflective *reflective = new Reflective;
    reflective->set_ka(0.7);
    reflective->set_kd(0.7);
    reflective->set_cd(RGBColor(0.2, 0.02, 0.15));
    reflective->set_ks(0.15);
    reflective->set_exp(25.0);
    reflective->set_kr(1.0);

    Phong *pointerPH = new Phong;
    pointerPH->set_cd(RGBColor(0.7, 0.7, 0.7));
    pointerPH->set_exp(0.1);
    pointerPH->set_ka(0.2);
    pointerPH->set_kd(3);
    pointerPH->set_ks(0.2);
    // addOBJ("testmonke.obj", reflective);
    addOBJ("torus.obj", new Cosine(white));
    // addOBJ("tower of hanoi np.obj", pointerPH);
    // add_ply("monke.ply", new Cosine(white), Point3D(-10, -10, -10), Point3D(10, 10, 10));
    // add_ply("UFO2.ply", new Cosine(white), Point3D(-10, -10, -10), Point3D(10, 10, 10));
    // addOBJ("UFO2.obj", new Cosine(white));
    // addOBJ("car.obj", new Cosine(white));

    // builds the acceleration structure
    std::clock_t start;
    double duration;
    start = std::clock(); // get current time
    this->accelaration_ptr = new Octree(2, 8, this->geometry);
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    std::cout << "Octree build time: " << duration << "\n";
}