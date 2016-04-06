#include "vsd_device.h"

#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "../vsd_driver/vsd_ioctl.h"

#define IS_ALIGNED(x, a)    (((x) & ((typeof(x))(a) - 1)) == 0)
#define PAGE_ALIGNED(addr, PAGE_SIZE)  IS_ALIGNED((unsigned long)addr, PAGE_SIZE)

static int vsd_descr = 0;

int vsd_init()
{
    vsd_descr = open("/dev/vsd", O_RDWR);
    if (vsd_descr == -1)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int vsd_deinit()
{
    close(vsd_descr);
    return EXIT_SUCCESS;
}

int vsd_get_size(size_t *out_size)
{
    vsd_ioctl_get_size_arg_t size_arg;
    if (ioctl(vsd_descr, VSD_IOCTL_GET_SIZE, &size_arg) == -1)
        return EXIT_FAILURE;

    *out_size = size_arg.size;
    return EXIT_SUCCESS;
}

int vsd_set_size(size_t size)
{
    vsd_ioctl_set_size_arg_t size_arg = {
        .size = size
    };

    if (ioctl(vsd_descr, VSD_IOCTL_SET_SIZE, &size_arg) == -1)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

ssize_t vsd_read(char* dst, off_t offset, size_t size)
{
    if (lseek(vsd_descr, offset, SEEK_SET) == -1)
        return EXIT_FAILURE;
    return read(vsd_descr, dst, size);
}

ssize_t vsd_write(const char* src, off_t offset, size_t size)
{
    if (lseek(vsd_descr, offset, SEEK_SET) == -1)
        return EXIT_FAILURE;
    return write(vsd_descr, src, size);
}

void* vsd_mmap(size_t offset)
{
    const int PAGE_SIZE = getpagesize();

    if (!PAGE_ALIGNED(offset, PAGE_SIZE))
        return MAP_FAILED;

    size_t vsd_size = 0;
    vsd_get_size(&vsd_size);
    return mmap(NULL, vsd_size - offset, PROT_READ | PROT_WRITE, MAP_SHARED, vsd_descr, offset);
}

int vsd_munmap(void* addr, size_t offset)
{
    const int PAGE_SIZE = getpagesize();

    if (!PAGE_ALIGNED(offset, PAGE_SIZE))
        return -1;

    size_t vsd_size = 0;
    vsd_get_size(&vsd_size);
    return munmap(addr, vsd_size - offset);
}

