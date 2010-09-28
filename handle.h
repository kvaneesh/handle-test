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
        int handle_size;
        int handle_type;
        unsigned char handle[0];
};

#define AT_FDCWD		-100
#define AT_SYMLINK_FOLLOW	0x400

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
	return syscall(343, mountfd, fh, buf, bufsize);
}

static inline int handle_stat(int mountfd, struct file_handle *fh, struct stat64 *buf)
{
	return syscall(344, mountfd, fh, buf);
}

static inline int handle_link(int mountfd, struct file_handle *fh, int newdirfd, char *newname)
{
	return syscall(345, mountfd, fh, newdirfd, newname);
}

static inline int handle_chown(int mountfd, struct file_handle *fh, uid_t owner, gid_t group)
{
	return syscall(346, mountfd, fh, owner, group);
}

#endif