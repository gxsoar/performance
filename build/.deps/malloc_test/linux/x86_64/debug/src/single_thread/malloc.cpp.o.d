{
    files = {
        "src/single_thread/malloc.cpp"
    },
    depfiles_gcc = "malloc.o: src/single_thread/malloc.cpp\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-g",
            "-O0",
            "-std=c++14"
        }
    }
}