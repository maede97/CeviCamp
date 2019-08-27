set -e;
mkdir -p build;
cd build;
cmake .. -DCMAKE_BUILD_TYPE=DEBUG;
make;
cd ..;
./build/main
