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

------------
Architecture
------------

** History **

Originally I tried to do it all in one go. However, that's not possible as JSON is recursive.

I then tried to do it as 'in one go' as possible, thinking to make it the fastest json parser available, but that's not very flexible to program on top of.

** Summary **

There are two levels, the *parser level* 'JSONParser' is the lowest level and won't be needed by most uses of the library.

The next level I call the *Generalized Layer*; this level provides generic methods and types that can be used to map
JSON types to your real C++ classes.

