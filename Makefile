all: run

# BINário
BIN = ils_time

# Compilação do arquivo
$(BIN): ils_time.cpp
	g++ -o $(BIN) ils_time.cpp -std=c++17 -O3

# Execução dos testes
run: $(BIN) 

# Testes com 5 segundos de execução
# Seed 1
	./$(BIN) instances/01.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 5 1 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 5 1 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 5 1 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 5 1 --perturbation_ratio 0.30

# Seed 2
	./$(BIN) instances/01.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 5 2 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 5 2 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 5 2 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 5 2 --perturbation_ratio 0.30

# Seed 3
	./$(BIN) instances/01.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 5 3 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 5 3 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 5 3 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 5 3 --perturbation_ratio 0.30

# Seed 4
	./$(BIN) instances/01.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 5 4 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 5 4 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 5 4 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 5 4 --perturbation_ratio 0.30

# Seed 5
	./$(BIN) instances/01.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 5 5 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 5 5 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 5 5 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 5 5 --perturbation_ratio 0.30

# Testes com 300 segundos de execução
# Seed 1
	./$(BIN) instances/01.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 300 1 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 300 1 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 300 1 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 300 1 --perturbation_ratio 0.30

# Seed 2
	./$(BIN) instances/01.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 300 2 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 300 2 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 300 2 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 300 2 --perturbation_ratio 0.30

# Seed 3
	./$(BIN) instances/01.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 300 3 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 300 3 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 300 3 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 300 3 --perturbation_ratio 0.30

# Seed 4
	./$(BIN) instances/01.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 300 4 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 300 4 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 300 4 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 300 4 --perturbation_ratio 0.30

# Seed 5
	./$(BIN) instances/01.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/02.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/03.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/04.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/05.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/06.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/07.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/08.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/09.txt 300 5 --perturbation_ratio 0.15
	./$(BIN) instances/10.txt 300 5 --perturbation_ratio 0.15

	./$(BIN) instances/01.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/02.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/03.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/04.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/05.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/06.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/07.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/08.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/09.txt 300 5 --perturbation_ratio 0.30
	./$(BIN) instances/10.txt 300 5 --perturbation_ratio 0.30

clean:
	rm -f $(BIN) 