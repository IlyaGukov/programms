 (thr[thread_number - 1]).mat = (matrix + size*size*(thread_number - 1)/thread_number);

   (thr[thread_number - 1]).mat = (matrix + size*size - (size * (thr[thread_number - 1]).num_of_iter));

   (thr[thread_number - 1]).sum_start = (size - (thr[thread_number - 1]).num_of_iter );

+  // AP: зачем отдельно запускть эту функцию? чтобы main тоже участвовал в вычислении. В цикле создается thread_number - 1
   // и ещё одна нить сам main

   for_thread(thr + thread_number - 1);

   for (; j < (thread_number - 1); j++ ) pthread_join(thid[j], NULL);

   free(thid);
