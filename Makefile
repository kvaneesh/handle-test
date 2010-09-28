TARGETS=append_handle create_handle_from_fd create_handle hardlink_handle read_handle truncate_handle
all: ${TARGETS}

clean:
	${RM}  ${TARGETS}

%: %.c
	$(CC) -Wall -Werror -o $@ $^