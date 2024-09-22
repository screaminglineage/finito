.PHONY = run
run: finito
	./finito test.am

finito: main.cpp parser.cpp execute.cpp
	g++ -std=c++20 $^ -ggdb -o finito
    


