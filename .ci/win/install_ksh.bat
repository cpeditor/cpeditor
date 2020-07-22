@echo off

REM Call from Root of project

cd third_party/syntax-highlighting
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_HTML_DOCS=OFF -DBUILD_MAN_DOCS=OFF -DBUILD_QTHELP_DOCS=OFF -DBUILD_TESTING=OFF 

cmake --build . --config Release -j $(nproc)
cmake --build . --target install