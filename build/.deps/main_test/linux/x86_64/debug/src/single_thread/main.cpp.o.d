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
            "-g",
            "-O0",
            "-std=c++14"
        }
    }
}