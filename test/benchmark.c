#include <stdio.h>
#include <time.h>
#include "../src/cloglib.h"

#define ITERATIONS 100000

void calculate(int iter)
{
	double start_time, end_time, total_time;
	int ind;

	start_time = (double)clock() / CLOCKS_PER_SEC;
	for (ind = 0; ind < iter; ind += 1) {
		cloglib_log(LOG_INFO, __FILE__, __LINE__,
			"log message : %d", ind);
	}
	end_time = (double)clock() / CLOCKS_PER_SEC;

	total_time = end_time - start_time;

	printf("%d iterations in %lf seconds\n", iter, total_time);
	printf("%0.2lf iterations per second\n", (double)iter / total_time);
}

void benchmark(int iter)
{
	printf("--------------------------------------\n");
	printf("benchmarking empty log...\n");
	set_quiet(true);
	calculate(iter);

	printf("--------------------------------------\n");
	printf("benchmarking basic log file...\n");
	init_basic_log("basic.log", LOG_INFO);
	calculate(iter);
	close_log_files();

	printf("--------------------------------------\n");
	printf("benchmarking rotated log file...\n");
	init_rotate_log("rotate.log", LOG_INFO, 1024 * 1024 * 5, 2);
	calculate(iter);
	close_log_files();
	
	printf("--------------------------------------\n");
}

int main(int argc, char *argv[])
{
	benchmark(ITERATIONS);
	return 0;
}
