cd dependencies
tar -xzvf llvm-3.0.tar.gz
mkdir build

cd build
../llvm-3.0.src/configure --enable-optimized --disable-assertions
make
cd ..
mkdir llvm
mv build/Release/* llvm
mkdir llvm/include
mkdir llvm/include/llvm
mv llvm-3.0.src/include/llvm/* llvm/include/llvm/
mv build/include/* llvm/include/llvm/
rm -rf build
rm -rf llvm-3.0.src
