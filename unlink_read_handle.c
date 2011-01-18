/*
 *   Copyright (C) International Business Machines  Corp., 2011
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

/* Now read using the handle */
static int read_from_handle(int mnt_fd, struct file_handle *fh)
{
	int readfd, ret;
        char buf[BUFSZ];

	readfd = open_by_handle(mnt_fd, fh, O_RDONLY);
	if (readfd <= 0 ) {
		return readfd;
	}
	memset(buf, 0 , BUFSZ);
	while (1) {
		ret = read(readfd, buf, BUFSZ -1);
		if (ret <= 0)
			break;
		buf[ret] = '\0';
		printf("%s", buf);
		memset(buf, 0 , BUFSZ);
	}
	close(readfd);
	return 0;
}

int main(int argc, char *argv[])
{
	int fd;
	int ret, mnt_id;
	int handle_sz;
        struct file_handle *fh = NULL;;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s  <filename>\n", argv[0]);
		exit(1);
	}
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
        ret = lname_to_handle(argv[1], fh, &mnt_id);
        if (ret && errno == EOVERFLOW) {
		printf("Found the handle size needed to be %d\n", fh->handle_bytes);
		goto again;
        } else if (ret) {
                perror("Error:");
		exit(1);
	}
	/* Now hold an open reference to the file and delete the file */
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		printf("Error in opening file %s\n", argv[1]);
		exit(1);
	}
	printf("Trying to unlink the file %s\n", argv[1]);
	unlink(argv[1]);
	if (read_from_handle(AT_FDCWD, fh) < 0) {
		printf("Error failed to read\n");
		exit(1);
	}
	/* Now close the open fd */
	close(fd);
	if (read_from_handle(AT_FDCWD, fh) == 0) {
		printf("Error Still able to read\n");
		exit(1);
	}
	return 0;
}
