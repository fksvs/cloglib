CLOGLIB
=======

ClogLib is small, easy to use and fast C/C++ logging library.

## Table of Contents

1. [Features](#Features)
2. [Installation](#Installation)
3. [Functions](#Functions)
4. [Custom Formatting](#Custom-Formatting)
5. [Examples](#Examples)
6. [Benchmark](#Benchmark)
7. [License](#License)

## Features

- Relatively fast (avg. 200.000 log/sec)
- Different log targets:
	- Console logging (stdout or stderr)
	- Basic logging file
	- Rotated logging file
	- Externally added file descriptors
- Colored output on console
- Custom log formatting

## Installation

- Clone the repository from [GitHub][] or [GitLab][]:

		git clone https://github.com/fksvs/cloglib
		git clone https://gitlab.com/fksvs/cloglib

- Change directory to `cloglib`:

		cd cloglib/

- Copy `cloglib.c` and `cloglib.h` from `src/` to your project path:

		cp src/cloglib.* [your_project_path]

- Include `cloglib.h` in your project and compile with it.

Functions
---------

#### `set_stream(int stream)`

Set output stream for console. Stream can only be `stdout (1)` or `stderr (2)`.

#### `set_level(int log_level)`

Set log level for console. Default log level is `LOG_TRACE`.

#### `set_quiet(bool quiet)`

#### `set_color(bool color)`

Enable or disable colored output on console. `true` for enabling colored output and `false` for disabling. Color is disabled by default.

#### `set_format(char *log_format)`

Set custom log format for console and log files. See Custom Formatting for further information.

#### `init_console(int stream, int log_level, bool quiet, bool color, char *log_format)`

Initialize console for logging. Calls `set_stream()`, `set_level()`, `set_quiet()`, `set_color()` and `set_format` internally.

#### `init_file_log(char *filename, int log_level, int log_type, size_t max_file_size, size_t max_rotation)`

Initialize a log file. `log_type` is a integer value to determine the file type. It could be `BASIC_LOG` for basic log files or `ROTATE_LOG` for rotating log files

#### `init_basic_log(char *filename, int log_level)`

Initialize basic log file.

#### `init_rotate_log(char *filename, int log_level, size_t max_file_size, size_t max_rotation)`

Initailize rotating log file.

#### `add_fd_log(int fd, intlog_level)`

Add a file descriptor to log targets.

#### `close_log_files()`

Close opened log files, should be called while closing program.

#### `cloglib_log(int log_level, char *log_file, int log_line, const char *msg_format, ...)`

Main logging function. Generates log message and writes it to console and log files. Logging could be done with function-like macros:

```
log_fatal(const char *msg_format, ...) or log_f(const char *msg_format, ...)
log_critical(const char *msg_format, ...) or log_c(const char *msg_format, ...)
log_error(const char *msg_format, ...) or log_e(const char *msg_format, ...)
log_warning(const char *msg_format, ...) or log_w(const char *msg_format, ...)
log_notice(const char *msg_format, ...) or log_n(const char *msg_format, ...)
log_info(const char *msg_format, ...) or log_i(const char *msg_format, ...)
log_debug(const char *msg_format, ...) or log_d(const char *msg_format, ...)
log_trace(const char *msg_format,...) or log_t(const char *msg_format, ...)
```

## Custom Formatting

Log message format could be customly formatted with `set_format()`
Default log format is specified in `cloglib.h` as `DEFAULT_FORMAT` in the form of:

	[10.02.2023] CRITICAL [source_file.c:149] Critical error !

#### Flags:

- `%T` : Date and Time (`14.04.2023 15:22:46`)
- `%d` : Day of the month 01-31 (`14`)
- `%m` : Month 01-12 (`04`)
- `%Y` : Year (`2023`)
- `%H` : Hours 00-23 (`15`)
- `%M` : Minutes 00-59 (`18`)
- `%S` : Seconds 00-59 (`54`)
- `%L` : Log level string (`"NOTICE"`)
- `%P` : Logger process ID (`58421`)
- `%t` : Logger thread ID (`56439`)
- `%U` : Logger user ID (`1000`)
- `%l` : Source line (`297`)
- `%F` : Source file (`"source_file.c"`)
- `%A` : Log message (`"This is a log message"`)
- `%%` : `%` character

#### Setting Custom Format:

		set_format("[%H:%M:%S] [%L] [%P/%t/%U] [%F:%l] : %A\n")

After setting the custom format above, log messages would be in the form of:

		[23:38:10] [INFO] [23421/4328/1000] [source_file.c:243] : socket initialized

#### Setting Format to Default:

`DEFAULT_FORMAT` is the default log format.

		set_format(DEFAULT_FORMAT)

## Examples

Sample program located in `test/` as `example.c`. Build it with:

		make example

Change directory to `test/` and run the example:

		cd test/
		./example

##### Console Logging:

```
#include "cloglib.h"

int main(int argc, char *argv[])
{
        log_info("default console stream is stderr");
        log_info("color is disabled by default");

        /* setting stream to stdout and enabling color */
        set_stream(1);
        set_color(true);

        log_notice("this message is logger to stdout and color is enabled");

        /* setting custom format */
        set_format("[%S] [%L] [%P/%t] [%F:%l] : %A\n");
        log_info("log format changed");

        /* initializing console with init_console() */
        init_console(2, LOG_FATAL, false, true, DEFAULT_FORMAT);

        return 0;
}
```

##### Basic Logging File:

```
#include "cloglib.h"

int main(int argc, char *argv[])
{
	char *filename = "basic.log";

	/* initializing a basic log file with LOG_INFO log level */
	init_basic_log(filename, LOG_INFO);

	log_info("this message is logged to %s", filename);

	/* closing log file */
	close_log_files();

	return 0;
}
```

##### Rotated Logging File:

```
#include "cloglib.h"

int main(int argc, char *argv[])
{
        char *filename = "rotate.log";
        size_t max_size = 1024;
        size_t max_file = 5;

        /* initializing a rotated log file */
        /* maximum file size is 1024 bytes with 5 files */
        init_rotate_log(filename, LOG_FATAL, max_size, max_file);

        /* closing log file */
        close_log_files();

        return 0;
}
```

## Benchmark

Benchmarks done in Void Linux, Intel i5-8265U @ 3.9GHz

```
--------------------------------------
benchmarking empty log...
100000 iterations in 0.306677 seconds
326075.97 iterations per second
--------------------------------------
benchmarking basic log file...
100000 iterations in 0.457158 seconds
218742.75 iterations per second
--------------------------------------
benchmarking rotated log file...
100000 iterations in 0.548958 seconds
182163.30 iterations per second
--------------------------------------
```

To run benchmark on your system, build the source:

		make benchmark

Change directory to `test/` and run `benchmark`

		cd test/
		./benchmark

## License

This project is free software; you can redistribute it and/or modify it under the terms of the GPLv3 license. See [LICENSE][] for details.

[GitHub]: https://github.com/fksvs/cloglib
[GitLab]: https://gitlab.com/fksvs/cloglib
[LICENSE]: https://www.gnu.org/licenses/gpl-3.0.en.html
