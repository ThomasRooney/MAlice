
LLVM_VERSION=3.1

if [ ! -d dependencies ] ; then
  mkdir dependencies
fi

cd dependencies


if [ ! -f llvm-$LLVM_VERSION.tar.gz ] ; then
  if [ -n "$LLVM_VERSION" -a -n "3.0" ];then
    result=$(awk -vn1="$LLVM_VERSION" -vn2="3.0" 'BEGIN{print (n1>n2)?1:0 }')
    echo $result
    if [ "$result" -eq 1 ] ; then
      wget http://llvm.org/releases/$LLVM_VERSION/llvm-$LLVM_VERSION.src.tar.gz
      mv llvm-$LLVM_VERSION.src.tar.gz llvm-$LLVM_VERSION.tar.gz
    else
      wget http://llvm.org/releases/$LLVM_VERSION/llvm-$LLVM_VERSION.tar.gz
    fi
  fi
fi


if [ ! -d llvm ] ; then
  tar -xzvf llvm-$LLVM_VERSION.tar.gz
  mkdir build

  cd build
  ../llvm-$LLVM_VERSION.src/configure --enable-optimized --disable-assertions
  make
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
