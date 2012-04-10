FLAGS = -s -O3 -Os -fexpensive-optimizations
LIB = -lconfig++

all : seap expandparam

expandparam :
	@mkdir -p bin
	@echo 'Compilando expandparam...'
	@g++ src/aux/expandparam.cpp -o bin/expandparam ${FLAGS}
	@echo 'Listo'

seap :
	@mkdir -p bin
	@echo 'Compilando seap...'
	@g++ src/utils/*.cpp src/juez*.cpp src/main.cpp -o bin/seap ${LIB} ${FLAGS}
	@echo 'Listo'
