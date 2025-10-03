# T1_INF05010
### Trabalho final de Otimização Combinatória
Este projeto implementa uma meta-heuristica de Busca Local Iterada (ILS) para resolver o problema de organização de times.
> **Problema de Organização de Times: dado um budget B global para cada time, uma lista de jogadores e seus respectivos salários J, e uma lista de conflitos entre duplas de jogadores I, busca-se achar o menor número de times para comportar jogadores. Cada time deve ter orçamento suficiente para pagar o salário a todos os jogadores e não pode possuir jogadores com conflitos.**

### Integrantes:
- João Vitor de Souza
- Isadora Brigo Vidor
- Aline Fraga da Silva

## Como rodar esse repositorio:
- Instale um compilador C++:
```sh
sudo apt update && sudo apt install g++
```
- Certifique-se de que o g++ está disponível no seu PATH:
```sh
g++ --version
```
- Baixe/clone o repositório para sua máquina:
```sh
git clone https://github.com/joaoo-vitor/T1_INF05010
cd T1_INF05010
```
- Compile o código executando o comando abaixo:
```sh
g++ -O2 -std=c++17 -Wall -Wextra -o ils ils.cpp
```
- Execute o código compilado chamando o arquivo compilado com argumentos:
```sh
./ils <caminho_da_instancia> <numero_maximo_de_iteracoes> <seed_de_aleatoriedade> [--perturbation_ratio <perturbation_ratio>]
```
Onde:
- `<caminho_da_instancia>` -> Caminho para arquivo de texto contendo a instância no formato do enunciado do trabalho;
- `<numero_maximo_de_iterações>` -> Número máximo de iterações de **Busca Local + Pertubação**;
- `<seed_de_aleatoriedade>` -> Semente de aleatoriedade usada para gerar a solução inicial e para escolher os jogadores para cada perturbação.
- `<pertubation_ratio>` -> Porcentagem de jogadores que sofrerão pertubação (troca de times) após cada iteração do __Local Search__, um valor de 0 a 1. (default é 0.15, ou seja, 15%).

Exemplo:
```sh
./ils instances/01.txt 100 1 --perturbation_ratio 0.15
```

## Link para relatório preliminar:
<botar_aqui_aline>
