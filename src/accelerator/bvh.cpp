#include "bvh.hh"
#include "../utils/random.hh"

namespace isim {

AABB surrounding_box(const std::vector<Object*> objects) {

  Vector3 pmin = Vector3(INFINITY, INFINITY, INFINITY);
  Vector3 pmax = Vector3(-INFINITY, -INFINITY, -INFINITY);

  for (auto const& obj : objects) {
    pmin.x = std::min(pmin.x, obj->get_bounding_box()->pmin_.x);
    pmin.y = std::min(pmin.y, obj->get_bounding_box()->pmin_.y);
    pmin.z = std::min(pmin.z, obj->get_bounding_box()->pmin_.z);

    pmax.x = std::max(pmax.x, obj->get_bounding_box()->pmax_.x);
    pmax.y = std::max(pmax.y, obj->get_bounding_box()->pmax_.y);
    pmax.z = std::max(pmax.z, obj->get_bounding_box()->pmax_.z);
  }

  return AABB(pmin, pmax);
}

AABB surrounding_box(const AABB& box1, const AABB& box2) {

  Vector3 pmin(
    std::min(box1.pmin_.x, box2.pmin_.x),
    std::min(box1.pmin_.y, box2.pmin_.y),
    std::min(box1.pmin_.z, box2.pmin_.z)
  );

  Vector3 pmax(
    std::max(box1.pmax_.x, box2.pmax_.x),
    std::max(box1.pmax_.y, box2.pmax_.y),
    std::max(box1.pmax_.z, box2.pmax_.z)
  );

  return AABB(pmin, pmax);
}

BVHNode::BVHNode(std::vector<Object*> aggregate)
    : aggregate_(aggregate), box_(surrounding_box(aggregate)) {}

BVHNode::BVHNode(AABB box, BVHNode* left, BVHNode* right)
    : box_(box),
      left_(left),
      right_(right) {}

BVHNode* construct_tree(std::vector<std::unique_ptr<Object>>& objects,                           
                        int start, int end) {

  // Stopping case : construct leaves with remaining objects
  int len = end - start;
  if (len <= OBJECTS_LIM) {
    std::vector<Object*> aggregate;
    for (auto it = objects.begin() + start; it != objects.begin() + end; it++) {
      aggregate.push_back(it->get());
    }
    return new BVHNode(aggregate);
  }

  // Else create two children by splitting sorted lists of objects
  int axis = rand() % 3;
  std::sort(
    objects.begin() + start,
    objects.begin() + end, 
    [axis](const std::unique_ptr<Object>& a, const std::unique_ptr<Object>& b) {
      return (a->get_bounding_box()->pmin_[axis]
              < b->get_bounding_box()->pmin_[axis]);
  });

  int mid = start + (end - start) / 2;
  BVHNode* left = construct_tree(objects, start, mid);
  BVHNode* right = construct_tree(objects, mid, end);
  AABB box = surrounding_box(left->box_, right->box_);

  return new BVHNode(box, left, right);
}

std::optional<std::pair<Object*, Vector3>> nearest_intersection(
    const std::vector<Object*> objects,
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
  });

  return std::make_optional(*min);
}

std::optional<std::pair<Object*, Vector3>> BVHNode::hit(const Ray& ray,
                                                        float t_min,
                                                        float t_max) {

  // Box hit check
  if (!box_.hit(ray, t_min, t_max)) {
    return std::nullopt;
  }

  // Leaf case
  if (!left_ && !right_) {
    return nearest_intersection(aggregate_, ray); 
  }

  // Internal node case
  std::optional<std::pair<Object*, Vector3>> res = std::nullopt;

  if (left_) {
    res = left_->hit(ray, t_min, t_max);
  }

  std::optional<std::pair<Object*, Vector3>> right_hit;
  if (right_ && (right_hit = right_->hit(ray, t_min, t_max))) {
    if (!res) {
      res = right_hit;
    } else {
      // FIXME : this is awful, need to compare them another way
      float tl = (res.value().second - ray.origin).euclidean_norm();
      float tr = (right_hit.value().second - ray.origin).euclidean_norm();
      if (tr < tl) {
        res = right_hit.value();
      }
    }
  }

  return res;
}
    
}