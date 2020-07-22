@echo off

cd third_party/extra-cmake-modules
mkdir build
cd build
cmake .. -DBUILD_HTML_DOCS=OFF -DBUILD_MAN_DOCS=OFF -DBUILD_QTHELP_DOCS=OFF -DBUILD_TESTING=OFF 

cmake --build . --config Release -j $(nproc)
cmake --build . --target install