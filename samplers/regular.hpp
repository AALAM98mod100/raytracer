#pragma once

/**
   This file declares the regular class which represents a regular sampler.

   It shoots a single ray of weight 1 through a pixel.

   Courtesy Kevin Suffern.
*/

#include "Sampler.hpp"

class regular : public Sampler {
protected:
int num_rays = 1;
  // add members to cache values to avoid recomputation in get_rays().

public:
  // Constructors.
  regular() = default;                      // initializes members to NULL.
  regular(Camera *c_ptr, ViewPlane *v_ptr); // set members.

  // Copy constuctor and assignment operator.
  regular(const regular &camera);
  regular &operator=(const regular &other);

  // Desctructor.
  virtual ~regular() = default;

  // Shoot a ray of weight 1 through the center of the pixel.
  std::vector<Ray> get_rays(int px, int py) const override;
};
