#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"
#include "instance.h"
#include "schedule.h"
#include "algo.h"

int main(int argc, char *argv[]) {
	if (argc != 3)
		ShowMessage("expe:main: invalid number of arguments", 1);

	char* infilename = argv[1];
	int m = atoi(argv[2]);

	clock_t start, end;

	Instance I = read_instance(infilename);
	struct schedule_t * S;

	start = clock();
	S = create_schedule(I, m, 0, 0);	// 0 : non-preemptive, 0 : non-balanced
	end = clock();
	double cpu_time_used00 = ((double) (end - start)) / CLOCKS_PER_SEC;
	unsigned long makespan00 = get_makespan(S);
	delete_schedule(S);

	start = clock();
	S = create_schedule(I, m, 0, 1); // 0 : non-preemptive, 1 : balanced
	end = clock();
	end = clock();
	double cpu_time_used01 = ((double) (end - start)) / CLOCKS_PER_SEC;
	unsigned long makespan01 = get_makespan(S);
	delete_schedule(S);

	start = clock();
	S = create_schedule(I, m, 1, 0); // 1 : preemptive, 0 : non-balanced
	end = clock();
	end = clock();
	double cpu_time_used10 = ((double) (end - start)) / CLOCKS_PER_SEC;
	unsigned long makespan10 = get_makespan(S);
	delete_schedule(S);

	start = clock();
	S = create_schedule(I, m, 1, 1);  // 1 : preemptive, 1 : balanced
	end = clock();
	double cpu_time_used11 = ((double) (end - start)) / CLOCKS_PER_SEC;
	unsigned long makespan11 = get_makespan(S);
	delete_schedule(S);

	delete_instance(I, 1);

	if (makespan00 != makespan01)
		ShowMessage("Preemtive : The makespan is not the same when using balanced and non balanced trees.\n", 1);
	if (makespan10 != makespan11)
		ShowMessage("Non-Preemtive : The makespan is not the same when using balanced and non balanced trees.\n", 1);

	printf("%lu %lf %lu %lf %lu %lf %lu %lf", makespan00, cpu_time_used00,
											  makespan01, cpu_time_used01,
											  makespan10, cpu_time_used10,
											  makespan11, cpu_time_used11);

	return EXIT_SUCCESS;
}