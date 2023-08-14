# Set env.
AFL_USE_ASAN=0

## Set gcov
sudo apt-get install gcc-11
sudo rm /usr/bin/cpp /usr/bin/gcc /usr/bin/g++  /usr/bin/gcov  /usr/bin/c++
sudo rm /usr/local/bin/cpp /usr/local/bin/gcc /usr/local/bin/g++ /usr/local/bin/gcov  /usr/local/bin/c++
sudo ln -s /usr/bin/cpp-11 /usr/bin/cpp
sudo ln -s /usr/bin/gcc-11 /usr/bin/gcc
sudo ln -s /usr/bin/g++-11 /usr/bin/g++
sudo ln -s /usr/bin/g++-11 /usr/bin/c++
sudo ln -s /usr/bin/gcov-11 /usr/bin/gcov

# For Cmake
sudo apt-get install libssl-dev

# Install pre-requirements
sudo apt-get install automake
sudo apt-get install python3-dev
python3 -m venv env/python
source env/python/bin/activate
pip3 install --upgrade pip
pip3 install wheel

sudo apt remove clang-10 clang-11 clang-9 clang-12
sudo rm -rf  /usr/bin/llvm-config
sudo apt autoremove

sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
sudo chmod 777 llvm.sh
sudo ./llvm.sh 13
sudo apt-get install -y lld-13 llvm-13-dev clang-13
sudo apt-get install -y lld llvm llvm-dev clang
sudo apt-get install -y build-essential python3-dev automake cmake git flex bison libglib2.0-dev libpixman-1-dev python3-setuptools cargo libgtk-3-dev
sudo apt-get install -y ninja-build cmake
sudo apt-get install -y wget git make cmake llvm gdb coreutils
sudo apt-get install gcc-11-plugin-dev
sudo apt autoremove

# Update default clang to 13
sudo update-alternatives --remove-all clang 
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-13 1300 --slave /usr/bin/clang++ clag++ /usr/bin/clang++-13
sudo update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-13 1300

# Remove olf AFL
rm -rf AFLplusplus

# Install AFL
LLVM_CONFIG=/usr/bin/llvm-config-13
#git clone https://github.com/google/AFL.git
#cd AFL
git clone https://github.com/AFLplusplus/AFLplusplus.git
cd AFLplusplus
AFL_USE_ASAN=0 make
sudo make install
