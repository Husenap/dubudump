cmake . -B.build -Ax64
cmake --build .build\ --config Release
cmake --install .build\ --config Release