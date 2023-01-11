{
    depfiles_gcc = "tcmalloc.o: src/multi_thread/tcmalloc.cpp\
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
        "src/multi_thread/tcmalloc.cpp"
    }
}