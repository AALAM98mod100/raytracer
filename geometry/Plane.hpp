#pragma once

/**
   This file declares the Plane class which represents a plane defined by an
   included point and its normal.

   Courtesy Kevin Suffern.
*/

#include "Geometry.hpp"

#include "../utilities/Point3D.hpp"
#include "../utilities/Vector3D.hpp"
#define inf std::numeric_limits<double>::infinity()

class Plane : public Geometry {
protected:
  Point3D a;  // point on the plane.
  Vector3D n; // normal to the plane, store as unit vector.
  float size; // Plane's size

public:
  // Constructors.
  Plane(); // set plane to xz-plane.
  Plane(const Point3D &pt,
        const Vector3D &n); 
        // const flat size); // set point and normal, then normalize.

  // Copy constructor and assignment operator.
  Plane(const Plane &object);
  Plane &operator=(const Plane &rhs);

  // Destructor.
  virtual ~Plane() = default;
  
  // String representation.
  virtual std::string to_string() const override;

  // Ray intersection. Set t and sinfo as per intersection with this object.
  virtual bool hit(const Ray &ray, float &t, ShadeInfo &s) const override;

  // shadow ray intersection
  virtual bool shadow_hit(const Ray& ray, float& tmin) const override;

  
  // Get bounding box.
  virtual BBox getBBox() const override;
};
