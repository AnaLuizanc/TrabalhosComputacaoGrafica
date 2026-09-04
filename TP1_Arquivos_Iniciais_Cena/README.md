# TP1 - Transformações Geométricas 2D e Viewport

Projeto desenvolvido para a disciplina de Computação Gráfica, com foco na implementação manual de transformações geométricas em 2D usando matrizes homogêneas e renderização com OpenGL.

## Integrantes

- Iago Rocha
- Ana Luiza Cordeiro

## Objetivo

O trabalho tem como objetivo demonstrar, de forma visual e interativa, o uso de transformações geométricas em objetos 2D, incluindo:

- translação;
- rotação;
- escala;
- reflexão;
- cisalhamento;
- conversão entre coordenadas do mundo e viewport;
- aplicação da matriz acumulada em objetos da cena.

Além disso, o projeto mostra visualmente que a ordem de multiplicação de matrizes influencia o resultado final, especialmente no caso de escala e translação.

## Ambiente de desenvolvimento

- Sistema operacional utilizado: Linux / Windows (MSYS2/MinGW)
- Linguagem: C++17
- Biblioteca gráfica: OpenGL legacy
- Interface de janelas: GLUT / freeglut
- Biblioteca matemática: GLM

## Dependências

Antes de compilar, certifique-se de que os pacotes abaixo estejam instalados:

- GCC/G++ com suporte a C++17
- OpenGL
- GLUT ou freeglut
- GLM

No Linux, isso costuma ser instalado via gerenciador de pacotes, por exemplo:

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglm-dev libgl1-mesa-dev
```

## Estrutura do projeto

A aplicação foi organizada em módulos para separar a lógica matemática da renderização:

- `main.cpp`: controla a janela, eventos do GLUT, desenho da cena e interação com teclado/mouse.
- `include/scene_types.hpp`: define as estruturas básicas de objetos, polígonos e cores.
- `include/casa.hpp`, `include/barco.hpp`, `include/moinho.hpp`: representam os objetos da cena.
- `include/transformacoes.hpp`: implementa as matrizes homogêneas e a conversão mundo ↔ viewport.
- `cena_inicial.hpp`: monta a cena inicial com os objetos presentes na aplicação.
- `Makefile`: automatiza compilação e execução.

## Como executar

Na raiz do projeto, execute:

```bash
make
```

Para rodar a aplicação:

```bash
make run
```

Para limpar os arquivos gerados:

```bash
make clean
```

## Controles

### Teclado

- `1`, `2`, `3`: seleciona o objeto ativo na cena
- Setas direcionais: translada o objeto selecionado
- `R` / `r`: rotaciona o objeto em torno do seu centro geométrico
- `+` / `-`: aumenta ou diminui a escala uniforme
- `X` / `Y`: aplica reflexão nos eixos X e Y
- `H` / `h`: aplica cisalhamento
- `0`: restaura a matriz do objeto para identidade
- `D`: alterna para a demonstração visual da ordem de transformação

### Mouse

- Clique esquerdo: seleciona automaticamente o objeto sob o ponteiro.
- Arraste com o botão esquerdo pressionado: move o objeto pela cena.

## Demonstração da ordem das transformações

A aplicação inclui uma janela de demonstração em que são apresentados dois casos comparativos:

- escala seguida de translação;
- translação seguida de escala.

A visualização mostra que a ordem de aplicação das matrizes não é comutativa, e o resultado final pode mudar dependendo da sequência escolhida.

## Observações importantes

- A implementação usa matrizes homogêneas 3x3 para as transformações.
- A conversão para viewport inverte o eixo Y para que o sistema gráfico corresponda corretamente à representação na tela.
- Nenhuma função pronta de transformação do OpenGL foi usada no fluxo principal da aplicação; as matrizes foram desenvolvidas manualmente.

## Referências

- HEARN, Donald; BAKER, M. Pauline; CARITHERS, Warren. Computer Graphics with OpenGL. 4. ed. Pearson, 2014.
- Uso de ferramentas de inteligência artificial para suporte estrutural e discussão matemática, conforme as diretrizes do trabalho e autoria do estudante.

## Resultado esperado

Ao executar o programa, o usuário visualiza uma cena com objetos geométricos 2D interativos, podendo aplicar transformações manualmente e verificar, de forma prática, os conceitos de composição de matrizes e viewport na computação gráfica.