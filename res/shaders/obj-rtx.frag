#version 120

uniform vec2 u_resolution;

struct Camera {
    vec3 pos;
    vec3 dir;
    float focal;
};

uniform Camera u_cam;

// Texture 2D contenant la scène (3 pixels RGBA par triangle)
uniform sampler2D u_sceneTexture;
uniform int u_triangleCount;
uniform vec3 u_sceneMinBounds;
uniform vec3 u_sceneMaxBounds;

struct Triangle {
    vec3 v0;
    vec3 v1;
    vec3 v2;
    vec3 color;
};

// --- DÉCODAGE DES TRIANGLES DEPUIS LA TEXTURE ---
Triangle getTriangle(int index) {
    Triangle tri;

    // Y normalisé [0.0, 1.0] pour l'index de ligne dans la texture
    float y = (float(index) + 0.5) / float(u_triangleCount);

    // En GLSL 1.20, on utilise texture2D au lieu de texture
    vec4 p0 = texture2D(u_sceneTexture, vec2(0.5 / 3.0, y));
    vec4 p1 = texture2D(u_sceneTexture, vec2(1.5 / 3.0, y));
    vec4 p2 = texture2D(u_sceneTexture, vec2(2.5 / 3.0, y));

    tri.v0 = mix(u_sceneMinBounds, u_sceneMaxBounds, p0.xyz);
    tri.v1 = mix(u_sceneMinBounds, u_sceneMaxBounds, p1.xyz);
    tri.v2 = mix(u_sceneMinBounds, u_sceneMaxBounds, p2.xyz);
    tri.color = vec3(p0.w, p1.w, p2.w);

    return tri;
}

// --- ALGORITHME MÖLLER-TRUMBORE (RAYON-TRIANGLE) ---
// En GLSL 1.20, le mot-clé "out" dans une fonction est remplacé par "inout"
float hitTriangle(vec3 rayOrigin, vec3 rayDir, Triangle tri, inout vec3 outNormal) {
    vec3 edge1 = tri.v1 - tri.v0;
    vec3 edge2 = tri.v2 - tri.v0;
    vec3 h = cross(rayDir, edge2);
    float a = dot(edge1, h);

    if (a > -0.00001 && a < 0.00001)
    return -1.0;

    float f = 1.0 / a;
    vec3 s = rayOrigin - tri.v0;
    float u = f * dot(s, h);

    if (u < 0.0 || u > 1.0)
    return -1.0;

    vec3 q = cross(s, edge1);
    float v = f * dot(rayDir, q);

    if (v < 0.0 || u + v > 1.0)
    return -1.0;

    float t = f * dot(edge2, q);

    if (t > 0.001) {
        outNormal = normalize(cross(edge1, edge2));
        return t;
    }

    return -1.0;
}

void main()
{
    vec2 st = vec2(gl_FragCoord.x, u_resolution.y - gl_FragCoord.y);
    vec2 uv = (st - 0.5 * u_resolution) / u_resolution.y;

    // --- CONSTRUCTIONS DES RAYONS DE LA CAMÉRA ---
    vec3 rayOrigin = u_cam.pos;
    vec3 forward = normalize(u_cam.dir);
    vec3 worldUp = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(forward, worldUp));
    vec3 up = cross(right, forward);

    vec3 rayDir = normalize(forward * u_cam.focal + uv.x * right - uv.y * up);

    // --- BOUCLE D'INTERSECTION ---
    float closestT = 1e30;
    int hitIndex = -1;
    vec3 hitNormal = vec3(0.0);

    for (int i = 0; i < u_triangleCount; ++i) {
        Triangle tri = getTriangle(i);
        vec3 normal = vec3(0.0);
        float t = hitTriangle(rayOrigin, rayDir, tri, normal);

        if (t > 0.001 && t < closestT) {
            closestT = t;
            hitIndex = i;
            hitNormal = normal;
        }
    }

    // --- SHADING ---
    vec3 finalColor = vec3(0.1, 0.1, 0.1);

    if (hitIndex != -1) {
        Triangle hitTri = getTriangle(hitIndex);
        vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));

        if (dot(hitNormal, rayDir) > 0.0) {
            hitNormal = -hitNormal;
        }

        float diffuse = max(dot(hitNormal, lightDir), 0.0);
        finalColor = hitTri.color * (diffuse + 0.2);
    }

    gl_FragColor = vec4(finalColor, 1.0);
}
