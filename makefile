CFLAGS = -std=c++11 -Wall

HuffTree: main.o HuffTree.o KeyTable.o EncReader.o EncWriter.o
	g++ main.o HuffTree.o KeyTable.o EncReader.o EncWriter.o -o HuffTree $(CFLAGS)

main.o: HuffTree.h KeyTable.h EncReader.h EncWriter.h main.cpp
	g++ -c main.cpp $(CFLAGS)

HuffTree.o: HuffTree.h HuffTree.cpp
	g++ -c HuffTree.cpp $(CFLAGS)

KeyTable.o: KeyTable.h KeyTable.cpp
	g++ -c KeyTable.cpp $(CFLAGS)

EncReader.o: EncReader.h EncReader.cpp
	g++ -c EncReader.cpp $(CFLAGS)

EncWriter.o: EncWriter.h EncWriter.cpp
	g++ -c EncWriter.cpp $(CFLAGS)

clean:
	rm *.o HuffTree