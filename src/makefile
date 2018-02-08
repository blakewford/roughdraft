roughdraft: roughdraft.o draft.o draftutils.o imgmanip.o imgutils.o bmpmanip.o bmpmanip24.o bmpmanip256.o pngmanip.o rdecompress.o
	make roughdraft.a
roughdraft.o: roughdraft.cpp
	gcc -fPIC -g -c -Wall roughdraft.cpp
	
draft.o: draft.cpp
	gcc -fPIC -g -c -Wall draft.cpp

draftutils.o: draftutils.cpp
	gcc -fPIC -g -c -Wall draftutils.cpp

imgmanip.o: imgmanip.cpp
	gcc -fPIC -g -c -Wall imgmanip.cpp

imgutils.o: imgutils.cpp
	gcc -fPIC -g -c -Wall imgutils.cpp

bmpmanip.o: bmpmanip.cpp
	gcc -fPIC -g -c -Wall bmpmanip.cpp

bmpmanip24.o: bmpmanip24.cpp
	gcc -fPIC -g -c -Wall bmpmanip24.cpp

bmpmanip256.o: bmpmanip256.cpp
	gcc -fPIC -g -c -Wall bmpmanip256.cpp

pngmanip.o: pngmanip.cpp
	gcc -fPIC -g -c -Wall pngmanip.cpp

rdecompress.o: rdecompress.cpp
	gcc -fPIC -g -c -Wall rdecompress.cpp

roughdraft.a: 
	ar rcs libroughdraft.a *.o 

install: roughdraft.a
	sudo cp ./libroughdraft.a /usr/local/lib
	sudo cp ./roughdraft.h /usr/local/include
	sudo cp ./roughdraftdefines.h /usr/local/include

clean: 
	rm *.o libroughdraft.a
