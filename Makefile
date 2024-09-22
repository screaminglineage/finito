.PHONY = run
run: finito
	./finito test.am

finito: src/main.cpp src/parser.cpp src/execute.cpp
	g++ -std=c++20 $^ -ggdb -o finito
    


