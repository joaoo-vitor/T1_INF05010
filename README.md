# T1_INF05010
### Trabalho final de Otimização Combinatória
Este projeto implementa uma meta-heuristica de Busca Local Iterada (ILS) para resolver o problema de organização de times.
> **Problema de Organização de Times: dado um budget B global para cada time, uma lista de jogadores e seus respectivos salários J, e uma lista de conflitos entre duplas de jogadores I, busca-se achar o menor número de times para comportar jogadores. Cada time deve ter orçamento suficiente para pagar o salário a todos os jogadores e não pode possuir jogadores com conflitos.**

### Integrantes:
- João Vitor de Souza
- Isadora Brigo Vidor
- Aline Fraga da Silva

## Como rodar esse repositorio:
- Instale um compilador C++ (ex: MinGW);
- Tenha o `g++` disponível no PATH do sistema;
- Baixe/clone o repositório para sua máquina;
- Compile o código executando o comando abaixo:
```sh
g++ -O2 -std=c++17 -Wall -Wextra -o main_explode main_explode.cpp
```
- Execute o código compilado chamando o arquivo compilado com argumentos:
```sh
main_explode.exe <caminho_da_instancia> <numero_maximo_de_iterações> <seed_de_aleatoriedade> [--perturbation_ratio <pertubation_ratio>]
```
Onde:
- `<caminho_da_instancia>` -> Caminho para arquivo de texto contendo a instância no formato do enunciado do trabalho;
- `<numero_maximo_de_iterações>` -> Número máximo de iterações de **Busca Local + Pertubação**;
- `<seed_de_aleatoriedade>` -> Semente de aleatoriedade usada para gerar a solução inicial e para escolher os jogadores para cada perturbação.
- `<pertubation_ratio>` -> Porcentagem de jogadores que sofrerão pertubação (troca de times) após cada iteração do __Local Search__, um valor de 0 a 1. (default é 0.15, ou seja, 15%).

Exemplo:
```sh
.\main_explode.exe "instances/01.txt" 100 0 --perturbation_ratio 0.2
```

## Link para relatório preliminar:
<botar_aqui_aline>
