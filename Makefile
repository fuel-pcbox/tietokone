VPATH = src
CPP = g++.exe
CFLAGS = -O0 -std=gnu++11 -fpermissive -g
OBJ = log.o main.o mem.o cpu.o keyboard.o

tietokone.exe: $(OBJ)
	$(CPP) $(OBJ) -o "tietokone.exe"

all: tietokone.exe

clean:
	rm *.o
	rm *.exe

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<
