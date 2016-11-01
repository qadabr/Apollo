mkdir -p solution

cd solution
cmake -j9 -G "CodeBlocks - Unix Makefiles" ..
cmake --build .
