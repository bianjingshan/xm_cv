mkdir -p build
cd build
cmake ..
make

cd ../bin
rm -f MSYH.TTF
ln -s ../3rdparty/font/MSYH.TTF MSYH.TTF
