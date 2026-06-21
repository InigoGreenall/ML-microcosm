rm -rf ./build
mkdir ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B ./build