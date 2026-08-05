#version 120

uniform vec2 u_resolution;

struct Camera {
    vec3 pos;
    vec3 dir;
    float focal;
};

uniform Camera u_cam;

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
};

#define MAX_SPHERES 10

uniform Sphere u_spheres[MAX_SPHERES];
uniform int u_sphereCount;

// hitSphere prend maintenant une origine et une direction de rayon distinctes par pixel
float hitSphere(vec3 rayOrigin, vec3 rayDir, Sphere sphere) {
    vec3 oc = rayOrigin - sphere.center;
    float a = dot(rayDir, rayDir);
    float b = 2.0 * dot(oc, rayDir);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

void main()
{
    vec2 st = vec2(gl_FragCoord.x, u_resolution.y - gl_FragCoord.y);
    vec2 uv = (st - 0.5 * u_resolution) / u_resolution.y;

    // ------------------------------------------------------------------------
    // CONSTRUCTIONS DES RAYONS DE LA CAMÉRA
    // ------------------------------------------------------------------------
    vec3 rayOrigin = u_cam.pos;

    // 1. Vecteur Avant (Direction dans laquelle regarde la caméra)
    vec3 forward = normalize(u_cam.dir);

    // 2. Vecteur Droite (Perpendiculaire à Forward et au Haut du monde)
    vec3 worldUp = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(forward, worldUp));

    // 3. Vecteur Haut local de la caméra
    vec3 up = cross(right, forward);

    // 4. Direction du rayon pour ce pixel (Combinaison du plan d'écran et du forward)
    vec3 rayDir = normalize(forward * u_cam.focal + uv.x * right - uv.y * up);

    // ------------------------------------------------------------------------
    // BOUCLE D'INTERSECTION
    // ------------------------------------------------------------------------
    float closestT = 1e30;
    int hitIndex = -1;

    for (int i = 0; i < MAX_SPHERES; ++i) {
        if (i >= u_sphereCount) break;

        float t = hitSphere(rayOrigin, rayDir, u_spheres[i]);

        if (t > 0.001 && t < closestT) {
            closestT = t;
            hitIndex = i;
        }
    }

    // ------------------------------------------------------------------------
    // SHADING
    // ------------------------------------------------------------------------
    vec3 finalColor = vec3(0.1, 0.1, 0.1);

    if (hitIndex != -1) {
        Sphere sphere = u_spheres[hitIndex];

        vec3 hitPoint = rayOrigin + closestT * rayDir;
        vec3 normal = normalize(hitPoint - sphere.center);
        vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // FIXME : source de lumiere doit etre variable

        float diffuse = max(dot(normal, lightDir), 0.0);
        finalColor = sphere.color * (diffuse + 0.2);
    }

    gl_FragColor = vec4(finalColor, 1.0);
}
