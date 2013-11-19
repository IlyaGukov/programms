 // AP: а где проверки на ошибки всех системных вызовов? write, close и т.п.? исправить!

   write(fd,"\n",1);

   get_vector(size,fd);

+  // AP: а где запись матрицы? запись матрицы идет выше, в цикле. Цикл вызывает get_vector size раз 

   if (close(fd) < 0) error("cannot close file \n");

   return 0;

 }
