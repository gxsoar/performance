{
    files = {
        "src/single_thread/tcmalloc.cpp"
    },
    depfiles_gcc = "tcmalloc.o: src/single_thread/tcmalloc.cpp\
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