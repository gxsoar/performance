{
    values = {
        "/usr/bin/g++",
        {
            "-m64",
            "-Lbuild/linux/x86_64/debug",
            "-lmulti_thread_allocator",
            "-ltcmalloc",
            "-lpthread"
        }
    },
    files = {
        "build/.objs/multi_thread_allocator_test/linux/x86_64/debug/src/multi_thread/allocator_test.cpp.o",
        "build/linux/x86_64/debug/libmulti_thread_allocator.a"
    }
}