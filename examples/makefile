shapes: shapes.o
	gcc shapes.o -o shapes -lstdc++ -lroughdraft -lz -lpng -lcairo

shapes.o: shapes.c
	gcc -c shapes.c 

clean:
	rm *.o *.bmp *.png shapes
