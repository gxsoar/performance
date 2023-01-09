{
    depfiles_gcc = "tcmalloc.o: src/single_thread/tcmalloc.cpp\
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
    },
    files = {
        "src/single_thread/tcmalloc.cpp"
    }
}