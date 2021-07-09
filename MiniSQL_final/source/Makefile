MiniSQL: test.o Interpreter.o API.o IndexManager.o RecordManager.o BufferManager.o CatalogManager.o
	g++ test.o Interpreter.o API.o IndexManager.o RecordManager.o BufferManager.o CatalogManager.o -o MiniSQL.exe
	del *.o

BufferManager.o: BufferManager.cpp
	g++ -c BufferManager.cpp

RecordManager.o: Recordmanager.cpp CatalogManager.h BufferManager.h
	g++ -c RecordManager.cpp

test.o: test.cpp Interpreter.h
	g++ -c test.cpp

CatalogManager.o: CatalogManager.cpp
	g++ -c CatalogManager.cpp

Interpreter.o: Interpreter.cpp API.h CatalogManager.h
	g++ -c Interpreter.cpp

IndexManager.o: IndexManager.cpp RecordManager.h CatalogManager.h
	g++ -c IndexManager.cpp

API.o: API.cpp RecordManager.h CatalogManager.h IndexManager.h BufferManager.h
	g++ -c API.cpp
