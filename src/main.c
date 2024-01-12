#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "instance.h"
#include "schedule.h"
#include "algo.h"

void display_help() {
	ShowMessage("", 0);
	ShowMessage("make run in=infilename out=outfilename m=machines pre=preemption bal=balanced\n", 0);
	ShowMessage("preemption:", 0);
	ShowMessage("\t0 - without preemption", 0);
	ShowMessage("\t1 - with preemption", 0);
	ShowMessage("", 0);
	ShowMessage("balanced:", 0);
	ShowMessage("\t0 - use a binary search tree", 0);
	ShowMessage("\t1 - use a balanced binary search tree", 0);
	ShowMessage("", 1);
}

int main(int argc, char *argv[]) {
	if (argc != 6)
		display_help();

	char* infilename = argv[1];
	char* outfilename = argv[2];
	int m = atoi(argv[3]);
	int preemption = atoi(argv[4]);	
	int balanced = atoi(argv[5]);

	Instance I = read_instance(infilename);
	struct schedule_t * S = create_schedule(I, m, preemption, balanced);
	printf("The makespan of the constructed schedule is %lu\n\n", get_makespan(S));
	view_schedule(S);
	save_schedule(S, outfilename);
	delete_schedule(S);
	delete_instance(I, 1);

	return EXIT_SUCCESS;
}