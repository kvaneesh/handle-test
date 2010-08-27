/*
 *   Copyright (C) International Business Machines  Corp., 2010
 *   Author(s): Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "handle.h"

#define BUFSZ 100
int main(int argc, char *argv[])
{
	int ret;
	int fd, mnt_fd;
	int handle_fd;
	struct stat64 bufstat;
        char buf[BUFSZ];
	struct file_handle handle, *fh;

	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s, <mountdir> <handle-file>\n", argv[0]);
		exit(1);
	}
	/* read the handle from handle file */
	handle_fd = open(argv[2], O_RDONLY);
	if (handle_fd < 0)
		perror("Error"), exit(1);

	read(handle_fd, &handle, sizeof(struct file_handle));
	printf("Handle size read from %s is %d\n", argv[2], handle.handle_size);

	fh = malloc(sizeof(struct file_handle) + handle.handle_size);
	lseek(handle_fd, 0, SEEK_SET);
	/* read the full handle now */
	read(handle_fd, fh, sizeof(struct file_handle) + handle.handle_size);

	mnt_fd = open(argv[1], O_RDONLY | O_DIRECTORY);
	if(mnt_fd < 0)
		perror("open"), exit(1);

	printf("Writing with O_APPEND flag \n");
	handle_stat(mnt_fd, fh, &bufstat);
	ret = S_ISLNK(bufstat.st_mode);
	if (ret) {
		printf("Operation not permitted on symlink\n");
	} else {
		/* Not a symlink */
		fd = open_by_handle(mnt_fd, fh, O_RDWR | O_APPEND);
		if (fd <= 0 ) {
			perror("Error:");
			exit(1);
		}
		write(fd, "This should be last line", 24);
		lseek(fd, 0, SEEK_SET);
		memset(buf, 0 , BUFSZ);
		while (1) {
			ret = read(fd, buf, BUFSZ -1);
			if (ret <= 0)
				break;
			buf[ret] = '\0';
			printf("%s", buf);
			memset(buf, 0 , BUFSZ);
		}
		close(fd);

	}
	close(mnt_fd);
	close(handle_fd);
	return 0;
}

