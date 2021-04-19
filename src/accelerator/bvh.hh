#pragma once

#include <vector>
#include <optional>

#include "../objects/object.hh"
#include "aabb.hh"

#define OBJECTS_LIM 4

namespace isim {

class BVHNode {

  public:
    // Leaf
    BVHNode(std::vector<Object*> objects);

    // Internal node
    BVHNode(AABB box, BVHNode* left, BVHNode* right);

    std::optional<std::pair<Object*, Vector3>> hit(const Ray& ray, float t_min,
                                                   float t_max);

    std::vector<Object*> aggregate_;
    BVHNode* left_ = nullptr;
    BVHNode* right_ = nullptr;
    AABB box_;

};

BVHNode* construct_tree(std::vector<std::unique_ptr<Object>>& objects,                           
                        int start, int end);

std::optional<std::pair<Object*, Vector3>> nearest_intersection(
    std::vector<Object*> objects, const Ray &ray);
    
}