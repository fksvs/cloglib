#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "../src/cloglib.h"

int main(int argc, char *argv[])
{
	log_info("welcome to cloglib !");
	log_info("there is 8 log levels, from %d to %d", 0, 7);
	log_info("messages can be formatted like %s", "printf()");

	log_warning("setting console stream to %s (%d)", "stdout", 1);
	set_stream(1);
	log_notice("this message is logged to stdout");

	/* setting log level for console */
	set_level(LOG_WARN);
	log_info("this message should not be shown");
	set_level(LOG_TRACE);
	log_info("this message should be shown");

	/* enabling colored output for console */
	set_color(true);
	log_fatal("outputs to console is colored now");
	set_color(false);

	/* setting custom log format */
	set_format("[%H:%M:%S] [%L] [%P/%t/%U] [%F:%l] : %A\n");
	log_info("output is now formatted customly");
	set_format(DEFAULT_FORMAT);

	/* initializing console settings with init_console() */
	init_console(2, LOG_TRACE, false, false, DEFAULT_FORMAT);

	/* creating basic log file */
	init_basic_log("basic.log", LOG_TRACE);

	/* creating rotated log file, with maximum size of 1024 and 5 files */
	init_rotate_log("rotate.log", LOG_DEBUG, 1024, 5);

	/* adding a file descriptor for logging */
        int fd;
        if ((fd = open("fd.log", O_APPEND | O_WRONLY | O_CREAT,
                       S_IRUSR | S_IWUSR)) == -1) {
                fprintf(stderr, "open : %s\n", strerror(errno));
                return -1;
        }
	add_fd_log(fd, LOG_TRACE);

	log_info("this message is logged to stderr, basic.log, rotate.log and fd.log");

	/* closing all log files */
	close_log_files();

	return 0;
}
