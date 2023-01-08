{
    files = {
        "src/multi_thread/allocator_test.cpp"
    },
    depfiles_gcc = "allocator_test.o: src/multi_thread/allocator_test.cpp  src/multi_thread/allocator.h\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-std=c++14",
            "-DNDEBUG"
        }
    }
}