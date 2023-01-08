{
    files = {
        "src/single_thread/main.cpp"
    },
    depfiles_gcc = "main.o: src/single_thread/main.cpp\
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