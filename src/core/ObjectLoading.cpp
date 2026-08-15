//
// Created by Antonin Fruchet on 15/08/2026.
//
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "ObjectLoading.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

void loadOBJ(std::vector<Triangle>& buffer, const std::string& filePath, const sf::Vector3f& defaultColor){
    // Structures de sortie remplies par l'API tinyobj
    tinyobj::attrib_t attrib;                  // Contient les sommets, normales, UVs
    std::vector<tinyobj::shape_t> shapes;      // Contient les maillages / formes
    std::vector<tinyobj::material_t> materials;// Contient les matériaux .mtl
    std::string err;

    // --- C'EST ICI L'API DE BASE DE TINYOBJLOADER ---
    // Paramètres : &attrib, &shapes, &materials, &err, chemin_fichier, dossier_mtl (optionnel), trianguler (true)
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str(), nullptr, true);

    if (!err.empty()) {
        std::cerr << "ERR TinyObj: " << err << std::endl;
    }

    if (!ret) {
        throw std::runtime_error("Impossible de charger le fichier .obj : " + filePath);
    }

    // --- EXTRACTION ET CONVERSION DANS NOTRE STRUCT TRIANGLE ---
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;

        // Pour chaque face/triangle dans le maillage
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

            // tinyobj a déjà tout triangulé grâce au paramètre 'true' dans LoadObj
            tinyobj::index_t idx0 = shapes[s].mesh.indices[index_offset + 0];
            tinyobj::index_t idx1 = shapes[s].mesh.indices[index_offset + 1];
            tinyobj::index_t idx2 = shapes[s].mesh.indices[index_offset + 2];

            // Récupération des 3 sommets (V0, V1, V2)
            sf::Vector3f v0{
                attrib.vertices[3 * idx0.vertex_index + 0],
                attrib.vertices[3 * idx0.vertex_index + 1],
                attrib.vertices[3 * idx0.vertex_index + 2]
            };

            sf::Vector3f v1{
                attrib.vertices[3 * idx1.vertex_index + 0],
                attrib.vertices[3 * idx1.vertex_index + 1],
                attrib.vertices[3 * idx1.vertex_index + 2]
            };

            sf::Vector3f v2{
                attrib.vertices[3 * idx2.vertex_index + 0],
                attrib.vertices[3 * idx2.vertex_index + 1],
                attrib.vertices[3 * idx2.vertex_index + 2]
            };

            // Couleur optionnelle s'il y a un matériau
            sf::Vector3f color = defaultColor;
            if (!shapes[s].mesh.material_ids.empty()) {
                int mat_id = shapes[s].mesh.material_ids[f];
                if (mat_id >= 0 && mat_id < static_cast<int>(materials.size())) {
                    color.x = materials[mat_id].diffuse[0];
                    color.y = materials[mat_id].diffuse[1];
                    color.z = materials[mat_id].diffuse[2];
                }
            }

            buffer.emplace_back(v0, v1, v2, color);

            index_offset += 3; // On avance de 3 sommets pour le prochain triangle
        }
    }
}

namespace {
std::uint8_t encodeColorChannel(float value) {
    return static_cast<std::uint8_t>(std::clamp(value, 0.f, 1.f) * 255.f + 0.5f);
}

std::uint8_t encodePositionChannel(float value, float minBound, float maxBound) {
    float range = maxBound - minBound;
    if (std::abs(range) < 0.00001f) {
        return 0;
    }

    float normalized = (value - minBound) / range;
    return encodeColorChannel(normalized);
}

void appendEncodedVertex(std::vector<std::uint8_t>& textureData,
                         const sf::Vector3f& vertex,
                         float colorChannel,
                         const SceneTextureInfo& info) {
    textureData.push_back(encodePositionChannel(vertex.x, info.minBounds.x, info.maxBounds.x));
    textureData.push_back(encodePositionChannel(vertex.y, info.minBounds.y, info.maxBounds.y));
    textureData.push_back(encodePositionChannel(vertex.z, info.minBounds.z, info.maxBounds.z));
    textureData.push_back(encodeColorChannel(colorChannel));
}
}

SceneTextureInfo createSceneTexture(const std::vector<Triangle>& scene, sf::Texture& texture) {
    SceneTextureInfo info;

    if (scene.empty()) {
        texture = sf::Texture();
        return info;
    }

    info.minBounds = {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    };
    info.maxBounds = {
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest()
    };

    auto includeVertex = [&info](const sf::Vector3f& vertex) {
        info.minBounds.x = std::min(info.minBounds.x, vertex.x);
        info.minBounds.y = std::min(info.minBounds.y, vertex.y);
        info.minBounds.z = std::min(info.minBounds.z, vertex.z);
        info.maxBounds.x = std::max(info.maxBounds.x, vertex.x);
        info.maxBounds.y = std::max(info.maxBounds.y, vertex.y);
        info.maxBounds.z = std::max(info.maxBounds.z, vertex.z);
    };

    for (const auto& tri : scene) {
        includeVertex(tri.v0);
        includeVertex(tri.v1);
        includeVertex(tri.v2);
    }

    // 3 pixels par triangle, 4 composantes (RGBA) par pixel
    size_t numTriangles = scene.size();
    size_t width = 3;
    size_t height = numTriangles;

    std::vector<std::uint8_t> textureData;
    textureData.reserve(width * height * 4);

    for (const auto& tri : scene) {
        // Pixel 0: V0 + Color.r
        appendEncodedVertex(textureData, tri.v0, tri.color.x, info);

        // Pixel 1: V1 + Color.g
        appendEncodedVertex(textureData, tri.v1, tri.color.y, info);

        // Pixel 2: V2 + Color.b
        appendEncodedVertex(textureData, tri.v2, tri.color.z, info);
    }

    if (!texture.resize({static_cast<unsigned int>(width), static_cast<unsigned int>(height)})) {
        throw std::runtime_error("Impossible de créer la texture contenant les triangles de la scène.");
    }
    texture.setSmooth(false); // Indispensable : désactiver l'interpolation linéaire (Nearest neighbor)

    texture.update(textureData.data());

    return info;
}
