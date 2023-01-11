{
    files = {
        "src/single_thread/ngx_test.cpp"
    },
    depfiles_gcc = "ngx_test.o: src/single_thread/ngx_test.cpp src/single_thread/ngx_malloc.h\
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