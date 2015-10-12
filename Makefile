VPATH = src
CPP = g++.exe
CFLAGS = -O2 -std=gnu++11 -fpermissive
OBJ = log.o main.o mem.o

tietokone.exe: $(OBJ)
	$(CPP) $(OBJ) -o "tietokone.exe"

all: tietokone.exe

clean:
	rm *.o
	rm *.exe

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<
