#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

// Todas as coordenadas desta estrutura pertencem ao sistema do mundo.
// A conversao para a viewport deve ser implementada pelos estudantes.
using namespace std;
using mat3 = glm::mat3;
using vec2 = glm::vec2;
using vec3 = glm::vec3;
struct PoligonoBase {
    vec3 cor;                 // RGB, componentes entre 0.0f e 1.0f
    vector<glm::vec2> vertices;
};

struct ObjetoBase {
    string nome;
    vector<PoligonoBase> poligonos;
    mat3 matrizAcumulada = mat3(1.0f);

    vec2 calcularCentro() const {
        vec2 centro(0.0f, 0.0f);
        int totalVertices = 0;

        for (const auto& pol : poligonos) {
            for (const auto& v : pol.vertices) {
                centro += v;
                totalVertices++;
            }
        }

        if (totalVertices > 0) {
            centro /= static_cast<float>(totalVertices);
        }
        return centro;
    }
};
