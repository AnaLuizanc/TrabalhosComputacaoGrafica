#ifndef TRANSFORMACOES_HPP
#define TRANSFORMACOES_HPP

#include <glm/glm.hpp>
#include <cmath>

namespace Transformacoes {

using glm::mat3;
using glm::vec2;
using glm::vec3;

inline mat3 translacao(float dx, float dy) {
    mat3 T(1.0f);
    T[2] = vec3(dx, dy, 1.0f);
    return T;
}

inline mat3 escala(float sx, float sy) {
    mat3 S(1.0f);
    S[0][0] = sx;
    S[1][1] = sy;
    return S;
}

inline mat3 rotacao(float angulo_radianos) {
    mat3 R(1.0f);
    R[0][0] = std::cos(angulo_radianos);
    R[0][1] = std::sin(angulo_radianos);
    R[1][0] = -std::sin(angulo_radianos);
    R[1][1] = std::cos(angulo_radianos);
    return R;
}

inline mat3 cisalhamento(float hx, float hy) {
    mat3 Sh(1.0f);
    Sh[1][0] = hx;
    Sh[0][1] = hy;
    return Sh;
}

inline mat3 reflexao(bool eixoX, bool eixoY) {
    mat3 Ref(1.0f);
    if (eixoX) Ref[1][1] = -1.0f;
    if (eixoY) Ref[0][0] = -1.0f;
    return Ref;
}

inline mat3 rotacaoNoCentro(float cx, float cy, float angulo_radianos) {
    mat3 ida = translacao(-cx, -cy);
    mat3 rot = rotacao(angulo_radianos);
    mat3 volta = translacao(cx, cy);
    return volta * rot * ida;
}

inline vec2 mundoParaViewport(vec2 pontoMundo,
                                  float w_xmin, float w_xmax, float w_ymin, float w_ymax,
                                  float v_xmin, float v_xmax, float v_ymin, float v_ymax) {
    float x_v = ((pontoMundo.x - w_xmin) / (w_xmax - w_xmin)) * (v_xmax - v_xmin) + v_xmin;
    float y_v = ((w_ymax - pontoMundo.y) / (w_ymax - w_ymin)) * (v_ymax - v_ymin) + v_ymin;
    return vec2(x_v, y_v);
}

}

#endif