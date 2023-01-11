{
    files = {
        "build/.objs/ngx_test/linux/x86_64/debug/src/single_thread/ngx_test.cpp.o",
        "build/linux/x86_64/debug/libngx_malloc.a"
    },
    values = {
        "/usr/bin/g++",
        {
            "-m64",
            "-Lbuild/linux/x86_64/debug",
            "-lngx_malloc",
            "-ltcmalloc"
        }
    }
}