all: run

# BIN1ário
BIN1 = ils_5s
BIN2 = ils_300s

# Compilação
$(BIN1): ils5s_time.cpp
	g++ -o $(BIN1) ils5s_time.cpp -std=c++17 -O3

$(BIN1): ils300s_time.cpp
	g++ -o $(BIN1) ils300s_time.cpp -std=c++17 -O3


run: $(BIN1) $(BIN2)
	./$(BIN1) instances/01.txt 1000 1 
	./$(BIN1) instances/02.txt 1000 1 
	./$(BIN1) instances/03.txt 1000 1 
	./$(BIN1) instances/04.txt 1000 1
	./$(BIN1) instances/05.txt 1000 1 
	./$(BIN1) instances/06.txt 1000 1 
	./$(BIN1) instances/07.txt 1000 1 
	./$(BIN1) instances/08.txt 1000 1 
	./$(BIN1) instances/09.txt 1000 1 
	./$(BIN1) instances/10.txt 1000 1 

clean:
	rm -f $(BIN1) $(BIN2) result_*.txt