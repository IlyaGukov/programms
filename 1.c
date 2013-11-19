#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void get_vector(int size, int fd);

int error(char* s);

int deg(int k);

int get_size(char* st);

int write_size_to_file(int fd, int size);

int main(int argc, char** argv)
{
	int size, fd; 
    	if (argc < 2)
    	{
		printf("not enough arguments \n");
		exit(-1);
    	}
    	size = get_size(argv[1]);
	umask(0);
	if ((fd = open("problem.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0)
	{
		printf("cannot open file \n");
		exit(-1);
	}
	write_size_to_file(fd,size);
	int i = 0;
	for (;i<size;i++)
	{	
		get_vector(size,fd);
		write(fd,"\n",1);
	}
	// AP: а где проверки на ошибки всех системных вызовов? write, close и т.п.? исправить!
	write(fd,"\n",1);
	get_vector(size,fd);
	if (close(fd) < 0) error("cannot close file \n");
	return 0;
}
			

void get_vector(int size, int fd)
{ 
	int k = 0, l = 1, j = 0, element;
	char* forfile;
	forfile = (char*)malloc(sizeof(char));
	for (j=0;j<size;j++)
	{
		element = (int)random();
		element%=15;
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
int error(char* s)
{ 
    int n;
    n = strlen(s);
    write(2,s,n+1);
    exit(-1);
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
	if ((st[i] <'0' || st[i]>'9') && st[i]!=' ') error("invalid size"); 
	
    }
    return size;
}
int write_size_to_file(int fd, int size)
{	
	char* forfile;
	int k = 0, l = 1;
	forfile = (char*)malloc(sizeof(char));
	while ((size/deg(l)) !=0) l++; 
	for (;k<l;k++)
	{
		 forfile[0] = (char)(size/deg(l-k-1)) + '0';
		 size%=deg(l-1-k); 
		 if (write(fd,forfile,1) < 0) error("cannot write to file \n");
	}
	write(fd,"\n",1);
	free(forfile);
	return 0;
}	
	
