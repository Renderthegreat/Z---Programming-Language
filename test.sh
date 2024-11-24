./src/generate_header.sh src/gen/files.h src/gen/header.h
make 
output/main -i tests/ -o tests/output/
gcc -c tests/output/main.S -o tests/output/bin/main.f