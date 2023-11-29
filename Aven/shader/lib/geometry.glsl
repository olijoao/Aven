

#ifndef GEOMETRY_GLSL
#define GEOMETRY_GLSL


struct Ray {
    vec3 origin;
    vec3 dir;
};


struct AABB {
    vec3 minPos, maxPos;
};

struct Sphere {
    vec3 center;
    float radius;
};


// Intersection (Ray - Sphere)
bool intersect(Ray r, Sphere s) {
    vec3 c_r = r.origin - s.center;
    float b = 2.0 * dot(r.dir, c_r);
    float delta = b * b - 4.0 * (dot(c_r, c_r) - (s.radius * s.radius));
    return (delta >= 0.0)
        && (0.5 * (-b - sqrt(delta))) > 0;
}

// Intersection (Ray - Sphere)
// Paramaters
//		out vec2 t: two intersections points
bool intersect(Ray r, Sphere s, out vec2 t) {
    vec3 c_r = r.origin - s.center;
    float b = 2.0 * dot(r.dir, c_r);
    float delta = b * b - 4.0 * (dot(c_r, c_r) - (s.radius * s.radius));
    if (delta < 0.0)
        return false;
    t = vec2(0.5 * (-b - sqrt(delta)), 0.5 * (-b + sqrt(delta)));
    return (t.x > 0);
}


//out vec2 t : two intersections points
bool intersect(Ray r, AABB aabb, out vec2 t) {
    vec3 tMin = (aabb.minPos - r.origin) / r.dir;
    vec3 tMax = (aabb.maxPos - r.origin) / r.dir;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    t = vec2(max(max(t1.x, t1.y), t1.z), min(min(t2.x, t2.y), t2.z));
    t.x = max(t.x, 0);
    return (t.x < t.y) && t.x >= 0;
}

bool intersect(Ray r, AABB aabb) {
    vec2 t;
    return intersect(r, aabb, t);
}

#endif	//GEOMETRY_GLSL