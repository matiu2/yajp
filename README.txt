Yet another JSON Parser.

Powered by 'ragel' and state machines.

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

** Summary **

There are two levels of useable code in the library.

The 'parser' directory is the lowest level and is fast and dangerous and has a functionalish usage style. It uses pointers, so the json string has to live longer than the yajp::JSONParser, and the Parser has to live longer than any exceptions it throws.

Look in 'tests/parser/test_person_example' for an example on how to use the parser level to turn json text into useable c++ objects quickly;

The 'json_types' directory contains higher level classes, and using the 'mapper' directory code, you can get a hierachy of JSON classes out of JSON string. These use smart pointers and virtual methods, and once you've got them, they're much easier and safer to use. However there will be a negligble performance hit, that shouldn't bother really any one.

You can also use 'mappers/AnyMapper' to convert a JSONString to a hierachy of Boost::Any variant like objects.

------------
Status
------------

 * The 'parser' code works and is well tested
 * The 'json_types' and 'mapper' code is still under development
 * The 'mappers/AnyMapper' code is still under development
