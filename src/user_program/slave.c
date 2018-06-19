#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define PAGE_SIZE 4096
#define BUF_SIZE 512

#define DEST_FILE_PERMISSION (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

struct dev_mmap_arg {
	size_t count;
	size_t offset;
};

int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	char mbuf[PAGE_SIZE];
	char *file_addr, *dev_addr;
	int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret, file_size = 0, data_size = -1;
	char file_name[50];
	char method[20];
	char ip[20];
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate the time between the device is opened and it is closed
	char *kernel_address, *file_address;
	struct dev_mmap_arg dm_arg;

	strcpy(file_name, argv[1]);
	strcpy(method, argv[2]);
	strcpy(ip, argv[3]);

	if( (dev_fd = open("/dev/slave_device", O_RDWR)) < 0)//should be O_RDWR for PROT_WRITE when mmap()
	{
		perror("failed to open /dev/slave_device\n");
		return 1;
	}
	gettimeofday(&start ,NULL);
	if( (file_fd = open (file_name, O_RDWR | O_CREAT | O_TRUNC, DEST_FILE_PERMISSION)) < 0)
	{
		perror("failed to open input file\n");
		return 1;
	}

	if(ioctl(dev_fd, 0x12345677, ip) == -1)	//0x12345677 : connect to master in the device
	{
		perror("ioclt create slave socket error\n");
		return 1;
	}

	switch(method[0])
	{
		case 'f'://fcntl : read()/write()
			do
			{
				ret = read(dev_fd, buf, sizeof(buf)); // read from the the device
				write(file_fd, buf, ret); //write to the input file
				file_size += ret;
			}while(ret > 0);
			break;
		default: // mmap
			dev_addr = mmap(NULL, PAGE_SIZE, PROT_READ, MAP_PRIVATE, dev_fd, 0);
			while(1) {
				data_size = 0;
				dm_arg.count = PAGE_SIZE;
				dm_arg.offset = 0;
				while(data_size < PAGE_SIZE) {
					ret = ioctl(dev_fd, 0x12345678, &dm_arg);
					if(ret == -1) {
						perror("ioclt slave mmap error");
						return 1;
					}
					if(ret == 0)
						break;

					data_size += ret;
					dm_arg.count -= ret;
					dm_arg.offset += ret;
				}
				if(data_size == 0)
					break;

				posix_fallocate(file_fd, file_size, data_size);
				file_addr = mmap(NULL, data_size, PROT_WRITE, MAP_SHARED, file_fd, file_size);
				if(file_addr == MAP_FAILED) {
					perror("file mmap error"); 
					return 1;
				}

				memcpy(file_addr, dev_addr, data_size);
				munmap(file_addr, data_size);
				file_size += data_size;
			}
			munmap(dev_addr, PAGE_SIZE);
	}

	if(ioctl(dev_fd, 0x12345679) == -1)// end receiving data, close the connection
	{
		perror("ioclt client exits error\n");
		return 1;
	}
	gettimeofday(&end, NULL);
	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
	printf("Transmission time: %lf ms, File size: %lu bytes\n", trans_time, file_size);


	close(file_fd);
	close(dev_fd);
	return 0;
}


