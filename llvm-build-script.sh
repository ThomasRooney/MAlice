LLVM_VERSION=3.0

cd dependencies

if [ ! -f llvm-$LLVM_VERSION.src.tar.gz ] ; then
    wget http://llvm.org/releases/$LLVM_VERSION/llvm-$LLVM_VERSION.src.tar.gz
fi

if [ ! -d llvm ] ; then
    tar -xzvf llvm-$LLVM_VERSION.src.tar.gz
    mkdir build

    cd build
    ../llvm-$LLVM_VERSION.src/configure --enable-optimized --disable-assertions
    make -j
    cd ..
    mkdir llvm
    mv build/Release/* llvm
    mkdir llvm/include
    mkdir llvm/include/llvm

    mv llvm-$LLVM_VERSION.src/include/llvm/* llvm/include/llvm/

    cp -rfl build/include/llvm/ llvm/include/

    rm -rf build
    rm -rf llvm-$LLVM_VERSION.src
else
    echo "LLVM already configured"

fi