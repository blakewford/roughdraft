shapes: shapes.o
	g++ shapes.o -o shapes -lroughdraft -lz -lpng -lcairo -L../src

shapes.o: shapes.c
	gcc -c shapes.c -I../src

clean:
	rm *.o *.bmp *.png shapes
