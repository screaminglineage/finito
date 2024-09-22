.PHONY = run
run: automagic
	./automagic test.am

automagic: main.cpp parser.cpp execute.cpp
	g++ -std=c++20 $^ -ggdb -o automagic
    


