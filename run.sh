for ((i = 1; i <= 10; ++ i));
do
	xmake r multi_thread_tcmalloc_test 10M;
done

