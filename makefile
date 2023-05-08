all: 
	rm  -f output.csv
	g++ -std=c++17  main.c -o PositionCounter.out
	g++ -std=c++17 map.cpp -o map.out
	g++ -std=c++17 reduce.cpp -o reduce.out

cl:
	rm -f line_* main *.out PositionCounter.out