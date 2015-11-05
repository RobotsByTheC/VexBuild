#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	execvp("python3", argv);
	int error = execvp("python", argv);
	fprintf(stderr,
			"Error: Could not run python, is you PATH configured properly?\n");
	return error;
}
