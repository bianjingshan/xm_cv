./configure
make
mkdir -p lib
cp objs/.libs/libfreetype.a ./lib
cp objs/.libs/libfreetype.so ./lib

