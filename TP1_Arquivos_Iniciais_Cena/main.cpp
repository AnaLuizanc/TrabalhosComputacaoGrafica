#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>

#include "transformacoes.hpp"

#include "scene_types.hpp"
#include "cena_inicial.hpp"

using namespace std;

vector<ObjetoBase> cena;

float mundoXMin = -100.0f, mundoXMax = 100.0f;
float mundoYMin = -100.0f, mundoYMax = 100.0f;

float vpXMin = 0.0f, vpXMax = 800.0f;
float vpYMin = 0.0f, vpYMax = 600.0f;

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& objeto : cena) {
        for (const auto& poligono : objeto.poligonos) {
            
            glColor3f(poligono.cor.r, poligono.cor.g, poligono.cor.b);
            
            glBegin(GL_POLYGON);
            for (const auto& vertice : poligono.vertices) {
                
                glm::vec3 pontoHomogeneo(vertice.x, vertice.y, 1.0f);
                
                glm::vec3 pontoTransformado = objeto.matrizAcumulada * pontoHomogeneo;
                
                glm::vec2 pontoViewport = Transformacoes::mundoParaViewport(
                    glm::vec2(pontoTransformado.x, pontoTransformado.y),
                    -100.0f, 100.0f, -100.0f, 100.0f, 
                    0.0f, 800.0f, 0.0f, 600.0f 
                );
                
                glVertex2f(pontoViewport.x, pontoViewport.y);
            }
            glEnd();
        }
    }

    glFlush();
}

int objetoSelecionado = 0; 

float passoTranslacao = 5.0f;
float passoEscala = 1.1f;
float passoRotacao = 0.1f; 

void teclado(unsigned char key, int x, int y) {
    if (cena.empty()) return;
    
    ObjetoBase& obj = cena[objetoSelecionado];

    switch (key) {
        case '1': objetoSelecionado = 0; break; 
        case '2': objetoSelecionado = 1; break; 
        case '3': objetoSelecionado = 2; break;
        case 'r': {
            glm::vec2 centro = obj.calcularCentro();
            obj.matrizAcumulada = Transformacoes::rotacaoNoCentro(centro.x, centro.y, 0.1f) * obj.matrizAcumulada;
            break;
        }
        case 'R': {
            glm::vec2 centro = obj.calcularCentro();
            obj.matrizAcumulada = Transformacoes::rotacaoNoCentro(centro.x, centro.y, -0.1f) * obj.matrizAcumulada;
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
    
    inicializa();
    glutDisplayFunc(desenha);
    
    glutMainLoop();
    
    return 0;
}