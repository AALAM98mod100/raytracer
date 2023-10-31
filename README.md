## Raytracer
This is a raytracer written in C++ for the course CS-440 (Computer Graphics) at Habib University, taught by Waqar Saleem. Most of the code here can be found in "Ray Tracing from the Ground Up" by Keving Suffern.

## Features
#### Acceleration Structures
- Octree
#### Cameras
- Parallel
- Perspective
#### Lights
- Point
- Spotlight
- Ambient
- Directional
#### Materials
- Matte
- Phong
- Reflective
- Cosine
#### Samplers
- Regular
- Random
- Simple
#### Geometry
- Plane
- Sphere
- Triangle
#### Tracers
- Basic/Single
- Whitted
#### BRDF
- Lambertian
- Glossy
- Perfect Specular

## Usage
run `make build` to build the project. Then run `./raytracer` to run the raytracer. The output will be in `scene.ppm`.