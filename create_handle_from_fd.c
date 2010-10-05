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

int main(int argc, char *argv[])
{
	int file_fd;
	int handle_fd;
	int handle_sz;
	int ret, mnt_id;
        struct file_handle *fh = NULL;;

	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s,  <filename> <handle_file>\n", argv[0]);
		exit(1);
	}
	file_fd = open(argv[1], O_RDONLY);
	if(file_fd < 0)
		perror("open"), exit(1);
again:
	if (fh && fh->handle_bytes) {
		handle_sz = fh->handle_bytes;
		free(fh);
		fh = malloc(sizeof(struct file_handle) + handle_sz);
		fh->handle_bytes = handle_sz;
	} else {
		fh = malloc(sizeof(struct file_handle));
		fh->handle_bytes = 0;
	}
        errno  = 0;
        ret = fd_to_handle(file_fd, fh, &mnt_id);
        if (ret && errno == EOVERFLOW) {
		printf("Found the handle size needed to be %d\n", fh->handle_bytes);
		goto again;
        } else if (ret) {
                perror("Error:");
		exit(1);
	}

	/* write the handle to a handle.data file */
	handle_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0600);
	write(handle_fd, fh, sizeof(*fh) + fh->handle_bytes);
	printf("Handle value stored %s\n", argv[2]);

	close(handle_fd);
	close(file_fd);
	return 0;
}

