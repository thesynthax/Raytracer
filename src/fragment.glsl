#version 460 core

#define PI 3.14159265359
#define EPSILON 0.0001
#define MAX_OBJECT_COUNT 100
#define MAX_BOUNCE_LIMIT 20

in vec2 uv;

out vec4 FragColor;

uniform vec2 u_screenPixels;
uniform float u_aspectRatio;
uniform vec2 u_mousePos;
uniform float u_fov;

uniform vec3 u_initCamPos;
uniform vec3 u_initCamDir;
uniform vec3 u_upDir;

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

struct Material {
    vec3 color;
    vec3 emission;

};
struct HitInfo {
    bool hit;
    float distance;
    vec3 hitPoint;
    vec3 normal;
    Material mat;
};

struct Sphere {
    vec3 center;
    float radius;
    Material mat;
};

uniform Sphere u_spheres[MAX_OBJECT_COUNT];

uint nextRandom(inout uint state)
{
    state = state * 747796405 + 2891336453;
    uint result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
    result = (result >> 22) ^ result;
    return result;
}

float randomValue(inout uint state)
{
    return nextRandom(state) / 4294967295.0; // 2^32 - 1
}

float randomValueNormalDistribution(inout uint state)
{
    float theta = 2 * 3.1415926 * randomValue(state);
    float rho = sqrt(-2 * log(randomValue(state)));
    return rho * cos(theta);
}

vec3 randomPointInSphere(inout uint state)
{
    float x = randomValueNormalDistribution(state);
    float y = randomValueNormalDistribution(state);
    float z = randomValueNormalDistribution(state);
    return normalize(vec3(x, y, z));
}

vec2 randomPointInCircle(inout uint rngState)
{
    float angle = randomValue(rngState) * 2 * PI;
    vec2 pointOnCircle = vec2(cos(angle), sin(angle));
    return pointOnCircle * sqrt(randomValue(rngState));
}

mat2 rot2D (float angle) {
    float s = sin(angle);
    float c = cos(angle);

    return mat2(c, -s, s, c);
}

float lerp(float a, float b, float t) {
    return a + (b-a) * t;
}

vec3 lerpVec(vec3 a, vec3 b, float t) {
    return a + (b-a) * t;
}


float scalarTriple(vec3 a, vec3 b, vec3 c) {
    return dot(cross(a, b), c);
}

HitInfo sphereIntersection(Ray ray, Sphere sphere) {
    HitInfo hitInfo = {false, 0, vec3(0), vec3(0), {vec3(0), vec3(0)}};

    /*
        * if (P-C).(P-C)=r^2 where P=A+tB vector and C=center
        * so expanding,
        * t^2(B.B) + 2tB.(A-C) + (A-C).(A-C) - r^2 = 0
        * so if D > 0, then the ray has hit the sphere    
    */
    vec3 oc = ray.origin - sphere.center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(ray.direction, oc);
    float c = dot(oc, oc) - sphere.radius*sphere.radius;

    float disc = b*b - 4*a*c;

    if (disc >= 0.0f) {
        float dist = ((-b - sqrt(disc)) / (2.0f * a)) > 0.0f ? ((-b - sqrt(disc)) / (2.0f * a)) : ((-b + sqrt(disc)) / (2.0f * a));
        hitInfo.hit = true;
        hitInfo.distance = dist;
        hitInfo.hitPoint = ray.origin + ray.direction * dist;
        hitInfo.normal = normalize(hitInfo.hitPoint - sphere.center);
        hitInfo.mat = sphere.mat;
    }
    return hitInfo;
}

vec3 GetEnvironmentLight(vec3 dir)
{
    //if (UseSky == 0) return 0;
    const vec3 GroundColour = vec3(0.35f, 0.3f, 0.35f);
    const vec3 SkyColourHorizon = vec3(1, 1, 1);
    const vec3 SkyColourZenith = vec3(0.08f, 0.37f, 0.73f); 

    float skyGradientT = pow(smoothstep(0.0f, 0.4f, dir.y), 0.35f);
    float groundToSkyT = smoothstep(-0.01f, 0.0f, dir.y);
    vec3 skyGradient = lerpVec(SkyColourHorizon, SkyColourZenith, skyGradientT);
    //float sun = pow(max(0, dot(dir, _WorldSpaceLightPos0.xyz)), SunFocus) * SunIntensity;
    // Combine ground, sky, and sun
    vec3 composite = lerpVec(GroundColour, skyGradient, groundToSkyT);// + sun * SunColour * (groundToSkyT >= 1);
    return composite;
}

HitInfo rayCollision(Ray ray) {
    HitInfo closest = {false, (100000), vec3(0), vec3(0), {vec3(0), vec3(0)}};

    for (int i = 0; i < u_spheres.length(); i++) {
        Sphere sphere = u_spheres[i];
        HitInfo hitInfo = sphereIntersection(ray, sphere);

        if (hitInfo.hit && hitInfo.distance < closest.distance) {
            closest = hitInfo;
        }
    }

    return closest;
}

vec3 computeRayColor(Ray ray, inout uint seed) {
    vec3 incomingLight = vec3(0);
    vec3 rayColor = vec3(1);

    for (int i = 0; i <= 20; i++) {
        HitInfo hitInfo = rayCollision(ray);

        if (hitInfo.hit) {
            ray.origin = hitInfo.hitPoint;
            ray.direction = hitInfo.normal + randomPointInSphere(seed);

            vec3 emittedLight = hitInfo.mat.emission;
            rayColor *= hitInfo.mat.color;
            incomingLight += emittedLight * rayColor;
        } else {
            //incomingLight += GetEnvironmentLight(ray.direction) * rayColor;
            break;
        }
    }

    return incomingLight;
}

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
    /*
    ray.origin.yz *= rot2D(-m.y);
    ray.direction.yz *= rot2D(-m.y);
    ray.origin.xz *= rot2D(-m.x);
    ray.direction.xz *= rot2D(-m.x);
    */

    //Taking current pixel as seed for RNG
    vec2 pixelCoord = uv * u_screenPixels;
    uint pixelIndex = int(pixelCoord.y) * int(u_screenPixels.x) + int(pixelCoord.x);
    uint seed = pixelIndex;

    // Coloring
    //HitInfo sphereHit = rayCollision(ray);
    vec3 col = computeRayColor(ray, seed); 
    FragColor = vec4(col, 1); 
    //FragColor = sphereHit.hit ? vec4(sphereHit.mat.color, 1) : vec4(0, 0, 0, 1);
}
