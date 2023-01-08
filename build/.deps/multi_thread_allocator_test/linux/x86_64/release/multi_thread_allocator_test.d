{
    files = {
        "build/.objs/multi_thread_allocator_test/linux/x86_64/release/src/multi_thread/allocator_test.cpp.o",
        "build/linux/x86_64/release/libmulti_thread_allocator.a"
    },
    values = {
        "/usr/bin/g++",
        {
            "-m64",
            "-Lbuild/linux/x86_64/release",
            "-s",
            "-lmulti_thread_allocator",
            "-lpthread"
        }
    }
}