#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

void * Check(size_t size)
{
return mmap(NULL, size, PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
}

int main()
{
	size_t all;
    size_t r = -1;
    size_t i = 0;
    for (i = r; i > 0; i /= 2)
    {
    	void* res = Check(i);
    	while (res != MAP_FAILED)
    	{
    		all += i;
    		res = Check(i);
    	}
    }

    printf("%zu\n", all); 
    return 0;
}
