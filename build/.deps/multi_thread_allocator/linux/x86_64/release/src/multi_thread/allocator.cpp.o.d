{
    depfiles_gcc = "allocator.o: src/multi_thread/allocator.cpp src/multi_thread/allocator.h\
",
    files = {
        "src/multi_thread/allocator.cpp"
    },
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