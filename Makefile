output:  Lexxer.o Parser.o  main.o emulator.o
	g++ Lexxer.o Parser.o main.o emulator.o -o output
Lexxer.o: src/Frontend/Lexxer.cpp  src/Frontend/Lexxer.h
	g++ -c -g src/Frontend/Lexxer.cpp
Parser.o: src/Frontend/Parser.cpp src/Frontend/Lexxer.h src/Frontend/Parser.h
	g++ -c -g src/Frontend/Parser.cpp
emulator.o: src/emulator/emulator.cpp src/Frontend/Lexxer.h src/Frontend/Parser.h src/emulator/emulator.h
	g++ -c -g src/emulator/emulator.cpp
main.o:  main.cpp src/Frontend/Lexxer.h src/Frontend/Parser.h src/emulator/emulator.h
	g++ -c -g main.cpp
clean:
	rm *.o output
	clear