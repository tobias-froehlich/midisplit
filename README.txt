
Installing GTest:

sudo aptitude install libgtest-dev
cd /usr/src/googletest/
sudo cmake .
sudo cmake --build . --target install

Building midisplit:

in midisplit/build/
rm -r *
cmake ..
make

Testing:

in midisplit/build/test
./Test


