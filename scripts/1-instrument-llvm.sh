# Make sure the swaps are active
sudo swapon /home/user123/swapfile
sudo swapon /home/user123/swapfile-BIG

# Get LLVM source
git clone https://github.com/llvm/llvm-project.git
cd llvm-project

# Get a specific LLVM (e.g. LLVM-13)
git checkout release/13.x

# configure with AFL
cd ..
mkdir build
cd build
AFL_USE_ASAN=0 cmake -G Ninja -Wall ../llvm-project/llvm/ -DLLVM_ENABLE_PROJECTS='clang' -DCMAKE_BUILD_TYPE='Release' -DCMAKE_C_COMPILERC=afl-cc -DCMAKE_CXX_COMPILER=afl-c++ -DBUILD_SHARED_LIBS=OFF -DLLVM_TARGETS_TO_BUILD=X86 -DCMAKE_C_FLAGS="-pthread" -DCMAKE_CXX_FLAGS="-pthread" -DBUILD_SHARED_LIBS="OFF" -DLLVM_BUILD_DOCS="OFF" -DLLVM_BUILD_EXAMPLES="OFF"

# build
AFL_USE_ASAN=0 ninja
