test: main.o
	gcc main.o -o test -lstdc++ -lroughdraft -lz -lpng -lcairo

main.o: main.c
	gcc -c main.c 

clean:
	rm *.o *.bmp *.png test
