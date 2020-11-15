#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct path {
	int *nodes;
	int size;
} path;

int main(int argc, char *argv[]) {
	int ants = atoi(argv[1]);
	int npaths = (argc - 2);
	path *paths = malloc((argc - 2) * sizeof(path));

	for (int i = 2; i < argc; ++i) {
		paths[i-2].size = atoi(argv[i]);
		paths[i-2].nodes = malloc(paths[i-2].size * sizeof(int));
		for (int j = 0; j < paths[i-2].size; ++j)
			paths[i-2].nodes[j]	= -1;
	}

	int i = 0;
	while (i < ants) {
		bool used = false;
		for (int j = 0; j < npaths; ++j)
			if (ants - i > paths[j].size) {
				used = true;
				paths[j].nodes[0] = i++;
			}
		if (!used)
			paths[0].nodes[0] = i++;
		used = false;
		for (int j = 0; j < npaths; ++j) {
			for (int k = paths[j].size - 1; k; --k) {
				if (paths[j].nodes[k - 1] != -1)
					printf("%d[%d]=%d ", j, k - 1, paths[j].nodes[k - 1]);
				/* getchar(); */
				paths[j].nodes[k] = paths[j].nodes[k - 1];
			}
			paths[j].nodes[0] = -1;
		}
		printf("\n");
	}

    return 0;
}
