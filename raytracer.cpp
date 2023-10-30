#include <iostream>
#include <ctime>

#include "materials/Cosine.hpp"

#include "samplers/Sampler.hpp"

#include "utilities/Image.hpp"
#include "utilities/RGBColor.hpp"
#include "utilities/Ray.hpp"
#include "utilities/ShadeInfo.hpp"

#include "world/World.hpp"
#include "world/ViewPlane.hpp"

#define USE_ACCELERATION true

int main(int argc, char **argv)
{
  World world;
  world.build();

  Sampler *sampler = world.sampler_ptr;
  ViewPlane &viewplane = world.vplane;
  Image image(viewplane);

  std::clock_t start;
  double duration;
  start = std::clock(); // get current time

  std::vector<Ray> rays;
  for (int x = 0; x < viewplane.hres; x++)
  { // across.
    std::cout << "\r" << x + 1 << "/" << viewplane.hres;
    for (int y = 0; y < viewplane.vres; y++)
    { // down.
      // Get rays for the pixel from the sampler. The pixel color is the
      // weighted sum of the shades for each ray.
      RGBColor pixel_color(0);
      rays = sampler->get_rays(x, y);
      for (const auto &ray : rays)
      {
        float weight = ray.w; // ray weight for the pixel.
        //std::cout << ray.to_string() << "\n";
        ShadeInfo sinfo = world.hit_objects(ray, USE_ACCELERATION);
        if (sinfo.hit)
        {
          pixel_color += sinfo.material_ptr->shade(sinfo);
          // std::cout << pixel_color.to_string() << "\n";
        }

        else
        {
          pixel_color += weight * world.bg_color;
        }
      }
      // Save color to image.
      image.set_pixel(x, y, pixel_color);
      // std::cout << pixel_color.to_string() << "\n";
    }
  }

  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

  // Write image to file.
  image.write_ppm("scene.ppm");

  std::cout << "\nWrote image.\n";
  std::cout << "Ray casting took " << (USE_ACCELERATION ? "(Accelerated): " : " (No Accelerated): ") << duration << "seconds" << std::endl;

  return 0;
}
