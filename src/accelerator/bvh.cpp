#include "bvh.hh"
#include "../utils/random.hh"

namespace isim {

AABB surrounding_box(const std::vector<Object*> objects) {

    Vector3 pmin = Vector3(INFINITY, INFINITY, INFINITY);
    Vector3 pmax = Vector3(-INFINITY, -INFINITY, -INFINITY);

    for (auto const& obj : objects) {
        pmin.x = std::min(pmin.x, obj->get_bounding_box()->pmin.x);
        pmin.y = std::min(pmin.y, obj->get_bounding_box()->pmin.y);
        pmin.z = std::min(pmin.z, obj->get_bounding_box()->pmin.z);

        pmax.x = std::max(pmax.x, obj->get_bounding_box()->pmax.x);
        pmax.y = std::max(pmax.y, obj->get_bounding_box()->pmax.y);
        pmax.z = std::max(pmax.z, obj->get_bounding_box()->pmax.z);
    }

    return AABB(pmin, pmax);
}

AABB surrounding_box(const AABB& box1, const AABB& box2) {

    Vector3 pmin(
        std::min(box1.pmin.x, box2.pmin.x),
        std::min(box1.pmin.y, box2.pmin.y),
        std::min(box1.pmin.z, box2.pmin.z)
    );

    Vector3 pmax(
        std::max(box1.pmax.x, box2.pmax.x),
        std::max(box1.pmax.y, box2.pmax.y),
        std::max(box1.pmax.z, box2.pmax.z)
    );

    return AABB(pmin, pmax);
}

BVHNode::BVHNode(std::vector<Object*> _aggregate)
 : aggregate(_aggregate), box(surrounding_box(_aggregate))
{}

BVHNode::BVHNode(AABB _box, BVHNode* _left, BVHNode* _right)
  : box(_box),
    left(_left),
    right(_right)
{}

BVHNode* construct_tree(std::vector<std::unique_ptr<Object>>& objects,                           
                        size_t start, size_t end) {

    size_t len = end - start;
    if (len <= OBJECTS_LIM) {
        std::vector<Object*> aggregate;
        auto it = objects.begin() + start;
        while (it < objects.begin() + end) {
            aggregate.push_back(it->get());
            it++;
        }
        return new BVHNode(aggregate);
    }

    int axis = rand() % 3;
    std::sort(objects.begin() + start, objects.begin() + end, 
        [axis](const std::unique_ptr<Object>& a,
               const std::unique_ptr<Object>& b) {
            return (a->get_bounding_box()->pmin[axis]
                    < b->get_bounding_box()->pmin[axis]);
    });

    size_t mid = len / 2;
    BVHNode* left = construct_tree(objects, start, mid);
    BVHNode* right = construct_tree(objects, mid, end);
    AABB box = surrounding_box(left->box, right->box);

    return new BVHNode(box, left, right);
}

std::optional<std::pair<Object*, Vector3>> 
nearest_intersection(const std::vector<Object*> objects,
                     const Ray &ray) {

    std::vector<std::pair<Object*, Vector3>> encountered;
    for (auto obj : objects) {
        std::optional<Vector3> intersection = obj->is_intersect(ray);
        if (intersection) {
            Vector3 pos = intersection.value();
            encountered.push_back(std::make_pair(obj, pos));
        }
    }

    if (encountered.empty())
        return std::nullopt;

    auto min = std::min_element(
        encountered.begin(),
        encountered.end(),
        [&ray](auto a, auto b) {
            return (a.second - ray.origin).euclidean_norm() 
                    < (b.second - ray.origin).euclidean_norm();
            } 
    );

    return std::make_optional(*min);
}

std::optional<std::pair<Object*, Vector3>> 
BVHNode::hit(const Ray& ray, float t_min, float t_max) {
    if (!left && !right) {
        return nearest_intersection(aggregate, ray); 
    }

    if (!box.hit(ray, t_min, t_max))
        return std::nullopt;

    auto left_hit = left ? left->hit(ray, t_min, t_max) : std::nullopt;
    auto right_hit = right ? right->hit(ray, t_min, t_max) : std::nullopt;

    std::optional<std::pair<Object*, Vector3>> res = std::nullopt;
    float t0 = t_min;
    if (left && (left_hit = left->hit(ray, t0, t_max))) {
        res = left_hit.value();
    }

    float t1 = t_min;
    if (right && (right_hit = right->hit(ray, t0, t_max)) && t1 < t0) {
        res = right_hit.value();
    }

    return res;
}
    
}