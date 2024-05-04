//
//  obb_picking.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/30/24.
//

#ifndef obb_picking_hpp
#define obb_picking_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "../component/component.hpp"
#include "../flecs.h"

bool test_rayobb_intersection(
      glm::vec3 ray_origin,        // Ray origin, in world space
      glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
      glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
      glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
      glm::mat4 model,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
      float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
  ){
    float tMin = 0.0f;
    float tMax = 100000.0f;
    glm::vec3 OBBposition_worldspace(model[3].x, model[3].y, model[3].z);
    glm::vec3 delta = OBBposition_worldspace - ray_origin;

    // Test intersection with the 2 planes perpendicular to the OBB's X axis
    {
        glm::vec3 xaxis(model[0].x, model[0].y, model[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(ray_direction, xaxis);

        if ( fabs(f) > 0.001f ){ // Standard case
            float t1 = (e+aabb_min.x)/f; // Intersection with the "left" plane
            float t2 = (e+aabb_max.x)/f; // Intersection with the "right" plane
            // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

            // We want t1 to represent the nearest intersection,
            // so if it's not the case, invert t1 and t2
            if (t1>t2){
                float w=t1;t1=t2;t2=w; // swap t1 and t2
            }

            // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
            if ( t2 < tMax )
                tMax = t2;
            // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
            if ( t1 > tMin )
                tMin = t1;

            // And here's the trick :
            // If "far" is closer than "near", then there is NO intersection.
            // See the images in the tutorials for the visual explanation.
            if (tMax < tMin )
                return false;

        }else{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
            if(-e+aabb_min.x > 0.0f || -e+aabb_max.x < 0.0f)
                return false;
        }
    }

    // Test intersection with the 2 planes perpendicular to the OBB's Y axis
    // Exactly the same thing than above.
    {
        glm::vec3 yaxis(model[1].x, model[1].y, model[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(ray_direction, yaxis);

        if ( fabs(f) > 0.001f ){
            float t1 = (e+aabb_min.y)/f;
            float t2 = (e+aabb_max.y)/f;

            if (t1>t2){float w=t1;t1=t2;t2=w;}

            if ( t2 < tMax )
                tMax = t2;
            if ( t1 > tMin )
                tMin = t1;
            if (tMin > tMax)
                return false;

        }else{
            if(-e+aabb_min.y > 0.0f || -e+aabb_max.y < 0.0f)
                return false;
        }
    }

    // Test intersection with the 2 planes perpendicular to the OBB's Z axis
    // Exactly the same thing than above.
    {
        glm::vec3 zaxis(model[2].x, model[2].y, model[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(ray_direction, zaxis);

        if ( fabs(f) > 0.001f ){

            float t1 = (e+aabb_min.z)/f;
            float t2 = (e+aabb_max.z)/f;

            if (t1>t2){float w=t1;t1=t2;t2=w;}

            if ( t2 < tMax )
                tMax = t2;
            if ( t1 > tMin )
                tMin = t1;
            if (tMin > tMax)
                return false;

        }else{
            if(-e+aabb_min.z > 0.0f || -e+aabb_max.z < 0.0f)
                return false;
        }
    }

    intersection_distance = tMin;
    return true;
}

#endif /* obb_picking_hpp */
