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

#ifndef HANDLE_H
#define HANDLE_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

struct file_handle {
        unsigned int handle_bytes;
        int handle_type;
        unsigned char handle[0];
};

#define AT_FDCWD		-100
#define AT_SYMLINK_FOLLOW	0x400
#define O_NOACCESS     00000003
#define __O_PATH       010000000
#define O_PATH         (__O_PATH | O_NOACCESS)

static inline int name_to_handle(const char *name, struct file_handle *fh, int *mnt_id)
{
	return syscall(341, AT_FDCWD, name, fh, mnt_id, AT_SYMLINK_FOLLOW);
}

static inline int lname_to_handle(const char *name, struct file_handle *fh, int *mnt_id )
{
	return syscall(341, AT_FDCWD, name, fh, mnt_id, 0);
}

static inline int fd_to_handle(int fd, struct file_handle *fh, int *mnt_id)
{
	return syscall(341, fd, NULL, fh, mnt_id, 0);
}

static inline int open_by_handle(int mountfd, struct file_handle *fh, int flags)
{
	return syscall(342, mountfd, fh, flags);
}

static inline ssize_t handle_readlink(int mountfd, struct file_handle *fh, char *buf, size_t bufsize)
{
	int fd, ret;
	fd = open_by_handle(mountfd, fh, O_PATH);
	if (fd < 0)
		return fd;
	ret = readlinkat(fd, "", buf, bufsize);
	close(fd);
	return ret;
}

static inline int handle_stat(int mountfd, struct file_handle *fh, struct stat *buf)
{
	int fd, ret;
	fd = open_by_handle(mountfd, fh, O_PATH);
	if (fd < 0)
		return fd;
	ret = fstat(fd, buf);
	close(fd);
	return ret;
}

static inline int handle_link(int mountfd, struct file_handle *fh, int newdirfd, char *newname)
{
	int fd, ret;
	fd = open_by_handle(mountfd, fh, O_PATH);
	if (fd < 0)
		return fd;
	ret = linkat(fd, "", newdirfd, newname, 0);
	close(fd);
	return ret;
}

static inline int handle_chown(int mountfd, struct file_handle *fh, uid_t owner, gid_t group)
{
	int fd, ret;
	fd = open_by_handle(mountfd, fh, O_PATH);
	if (fd < 0)
		return fd;
	ret = fchown(fd, owner, group);
	close(fd);
	return ret;
}

#endif
