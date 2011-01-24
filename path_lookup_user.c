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
#include <limits.h>


int main(int argc, char *argv[])
{
	int dir_fd = AT_FDCWD;
	char *path_comp;
	char path[PATH_MAX];

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s, <filename>\n", argv[0]);
		exit(1);
	}
	if (argv[1][0] == '/') {
		dir_fd = openat(0, "/", O_PATH | O_NOFOLLOW);
		if (dir_fd < 0) {
			printf("Failed to traverse the path\n");
			perror("Error");
			exit(1);
		}
	}

	/* We do O_PATH lookup of each argument */
	strcpy(path, argv[1]);
	path_comp = strtok(path, "/");
	while (path_comp)
	{
		dir_fd = openat(dir_fd, path_comp, O_PATH | O_NOFOLLOW);
		if (dir_fd < 0) {
			printf("Failed to traverse the path\n");
			perror("Error");
			exit(1);
		}
		path_comp = strtok(NULL, "/");
	}
	return 0;
}
