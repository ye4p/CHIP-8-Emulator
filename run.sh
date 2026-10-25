cmake -S . -B build && 
cmake --build build &&

cd build &&
chmod a+wrx app &&

./app
