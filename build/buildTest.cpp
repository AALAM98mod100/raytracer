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
#include <random>

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
    vplane.vres = 1920;

    bg_color = RGBColor(0.15);

    // Camera and sampler.
    set_camera(new Perspective(0, 0, 20));
    set_tracer(new Basic(this));
    sampler_ptr = new Simple(camera_ptr, &vplane);

    int num = 10000;
    for (int i = 0; i < num; i++)
    {
        RGBColor c(((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)));
        int randomX = (-10) + (rand() % static_cast<int>(10 - (-10) + 1));
        int randomY = (-10) + (rand() % static_cast<int>(10 - (-10) + 1));
        Sphere *sphere_ptr = new Sphere(Point3D(randomX, randomY, 0), 3);
        sphere_ptr->set_material(new Cosine(c));
        add_geometry(sphere_ptr);
    }

    std::clock_t start;
    double duration;
    start = std::clock(); // get current time
    this->accelaration_ptr = new Octree(2, 8, this->geometry);
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    std::cout << "Octree build time: " << duration << "\n";
}