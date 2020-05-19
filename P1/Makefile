CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -pedantic
CPPFLAGS = -DP1 -I../Tests-auto -I.
VPATH = ../Tests-auto:.

EXES = test-P1-auto test-P1-consola fecha_check cadena_check
CLASES = cadena.o fecha.o

.PHONY: all clean

all: $(EXES)

test-P1-auto: test-caso0-fecha-auto.o test-caso0-cadena-auto.o test-auto.o $(CLASES)
	$(CXX) $(CXXFLAGS) $^ -o $@

test-P1-consola: test-P1-consola.o $(CLASES)
		$(CXX) $(CXXFLAGS) $^ -o $@

test-caso0-fecha-auto.o test-caso0-cadena-auto.o test-auto.o: test-caso0-fecha-auto.cpp test-caso0-cadena-auto.cpp test-auto.cpp test-auto.hpp fecha.hpp cadena.hpp

fecha_check:
	${MAKE} -f Make_check.mk $@

cadena_check:
	${MAKE} -f Make_check.mk $@

check_Fecha:
	${MAKE} -f Make_check.mk $@

check_Cadena:
	${MAKE} -f Make_check.mk $@	

prueba: prueba.o $(CLASES)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean: 
	@$(RM) $(EXES) *.o