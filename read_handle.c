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
	char *handle_file;
	struct file_handle handle, *fh;

	if(argc != 2 && argc != 3)
	{
		fprintf(stderr,
			"Usage: %s, [<mountdir>] <handle-file>\n", argv[0]);
		exit(1);
	}
	if (argc == 2)
	{
		mnt_fd = AT_FDCWD;
		handle_file = argv[1];
	} else
		handle_file = argv[2];
	/* read the handle from handle file */
	handle_fd = open(handle_file, O_RDONLY);
	if (handle_fd < 0)
		perror("Error"), exit(1);

	read(handle_fd, &handle, sizeof(struct file_handle));
	printf("Handle size read from %s is %d\n", argv[2], handle.handle_size);

	fh = malloc(sizeof(struct file_handle) + handle.handle_size);
	lseek(handle_fd, 0, SEEK_SET);
	/* read the full handle now */
	read(handle_fd, fh, sizeof(struct file_handle) + handle.handle_size);

	if (argc != 2) {
		mnt_fd = open(argv[1], O_RDONLY | O_DIRECTORY);
		if(mnt_fd < 0)
			perror("open"), exit(1);
	}
	printf("Reading the content now \n");
	handle_stat(mnt_fd, fh, &bufstat);
	ret = S_ISLNK(bufstat.st_mode);
	if (ret) {
		memset(buf, 0 , BUFSZ);
		handle_readlink(mnt_fd, fh, buf, BUFSZ);
		printf("handle is a symlink pointing to %s\n",  buf);
	} else {
		/* Not a symlink */
		fd = open_by_handle(mnt_fd, fh, O_RDONLY);
		if (fd <= 0 ) {
			perror("Error:");
			exit(1);
		}
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
