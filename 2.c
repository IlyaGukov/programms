#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define BUF_SIZE 20
typedef struct forthr
{
	int* mat;
	int* vec;
	int size;
	int num_of_iter;
	int sum_start;
}	forthr;
int number_size = 0;

char* normal_strcat(char* s1, char* s2, int size, int rr);

int* result_vector = NULL;

int deg(int k);

int get_number(char* st);

int get_size(char* st);

int my_error(char* s);

void* for_thread (void* arg);

void write_result_to_file(int *result, int size, int fd);

int main(int argc, char** argv)
{
	time_t time1;
	time_t time2;
	int fd, thread_number;
	if (argc < 2) my_error("not enough arguments \n");
	thread_number = get_size(argv[1]);
	if ((fd = open("problem.txt",O_RDONLY)) < 0) my_error("file open error \n");
	int* matrix;
	int* vector;
	int result, i = 0, n = 0, size = 0, pre_matrix = 0;
	char* workstr = NULL;
	workstr = (char*)malloc(sizeof(char)*BUF_SIZE);
	char* restring = NULL;
	restring = (char*)malloc(sizeof(char));
	int strsize = 0;
	time2 = time(NULL);
	while((result = read(fd,workstr,BUF_SIZE)) > 0)
	{	
		restring = realloc(restring,sizeof(char)*(strsize + result));
		normal_strcat(restring,workstr,strsize,result);
		strsize+=result;
	}
	if (result < 0) my_error(" reading error \n");
	size = get_number(restring);
	pre_matrix = number_size + 1;
	matrix = (int*)malloc(sizeof(int)*(size*size));
	vector = (int*)malloc(sizeof(int)*size);
	i = 0;
	for (;i < (size*size); i++)	
	{	
		matrix[i] = get_number(restring + n + pre_matrix);
		n += (number_size + 1);	
	} 
	n++;
	i = 0;
	for (;i < size; i++)
	{	
		vector[i] = get_number(restring + n + pre_matrix);
		n += (number_size+1);
	}
	result_vector = (int*)calloc(size,sizeof(int));
	free(restring);
	free(workstr);
	i = 0;
	int j = 0;
	pthread_t *thid;
	thid = (pthread_t*)malloc(sizeof(pthread_t)*thread_number);
	forthr *thr;
	thr = (forthr*)malloc(sizeof(forthr)*thread_number);
	int k = 0;
	// AP: сделайте вывод с точостью до микросекунд
	time2 = time(NULL) - time2;
	printf("reading from problem file time is  %i \n", (int)time2);
	for (; k < (thread_number - 1); k++)
	{
		(thr[k]).mat = (matrix + k*size*(size/thread_number));
		(thr[k]).vec = vector;
		(thr[k]).size = size;
		(thr[k]).num_of_iter = (size/thread_number);
		(thr[k]).sum_start = k*(thr[k]).num_of_iter;
	}		
	k = 0;
	time1 = time(NULL);
	for (; k < (thread_number - 1); k++) if (pthread_create(thid + k,NULL,&for_thread,thr + k) != 0) my_error ("thread creating fault \n");
	(thr[thread_number - 1]).vec = vector;
	(thr[thread_number - 1]).size = size;
	(thr[thread_number - 1]).num_of_iter = (size/thread_number + size%thread_number);
	(thr[thread_number - 1]).mat = (matrix + size*size*(thread_number - 1)/thread_number);
	(thr[thread_number - 1]).mat = (matrix + size*size - (size * (thr[thread_number - 1]).num_of_iter));
	(thr[thread_number - 1]).sum_start = (size - (thr[thread_number - 1]).num_of_iter );
	// AP: зачем отдельно запускть эту функцию?
	for_thread(thr + thread_number - 1);
	for (; j < (thread_number - 1); j++ ) pthread_join(thid[j], NULL);
	free(thid);
	free(thr);		
	int fd1;
	time1 = time(NULL) - time1;
	printf("execution time is %i \n", (int)time1);
	if ((fd1 = open("result.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0)
	{
		printf("cannot open file \n");
		exit(-1);
	}
	write_result_to_file(result_vector, size, fd1);
	free(matrix);
	free(vector);
	free(result_vector);
	if (close(fd) < 0) error("cannot close file \n");
	if (close(fd1) < 0) error("cannot close file \n");
	return 0;	
}  
int deg(int k)    
{
    	int a=1,i=0;
    	for (;i<k;i++) a*=10;
    	return a;
}
int get_size(char* st)
{
    	int i=0, size=0;	
	for(;i<strlen(st);i++)
    	{
		if ((st[i]>='0') && (st[i] <='9')) size=size*10+st[i]-'0';
		if ((st[i] <'0' || st[i]>'9') && st[i]!=' ') my_error("invalid size"); 
	
    	}
    	return size;
}		
int get_number(char* st)
{
    	int i=0, number=0;	
	while ((st[i] != ' ') && (st[i] != '\n'))
    	{
		if ((st[i]>='0') && (st[i] <='9')) number = number*10+st[i]-'0';
		if (((st[i] <'0') || (st[i]>'9')) && (st[i]!=' ')) my_error("invalid number \n");
		if ((st[i+1] == ' ') && (st[i+2] == '\n')) i++;
	i++; 
    	}
	number_size = i;
    	return number;
}		
int my_error(char* s)
{ 
    	int n;
    	n = strlen(s);
    	write(2,s,n+1);
    	exit(-1);
}
void* for_thread (void* arg)
{
	forthr *ft;
	int i = 0, j = 0,k;
	ft = (forthr*)arg;
	k = ft->sum_start;
	for (;i < ft->num_of_iter; i++)
	{
		for (; j < ft->size; j++) result_vector[k] += ((ft->mat)[j + (i * ft->size)] * (ft->vec)[j]);
		k++;
		j = 0;
	}
	return NULL;		
}
void write_result_to_file(int *result,int size, int fd)
{ 
	int k = 0, l = 1, j = 0, element;
	char* forfile;
	forfile = (char*)malloc(sizeof(char));
	for (j=0;j<size;j++)
	{
		element = result[j];
		while ((element/deg(l)) !=0) l++; 
		for (;k<l;k++)
		{
		 	forfile[0] = (char)(element/deg(l-k-1)) + '0';
			element%=deg(l-1-k); 
			if (write(fd,forfile,1) < 0) error("cannot write to file \n");
		}
		write(fd," ",1);
		k = 0;
		l = 1;
	}
	free(forfile);
	return;
}
char* normal_strcat(char* s1, char* s2, int size, int rr)
{
    	int i = 0;
	for(;i<rr;i++) s1[i + size] = s2[i];
    	return s1;
}
