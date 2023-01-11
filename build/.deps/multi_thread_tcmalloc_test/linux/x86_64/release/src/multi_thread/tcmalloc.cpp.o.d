{
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
    depfiles_gcc = "tcmalloc.o: src/multi_thread/tcmalloc.cpp\
",
    files = {
        "src/multi_thread/tcmalloc.cpp"
    }
}