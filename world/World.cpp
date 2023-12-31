#include "World.hpp"
#include <iostream>
#include "../acceleration/Octree.hpp"
#include "strings.h"
#include <ctime>
#include "../materials/Reflective.hpp"
#include "../happly/happly.h"

World::World()
{
    vplane = ViewPlane();
    bg_color = RGBColor();
    ambient_ptr = new Ambient;
    camera_ptr = nullptr;
    sampler_ptr = nullptr;
    accelaration_ptr = nullptr;
    tracer_ptr = nullptr;
}

World::~World()
{
    for (auto i : geometry)
    {
        delete i;
    }
    for (auto i : lights)
    {
        delete i;
    }
    delete tracer_ptr;
    delete camera_ptr;
    delete sampler_ptr;
    delete ambient_ptr;
    delete accelaration_ptr;
}

void World::add_geometry(Geometry *geom_ptr)
{
    this->geometry.push_back(geom_ptr);
}
void World::set_tracer(Tracer *t_ptr)
{
    tracer_ptr = t_ptr;
}
void World::add_light(Light *light_ptr)
{
    lights.push_back(light_ptr);
}
void World::set_camera(Camera *c_ptr)
{
    this->camera_ptr = c_ptr;
}

void World::set_ambient_light(Ambient *amb_ptr)
{
    if (ambient_ptr)
    {
        delete ambient_ptr;
    }
    ambient_ptr = amb_ptr->clone();
}

ShadeInfo World::hit_objects(const Ray &ray, bool accelerate)
{
    if (accelerate)
        return this->accelerated_hit_objects(ray);

    return this->regular_hit_objects(ray);
}

ShadeInfo World::regular_hit_objects(const Ray &ray)
{
    float tmin = kHugeValue;
    ShadeInfo s(*this);
    Vector3D normal;
    Point3D local_hit;
    float t;

    for (auto i : geometry)
    {
        bool ray_intersection = i->hit(ray, t, s);

        if (ray_intersection && s.t < tmin)
        {
            tmin = s.t;
            //std::cout << object_shade.hit << "\n";
            s.material_ptr = i->get_material();
            s.hit = true;
            normal = s.normal;
            local_hit = s.hit_point;
        }
    }

    if (s.hit)
    {
        s.t = tmin;
        s.normal = normal;
        s.hit_point = local_hit;
    }

    return s;
}

ShadeInfo World::accelerated_hit_objects(const Ray &ray)
{
    ShadeInfo s(*this);

    return this->accelaration_ptr->hitObjects(ray, s);
}

void World::add_ply(std::string fname, Material *mPtr, Point3D bottom,
                    Point3D top)
{
    // Construct the data object by reading from file
    happly::PLYData plyIn(fname);

    // Get mesh-style data from the object
    std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
    std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();
    std::vector<Point3D> points;
    std::vector<Vector3D> normals;
    Vector3D normal;

    Point3D modelMin = Point3D(vPos[0][0], vPos[0][1], vPos[0][2]);
    Point3D modelMax = Point3D(vPos[0][0], vPos[0][1], vPos[0][2]);

    for (const std::array<double, 3> &point : vPos)
    {
        points.push_back(Point3D(point[0], point[1], point[2]));
        modelMin = min(modelMin, Point3D(point[0], point[1], point[2]));
        modelMax = max(modelMax, Point3D(point[0], point[1], point[2]));
    }

    for (Point3D &point : points)
    {
        point = interpolate(modelMin, modelMax, point, bottom, top);
    }

    // prepare normals array

    // loop through faces
    for (std::vector<size_t> face : fInd)
    {
        Triangle *fTriangle =
            new Triangle(points[face[0]], points[face[1]], points[face[2]]);
        fTriangle->set_material(mPtr->clone());
        add_geometry(fTriangle);
    }

    delete mPtr;
}

void World::addOBJ(const char *path, Material *mPtr)
{
    int count_duplicates = 0;
    std::vector<uint64_t> vertex_indices, normal_indices;
    std::vector<Point3D> vertices;
    std::vector<Vector3D> normals;

    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        std::cout << "Cannot open file " << path << "\n";
        return;
    }

    while (1)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0)
        {
            Point3D vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            Vector3D normal;
            fscanf(file, "%lf %lf %lf\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string v1, v2, v3;
            int vertexIndex[3], normalIndex[3];
            fscanf(file, "%d/%*d/%d %d/%*d/%d %d/%*d/%d\n", &vertexIndex[0],
                   &normalIndex[0], &vertexIndex[1], &normalIndex[1],
                   &vertexIndex[2], &normalIndex[2]);
            vertex_indices.push_back(vertexIndex[0]);
            vertex_indices.push_back(vertexIndex[1]);
            vertex_indices.push_back(vertexIndex[2]);

            normal_indices.push_back(normalIndex[0]);
            normal_indices.push_back(normalIndex[1]);
            normal_indices.push_back(normalIndex[2]);
        }
    }

    // now we have four arrays 1) Vertices 2) Normals 3) Vertex Indices 4) Normal Indices
    // to get faces we need to iterate over vertex indices and normal indices and create triangles
    for (size_t i = 0; i < vertex_indices.size(); i += 3)
    {
        Triangle *triangle = new Triangle(vertices[vertex_indices[i] - 1],
                                          vertices[vertex_indices[i + 1] - 1],
                                          vertices[vertex_indices[i + 2] - 1]);

        if ((vertices[vertex_indices[i] - 1] == vertices[vertex_indices[i + 1] - 1] || vertices[vertex_indices[i + 1] - 1] == vertices[vertex_indices[i + 2] - 1] || vertices[vertex_indices[i + 2] - 1] == vertices[vertex_indices[i] - 1]))
        {
            std::cout << "\r Duplicate Count " << count_duplicates;
            count_duplicates++;
        }
        triangle->set_material(mPtr);
        // triangle->set_normal(normals[normal_indices[i] - 1],
        //                      normals[normal_indices[i + 1] - 1],
        //                      normals[normal_indices[i + 2] - 1]);
        add_geometry(triangle);
    }
    std::cout << "\nAdded " << geometry.size() << " objects\n";
}