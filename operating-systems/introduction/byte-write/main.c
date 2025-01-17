#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MB 1000000

int main(int argc, char **argv) {
	char *fname = argv[1];
	FILE *fd    = fopen(fname, "wb");
	if (!fd) {
		perror("Failed to open file");
		return 1;
	}
	
	unsigned char data[] = {0x62, 0x75, 0x74, 0x74, 0x73};
	size_t datalen 	     = sizeof(data);

	struct statx stx;

	int 	   atflag = AT_SYMLINK_NOFOLLOW;
	unsigned int mask = STATX_SIZE;

	if (statx(AT_FDCWD, fname, atflag, mask, &stx) != 0) {
		perror("statx error");
		return 1;
	}

	unsigned long long prev_blocks = stx.stx_blocks;

	printf("Initial block size %llu\n", prev_blocks);

	for (unsigned long long i = 0; i < MB; i++) {
		fputc(data[i % datalen], fd);
		statx(AT_FDCWD, fname, atflag, mask, &stx);
		if (stx.stx_blocks != prev_blocks) {
			printf("File block size changed from %llu to %llu when writing byte %llu\n",
				prev_blocks, stx.stx_blocks, i);
			prev_blocks = stx.stx_blocks;
		}
	}
	fclose(fd);
	return 0;
}
