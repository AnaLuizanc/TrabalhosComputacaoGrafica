#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "transformacoes.hpp"
#include "scene_types.hpp"
#include "cena_inicial.hpp"

using namespace std;

vector<ObjetoBase> cena;

float mundoXMin = -100.0f, mundoXMax = 100.0f;
float mundoYMin = -100.0f, mundoYMax = 100.0f;

float vpXMin = 20.0f, vpXMax = 580.0f;
float vpYMin = 20.0f, vpYMax = 580.0f;

int objetoSelecionado = 0; 
bool modoDemonstracao = false; 
bool arrastando = false;
glm::vec2 ultimoMouseMundo(0.0f, 0.0f);

float passoTranslacao = 5.0f;
float passoEscala = 1.1f;
float passoRotacao = 0.1f; 

void desenhaBordaViewport() {
    glColor3f(1.0f, 1.0f, 1.0f); 
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(vpXMin, vpYMin); // Canto superior esquerdo
        glVertex2f(vpXMax, vpYMin); // Canto superior direito
        glVertex2f(vpXMax, vpYMax); // Canto inferior direito
        glVertex2f(vpXMin, vpYMax); // Canto inferior esquerdo
    glEnd();
}

void desenhaTexto(const char* texto, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    while (*texto) {

        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *texto);
        texto++;
    }
}

void desenhaCaixasPainel() {
    glColor3f(0.5f, 0.5f, 0.5f);
    
    // Caixa 1: Seção "OBJETO ATIVO"
    glBegin(GL_LINE_LOOP);
        glVertex2f(605.0f, 55.0f);   // Canto superior esquerdo
        glVertex2f(790.0f, 55.0f);   // Canto superior direito
        glVertex2f(790.0f, 110.0f);  // Canto inferior direito
        glVertex2f(605.0f, 110.0f);  // Canto inferior esquerdo
    glEnd();

    // Caixa 2: Seção "COMANDOS"
    glBegin(GL_LINE_LOOP);
        glVertex2f(605.0f, 120.0f);
        glVertex2f(790.0f, 120.0f);
        glVertex2f(790.0f, 380.0f);
        glVertex2f(605.0f, 380.0f);
    glEnd();
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
    desenhaTexto("O / o  : Rotacionar", 610.0f, 220.0f);
    desenhaTexto("         na origem", 610.0f, 240.0f);
    desenhaTexto("+ / -  : Escala", 610.0f, 260.0f);
    desenhaTexto("X / Y  : Espelhar", 610.0f, 280.0f);
    desenhaTexto("H / h  : Cisalhar", 610.0f, 300.0f);
    desenhaTexto("0      : Reiniciar", 610.0f, 320.0f);
    desenhaTexto("D      : Mostrar Ordem", 610.0f, 340.0f);
    desenhaTexto("        (T*S vs S*T)", 610.0f, 360.0f);
}

void desenhaDemonstracao() {
    if (cena.empty()) return;
    ObjetoBase& obj = cena[objetoSelecionado]; 

    float meioX = vpXMin + (vpXMax - vpXMin) / 2.0f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(meioX, vpYMin);
        glVertex2f(meioX, vpYMax);
    glEnd();

    glm::mat3 S = Transformacoes::escala(1.1f, 0.5f);       
    glm::mat3 T = Transformacoes::translacao(70.0f, 10.0f);  

    glm::mat3 matrizTS = T * S; // Lado Esquerdo
    glm::mat3 matrizST = S * T; // Lado Direito

    auto desenharComViewport = [&](glm::mat3 matrizFinal, float vXMin, float vXMax) {
        
        glColor3f(0.3f, 0.3f, 0.3f); 
        glBegin(GL_LINES);
            glm::vec2 pX1 = Transformacoes::mundoParaViewport(glm::vec2(-100.0f, 0.0f), -100.0f, 100.0f, -100.0f, 100.0f, vXMin, vXMax, vpYMin, vpYMax);
            glm::vec2 pX2 = Transformacoes::mundoParaViewport(glm::vec2(100.0f, 0.0f), -100.0f, 100.0f, -100.0f, 100.0f, vXMin, vXMax, vpYMin, vpYMax);
            glVertex2f(pX1.x, pX1.y); glVertex2f(pX2.x, pX2.y);
            glm::vec2 pY1 = Transformacoes::mundoParaViewport(glm::vec2(0.0f, -100.0f), -100.0f, 100.0f, -100.0f, 100.0f, vXMin, vXMax, vpYMin, vpYMax);
            glm::vec2 pY2 = Transformacoes::mundoParaViewport(glm::vec2(0.0f, 100.0f), -100.0f, 100.0f, -100.0f, 100.0f, vXMin, vXMax, vpYMin, vpYMax);
            glVertex2f(pY1.x, pY1.y); glVertex2f(pY2.x, pY2.y);
        glEnd();

        for (const auto& poligono : obj.poligonos) {
            glColor3f(poligono.cor.r, poligono.cor.g, poligono.cor.b);
            glBegin(GL_POLYGON);
            for (const auto& vertice : poligono.vertices) {
                glm::vec3 p(vertice.x, vertice.y, 1.0f);
                glm::vec3 pt = matrizFinal * p; 
                
                glm::vec2 pv = Transformacoes::mundoParaViewport(
                    glm::vec2(pt.x, pt.y),
                    -100.0f, 100.0f, -100.0f, 100.0f,
                    vXMin, vXMax, vpYMin, vpYMax
                );
                glVertex2f(pv.x, pv.y);
            }
            glEnd();
        }
    };

    desenharComViewport(matrizTS, vpXMin, meioX);
    desenharComViewport(matrizST, meioX, vpXMax);
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);

    desenhaBordaViewport();

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
        glFlush();
    }

    desenhaCaixasPainel();
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
            glm::vec2 centroOriginal = obj.calcularCentro();
            glm::vec3 centroAtual = obj.matrizAcumulada * glm::vec3(centroOriginal.x, centroOriginal.y, 1.0f);
            obj.matrizAcumulada = Transformacoes::rotacaoNoCentro(centroAtual.x, centroAtual.y, 0.1f) * obj.matrizAcumulada;
            break;
        }
        case 'R': {
            glm::vec2 centroOriginal = obj.calcularCentro();
            glm::vec3 centroAtual = obj.matrizAcumulada * glm::vec3(centroOriginal.x, centroOriginal.y, 1.0f);
            obj.matrizAcumulada = Transformacoes::rotacaoNoCentro(centroAtual.x, centroAtual.y, -0.1f) * obj.matrizAcumulada;
            break;
        }
        case 'o': { 
            obj.matrizAcumulada = Transformacoes::rotacao(0.1f) * obj.matrizAcumulada;
            break;
        }
        case 'O': { 
            obj.matrizAcumulada = Transformacoes::rotacao(-0.1f) * obj.matrizAcumulada;
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
        case 'd':
        case 'D':
            modoDemonstracao = !modoDemonstracao; 
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