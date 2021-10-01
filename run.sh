mkdir -p build
cd build
cmake ..
make
cd ..
valgrind ./build/main Arquitetura5x5.txt test.dot