#version 460 core

in vec2 uv;

out vec4 FragColor;

uniform float u_aspectRatio;
uniform float u_time;
uniform vec2 u_mousePos;

float sdSphere(vec3 p, float r) {
    return length(p) - r;
}

float sdGround(vec3 p) {
    return p.y;
}

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float smoothMin(float a, float b, float k) {
    float h = max(k - abs(a-b), 0.0) / k;
    return min(a, b) - h*h*h*k*(1.0f/6.0f);
}

float map(vec3 p) {
    vec3 spherePos = vec3(sin(u_time) * 4.f, 0, 0);
    float radius = 1.0f;
    float sphere = sdSphere(p - spherePos, radius);
    float box = sdBox(p, vec3(0.7f));
    float ground = sdGround(p + 0.75f);
    return smoothMin(ground, smoothMin(sphere, box, 1.0f), 0.5f);
}

mat2 rot2D (float angle) {
    float s = sin(angle);
    float c = cos(angle);

    return mat2(c, -s, s, c);
}

void main() {
    vec2 n = vec2(uv.x * u_aspectRatio, uv.y);
    vec2 m = vec2(u_mousePos.x * u_aspectRatio, u_mousePos.y); 

    // Initialization
    vec3 ro = vec3(0, 0, -3);         // ray origin
    vec3 rd = normalize(vec3(n, 1)); // ray direction
    vec3 col = vec3(0);               // final pixel color

    float t = 0.; // total distance travelled

    //Camera Rotation
    ro.y = 1.0f;
    ro.yz *= rot2D(-m.y);
    rd.yz *= rot2D(-m.y);
    ro.xz *= rot2D(-m.x);
    rd.xz *= rot2D(-m.x);

    // Raymarching
    int i = 0;
    for (i = 0; i < 80; i++) {
        vec3 p = ro + rd * t;     // position along the ray

        float d = map(p);         // current distance to the scene

        t += d;                   // "march" the ray

        if (d < .001) break;      // early stop if close enough
        if (t > 100.) break;      // early stop if too far
    }

    // Coloring
    //col = vec3(t * .2);           // color based on distance
    col = vec3(t*.04 + float(i)*.005);

    FragColor = vec4(col, 1);
}
