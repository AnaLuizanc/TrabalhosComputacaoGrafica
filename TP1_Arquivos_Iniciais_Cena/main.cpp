#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "transformacoes.hpp"
#include "scene_types.hpp"
#include "cena_inicial.hpp"

using namespace std;

vector<ObjetoBase> cena;

// Limites do mundo
float mundoXMin = -100.0f, mundoXMax = 100.0f;
float mundoYMin = -100.0f, mundoYMax = 100.0f;

// Limites da Viewport gráfica
float vpXMin = 0.0f, vpXMax = 600.0f; 
float vpYMin = 0.0f, vpYMax = 600.0f;

int objetoSelecionado = 0; 
bool modoDemonstracao = false; 
bool arrastando = false;
glm::vec2 ultimoMouseMundo(0.0f, 0.0f);

float passoTranslacao = 5.0f;
float passoEscala = 1.1f;
float passoRotacao = 0.1f; 


void desenhaTexto(const char* texto, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    while (*texto) {

        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *texto);
        texto++;
    }
}

void desenhaPainel() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
        glVertex2f(600.0f, 0.0f);
        glVertex2f(600.0f, 600.0f);
    glEnd();

    desenhaTexto("PAINEL DE CONTROLE", 620.0f, 30.0f);
    desenhaTexto("------------------", 620.0f, 40.0f);
    
    desenhaTexto("OBJETO ATIVO:", 610.0f, 70.0f);
    if (objetoSelecionado == 0) desenhaTexto("[1] Casa", 610.0f, 90.0f);
    else if (objetoSelecionado == 1) desenhaTexto("[2] Barco", 610.0f, 90.0f);
    else if (objetoSelecionado == 2) desenhaTexto("[3] Moinho", 610.0f, 90.0f);
    
    desenhaTexto("COMANDOS:", 610.0f, 140.0f);
    desenhaTexto("1, 2, 3: Selecionar", 610.0f, 160.0f);
    desenhaTexto("Setas  : Transladar", 610.0f, 180.0f);
    desenhaTexto("R / r  : Rotacionar", 610.0f, 200.0f);
    desenhaTexto("+ / -  : Escala", 610.0f, 220.0f);
    desenhaTexto("X / Y  : Espelhar", 610.0f, 240.0f);
    desenhaTexto("H / h  : Cisalhar", 610.0f, 260.0f);
    desenhaTexto("0      : Reiniciar", 610.0f, 280.0f);
    
    desenhaTexto("------------------", 620.0f, 320.0f);
    desenhaTexto("D : Mostrar Ordem", 610.0f, 340.0f);
    desenhaTexto("    (T*S vs S*T)", 610.0f, 355.0f);
}

void desenhaDemonstracao() {
    if (cena.empty()) return;
    ObjetoBase obj = cena[0];
    
    desenhaTexto("DEMONSTRACAO: A ordem importa (Aperte 'D' para voltar)", 50.0f, 30.0f);
    desenhaTexto("Esquerda: Escala -> Translacao (T * S)", 20.0f, 70.0f);
    desenhaTexto("Direita : Translacao -> Escala (S * T)", 320.0f, 70.0f);

    glm::mat3 T = Transformacoes::translacao(30.0f, 30.0f);
    glm::mat3 S = Transformacoes::escala(2.0f, 2.0f);

    glm::mat3 matrizTS = T * S;
    glm::mat3 matrizST = S * T;

    auto desenhaObj = [&](glm::mat3 matriz, float offsetX) {
        for (const auto& poligono : obj.poligonos) {
            glColor3f(poligono.cor.r, poligono.cor.g, poligono.cor.b);
            glBegin(GL_POLYGON);
            for (const auto& vertice : poligono.vertices) {
                glm::vec3 ponto(vertice.x + offsetX, vertice.y, 1.0f);
                glm::vec3 pTransf = matriz * ponto;
                
                glm::vec2 pVP = Transformacoes::mundoParaViewport(
                    glm::vec2(pTransf.x, pTransf.y),
                    mundoXMin, mundoXMax, mundoYMin, mundoYMax, 
                    vpXMin, vpXMax, vpYMin, vpYMax 
                );
                glVertex2f(pVP.x, pVP.y);
            }
            glEnd();
        }
    };

    desenhaObj(matrizTS, -50.0f); 
    desenhaObj(matrizST,  10.0f); 
}


void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (modoDemonstracao) {
        desenhaDemonstracao();
    } else {
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_LINES);
            glm::vec2 vpy1 = Transformacoes::mundoParaViewport(glm::vec2(0, mundoYMin), 
                mundoXMin, mundoXMax, mundoYMin, mundoYMax, vpXMin, vpXMax, vpYMin, vpYMax);
            glm::vec2 vpy2 = Transformacoes::mundoParaViewport(glm::vec2(0, mundoYMax),
                mundoXMin, mundoXMax, mundoYMin, mundoYMax, vpXMin, vpXMax, vpYMin, vpYMax);
            glm::vec2 vpx1 = Transformacoes::mundoParaViewport(glm::vec2(mundoXMin, 0),
                mundoXMin, mundoXMax, mundoYMin, mundoYMax, vpXMin, vpXMax, vpYMin, vpYMax);
            glm::vec2 vpx2 = Transformacoes::mundoParaViewport(glm::vec2(mundoXMax, 0),
                mundoXMin, mundoXMax, mundoYMin, mundoYMax, vpXMin, vpXMax, vpYMin, vpYMax);
            glVertex2f(vpy1.x, vpy1.y); glVertex2f(vpy2.x, vpy2.y);
            glVertex2f(vpx1.x, vpx1.y); glVertex2f(vpx2.x, vpx2.y);
        glEnd();

        for (const auto& objeto : cena) {
            for (const auto& poligono : objeto.poligonos) {
                glColor3f(poligono.cor.r, poligono.cor.g, poligono.cor.b);
                glBegin(GL_POLYGON);
                for (const auto& vertice : poligono.vertices) {
                    glm::vec3 pontoHomogeneo(vertice.x, vertice.y, 1.0f);
                    glm::vec3 pontoTransformado = objeto.matrizAcumulada * pontoHomogeneo;
                    
                    glm::vec2 pontoViewport = Transformacoes::mundoParaViewport(
                        glm::vec2(pontoTransformado.x, pontoTransformado.y),
                        mundoXMin, mundoXMax, mundoYMin, mundoYMax, 
                        vpXMin, vpXMax, vpYMin, vpYMax
                    );
                    
                    glVertex2f(pontoViewport.x, pontoViewport.y);
                }
                glEnd();
            }
        }
    }

    desenhaPainel();
    glFlush();
}

void teclado(unsigned char key, int x, int y) {
    if (cena.empty()) return;
    
    if (key == 'd' || key == 'D') {
        modoDemonstracao = !modoDemonstracao;
        glutPostRedisplay();
        return;
    }

    ObjetoBase& obj = cena[objetoSelecionado];

    switch (key) {
        case '1': objetoSelecionado = 0; break; 
        case '2': objetoSelecionado = 1; break; 
        case '3': objetoSelecionado = 2; break;
        case 'r': {
            glm::vec2 centro = obj.calcularCentro();
            obj.matrizAcumulada = Transformacoes::rotacaoNoCentro(centro.x, centro.y, passoRotacao) * obj.matrizAcumulada;
            break;
        }
        case 'R': {
            glm::vec2 centro = obj.calcularCentro();
            obj.matrizAcumulada = Transformacoes::rotacaoNoCentro(centro.x, centro.y, -passoRotacao) * obj.matrizAcumulada;
            break;
        }
        case '+': 
            obj.matrizAcumulada = Transformacoes::escala(passoEscala, passoEscala) * obj.matrizAcumulada;
            break;
        case '-': 
            obj.matrizAcumulada = Transformacoes::escala(1.0f / passoEscala, 1.0f / passoEscala) * obj.matrizAcumulada;
            break;
        case 'x': 
        case 'X':
            obj.matrizAcumulada = Transformacoes::reflexao(true, false) * obj.matrizAcumulada;
            break;
        case 'y': 
        case 'Y':
            obj.matrizAcumulada = Transformacoes::reflexao(false, true) * obj.matrizAcumulada;
            break;
        case 'h':
            obj.matrizAcumulada = Transformacoes::cisalhamento(0.2f, 0.0f) * obj.matrizAcumulada;
            break;
        case 'H':
            obj.matrizAcumulada = Transformacoes::cisalhamento(-0.2f, 0.0f) * obj.matrizAcumulada;
            break;
        case '0': 
            obj.matrizAcumulada = glm::mat3(1.0f);
            break;
    }
    
    glutPostRedisplay(); 
}

void teclasEspeciais(int key, int x, int y) {
    if (cena.empty()) return;
    
    ObjetoBase& obj = cena[objetoSelecionado];

    switch (key) {
        case GLUT_KEY_UP:
            obj.matrizAcumulada = Transformacoes::translacao(0, passoTranslacao) * obj.matrizAcumulada;
            break;
        case GLUT_KEY_DOWN:
            obj.matrizAcumulada = Transformacoes::translacao(0, -passoTranslacao) * obj.matrizAcumulada;
            break;
        case GLUT_KEY_LEFT:
            obj.matrizAcumulada = Transformacoes::translacao(-passoTranslacao, 0) * obj.matrizAcumulada;
            break;
        case GLUT_KEY_RIGHT:
            obj.matrizAcumulada = Transformacoes::translacao(passoTranslacao, 0) * obj.matrizAcumulada;
            break;
    }
    
    glutPostRedisplay();
}

bool pontoNoObjeto(glm::vec2 pMundo, const ObjetoBase& obj) {
    float minX = 9999, maxX = -9999, minY = 9999, maxY = -9999;
    
    for (const auto& pol : obj.poligonos) {
        for (const auto& v : pol.vertices) {
            glm::vec3 pTransformado = obj.matrizAcumulada * glm::vec3(v.x, v.y, 1.0f);
            if (pTransformado.x < minX) minX = pTransformado.x;
            if (pTransformado.x > maxX) maxX = pTransformado.x;
            if (pTransformado.y < minY) minY = pTransformado.y;
            if (pTransformado.y > maxY) maxY = pTransformado.y;
        }
    }
    return (pMundo.x >= minX && pMundo.x <= maxX && pMundo.y >= minY && pMundo.y <= maxY);
}

void mouseClick(int button, int state, int x, int y) {
    if (modoDemonstracao || cena.empty() || x > vpXMax) return;
    glm::vec2 mouseMundo = Transformacoes::viewportParaMundo(
        glm::vec2(x, y), mundoXMin, mundoXMax, mundoYMin, mundoYMax, vpXMin, vpXMax, vpYMin, vpYMax
    );
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            for (size_t i = 0; i < cena.size(); ++i) {
                if (pontoNoObjeto(mouseMundo, cena[i])) {
                    objetoSelecionado = i;
                    arrastando = true;
                    ultimoMouseMundo = mouseMundo;
                    glutPostRedisplay();
                    break;
                }
            }
        } else if (state == GLUT_UP) {
            arrastando = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (!arrastando || cena.empty()) return;
    glm::vec2 mouseMundo = Transformacoes::viewportParaMundo(
        glm::vec2(x, y), mundoXMin, mundoXMax, mundoYMin, mundoYMax, vpXMin, vpXMax, vpYMin, vpYMax
    );

    float dx = mouseMundo.x - ultimoMouseMundo.x;
    float dy = mouseMundo.y - ultimoMouseMundo.y;
    cena[objetoSelecionado].matrizAcumulada = Transformacoes::translacao(dx, dy) * cena[objetoSelecionado].matrizAcumulada;

    ultimoMouseMundo = mouseMundo;
    glutPostRedisplay();
}

void inicializa() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 600.0, 0.0);
    glMatrixMode(GL_MODELVIEW);

    cena = criarCenaInicial(); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("TP1 - Transformacoes 2D");
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    inicializa();
    glutDisplayFunc(desenha);
    
    glutMainLoop();
    
    return 0;
}