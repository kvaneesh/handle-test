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
	int ret, mnt_fd;
	int handle_fd, fd;
	struct file_handle handle, *fh;

	if(argc != 5)
	{
		fprintf(stderr, "Usage: %s, <mountdir> <handle-file> <orig-name> <link name>\n", argv[0]);
		exit(1);
	}
	/* read the handle from handle file */
	handle_fd = open(argv[2], O_RDONLY);
	if (handle_fd < 0)
		perror("Error"), exit(1);

	read(handle_fd, &handle, sizeof(struct file_handle));
	printf("Handle size read from %s is %d\n", argv[2], handle.handle_bytes);

	fh = malloc(sizeof(struct file_handle) + handle.handle_bytes);
	lseek(handle_fd, 0, SEEK_SET);
	/* read the full handle now */
	read(handle_fd, fh, sizeof(struct file_handle) + handle.handle_bytes);

	mnt_fd = open(argv[1], O_RDONLY | O_DIRECTORY);
	if(mnt_fd < 0)
		perror("open"), exit(1);

	printf("holding an open reference to the file\n");
	fd = open(argv[3], O_RDONLY);
	if (fd <= 0 ) {
		perror("Error:");
		exit(1);
	}
	printf("Now deleteing %s\n", argv[3]);
	unlink(argv[3]);
	printf("Creating hardlink to the handle\n");
	ret = handle_link(mnt_fd, fh, AT_FDCWD, argv[4]);
	if (ret && errno == ENOENT)
		printf("Success\n");
	else
		perror("Expected ENOENT but got:");

	close(mnt_fd);
	close(fd);
	close(handle_fd);
	return 0;
}
