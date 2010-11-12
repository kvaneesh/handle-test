TARGETS=append_handle create_handle_from_fd create_handle hardlink_handle \
	read_handle truncate_handle hardlink_unlink_file_handle \
	chown_handle o_path_test
all: ${TARGETS}

clean:
	${RM}  ${TARGETS}

%: %.c
	$(CC) -Wall -Werror -o $@ $^
