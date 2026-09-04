# Arquivos iniciais - TP1

Esta pasta contem a descricao da cena inicial do TP1 em coordenadas do mundo.

- `scene_types.hpp`: estruturas `PoligonoBase` e `ObjetoBase`;
- `casa.hpp`, `barco.hpp` e `moinho.hpp`: figuras coloridas;
- `cena_inicial.hpp`: funcao que reune os tres objetos.

As estruturas usam `glm::vec2` para vertices e `glm::vec3` para cores RGB. Os vertices devem permanecer no sistema do mundo; a conversao para a viewport e responsabilidade da implementacao dos estudantes.

Exemplo de uso:

```cpp
#include "cena_inicial.hpp"

std::vector<ObjetoBase> cena = criarCenaInicial();
```

O professor pode modificar, ampliar ou substituir a cena para criar variacoes de avaliacao.
