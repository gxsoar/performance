{
    files = {
        "src/single_thread/ngx_malloc.cpp"
    },
    depfiles_gcc = "ngx_malloc.o: src/single_thread/ngx_malloc.cpp  src/single_thread/ngx_malloc.h\
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