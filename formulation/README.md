# Como executar a formulação inteira
- Certifique-se que você tem julia instalado.
Após instalação é posssível rodar o terminal Julia:
```
julia
```
- Instale HiGHS e JUMP rodando o comando abaixo no terminal Julia
```
import Pkg; Pkg.add("JuMP"); Pkg.add("HiGHS");
```
- Compile o código executando o comando abaixo (fora do terminal julia)
```sh
julia fli.jl <arquivo.txt> <tempo_max_s> <seed>
```
Exemplo:
(rodar a instância 01, com tempo de 10s e seed 0)
```sh
julia fli.jl instances/01.txt 10 0
```


# Como executar a heuristica
- Instale um compilador C++:
```sh
sudo apt update && sudo apt install g++
```
- Certifique-se de que o g++ está disponível no seu PATH:
```sh
g++ --version
```
- Compile o código executando o comando abaixo:
```sh
g++ -O2 -std=c++17 -Wall -Wextra -o ils ils.cpp
```

Ou, para fazer debug:
```sh
g++ -O2 -std=c++17 -Wall -Wextra -g -o ils ils.cpp
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