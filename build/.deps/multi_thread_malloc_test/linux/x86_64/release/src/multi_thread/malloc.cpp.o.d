{
    files = {
        "src/multi_thread/malloc.cpp"
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
    },
    depfiles_gcc = "malloc.o: src/multi_thread/malloc.cpp\
"
}