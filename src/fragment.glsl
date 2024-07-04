#version 460 core

#define PI 3.14159265359
#define EPSILON 0.0001

in vec2 uv;

out vec4 FragColor;

uniform float u_aspectRatio;
uniform vec2 u_mousePos;
uniform float u_fov;

uniform vec3 u_initCamPos;
uniform vec3 u_initCamDir;
uniform vec3 u_upDir;
//uniform Sphere spheres[50];

float deg2rad(float deg) {
    float rad = deg * PI / 180.0f;
    return rad;
}

struct Camera {
    vec3 position;
    vec3 llc;
    vec3 horizontal;
    vec3 vertical;
};

Camera camera(float fov, float aspectRatio, vec3 lookFrom, vec3 lookAt, vec3 vup) {
    float theta = deg2rad(fov);
    float h = tan(theta/2.0f);
    float viewportHeight = h * 2.0f;
    float viewportWidth = viewportHeight * aspectRatio;

    vec3 w = normalize(lookFrom - lookAt);
    vec3 u = normalize(cross(vup, w));
    vec3 v = cross(w, u);

    Camera cam;
    cam.position = lookFrom;
    cam.horizontal = u * viewportWidth;
    cam.vertical = v * viewportHeight;
    cam.llc = cam.position - cam.horizontal / 2.0f - cam.vertical / 2.0f - w;
    return cam;
}

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct HitInfo {
    bool hit;
    float distance;
    vec3 hitPoint;
    vec3 normal;
    //Material mat;
};

struct Material {
    vec3 color;
};

struct Sphere {
    vec3 center;
    float radius;
    Material mat;
};

mat2 rot2D (float angle) {
    float s = sin(angle);
    float c = cos(angle);

    return mat2(c, -s, s, c);
}

float scalarTriple(vec3 a, vec3 b, vec3 c) {
    return dot(cross(a, b), c);
}

HitInfo sphereIntersection(Ray ray, vec3 center, float radius) {
    HitInfo hitInfo = {false, 0, vec3(0), vec3(0)};

    /*
        * if (P-C).(P-C)=r^2 where P=A+tB vector and C=center
        * so expanding,
        * t^2(B.B) + 2tB.(A-C) + (A-C).(A-C) - r^2 = 0
        * so if D > 0, then the ray has hit the sphere    
    */
    vec3 oc = ray.origin - center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(ray.direction, oc);
    float c = dot(oc, oc) - radius*radius;

    float disc = b*b - 4*a*c;

    if (disc >= 0.0f) {
        float dist = ((-b - sqrt(disc)) / (2.0f * a)) > 0.0f ? ((-b - sqrt(disc)) / (2.0f * a)) : ((-b + sqrt(disc)) / (2.0f * a));
        hitInfo.hit = true;
        hitInfo.distance = dist;
        hitInfo.hitPoint = ray.origin + ray.direction * dist;
        hitInfo.normal = normalize(hitInfo.hitPoint - center);
    }
    return hitInfo;
}
/*
HitInfo rayCollision(Ray ray) {
    HitInfo closest = {false, (1./0.), vec3(0), vec3(0)};

    for (int i = 0; i < spheres.length(); i++) {
        Sphere sphere = spheres[i];
        HitInfo hitInfo = sphereIntersection(ray, sphere.center, sphere.radius);

        if (hitInfo.hit && hitInfo.distance < closest.distance) {
            closest = hitInfo;
            closest.mat = sphere.mat;
        }
    }

    return closest;
}*/

void main() {
    
    vec2 n = vec2(uv.x * u_aspectRatio, uv.y);
    vec2 m = vec2(u_mousePos.x * u_aspectRatio, u_mousePos.y); 

    // Initialization
    Camera cam = camera(u_fov, u_aspectRatio, u_initCamPos, u_initCamDir, u_upDir);

    Ray ray;
    ray.origin = cam.position;
    vec3 rayTarget = vec3(n, 0.0f);
    ray.direction = normalize(rayTarget - ray.origin);

    //Camera Rotation
    //ray.origin.yz *= rot2D(-m.y);
    //ray.direction.yz *= rot2D(-m.y);
    //ray.origin.xz *= rot2D(-m.x);
    //ray.direction.xz *= rot2D(-m.x);

    // Coloring
    bool sphereHit = sphereIntersection(ray, vec3(0, 0, 3.0f), 1).hit;
    vec3 col = sphereHit ? vec3(1) : vec3(0);

    FragColor = vec4(col, 1);
}
