all:
	g++ -c src/*.cpp -std=c++11 -I /usr/include/SDL2 -I /usr/include/GL
	g++ *.o -o Main -l SDL2 -l SDL2_image -l GL -l GLU -l GLEW
	rm *.o
