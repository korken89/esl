cd /usr/src/gtest
cmake . -DBUILD_SHARED_LIBS=ON
make
mv libgtest* /usr/lib/
