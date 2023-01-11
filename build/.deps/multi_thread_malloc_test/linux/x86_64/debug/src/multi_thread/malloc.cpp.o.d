{
    depfiles_gcc = "malloc.o: src/multi_thread/malloc.cpp\
",
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
        "src/multi_thread/malloc.cpp"
    }
}