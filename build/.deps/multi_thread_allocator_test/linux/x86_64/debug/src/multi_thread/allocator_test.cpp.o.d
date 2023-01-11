{
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-g",
            "-O0",
            "-std=c++14"
        }
    },
    files = {
        "src/multi_thread/allocator_test.cpp"
    },
    depfiles_gcc = "allocator_test.o: src/multi_thread/allocator_test.cpp  src/multi_thread/allocator.h\
"
}