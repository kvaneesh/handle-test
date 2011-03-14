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
	int fd;
	char buf[10];

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s, <filename>\n", argv[0]);
		exit(1);
	}
	fd = open(argv[1], O_PATH);
	if (unlinkat(AT_FDCWD, argv[1], 0) == 0)
		printf("Unlink test passed\n");
	else
		printf("Unlink Test failed\n");


	if (read(fd, buf, 10) >= 0)
		printf("Read Test failed\n");
	else
		printf("Read test passed\n");
	return 0;
}
