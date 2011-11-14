Yet another JSON Parser.

Based on 'ragel' and state machines.

To build on ubuntu:

sudo apt-get install ragel build-essential libboost-test-dev g++ cmake git dot eog
git clone https://github.com/matiu2/yajp
cd yajp
mkdir build
cd build
cmake ..
make
eog number.png # Check out the state diagram
./tests/test_runner  # Make sure the tests run
./demo_number 1.2
./demo_number 1000
./demo_number -1000.54e-3
