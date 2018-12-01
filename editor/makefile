editor: shapes.o parser.o
	g++ *.o -o editor -lroughdraft -lz -lpng -lcairo -L../src

shapes.o: shapes.cpp
	g++ -gdwarf-4 -std=c++11 -c shapes.cpp -I../src

parser.o: parser.cpp
	g++ -gdwarf-4 -std=c++11 -c parser.cpp -I../src

clean:
	rm *.o *.bmp *.png editor
