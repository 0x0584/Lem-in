#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct path {
    int *nodes;
    int size;
} path;

int main(int argc, char *argv[]) {
    int ants = atoi(argv[1]);
    int npaths = (argc - 2);
    path *paths = malloc((argc - 2) * sizeof(path));

    for (int i = 2; i < argc; ++i) {
        paths[i - 2].size = atoi(argv[i]);
        paths[i - 2].nodes = malloc(paths[i - 2].size * sizeof(int));
        for (int j = 0; j < paths[i - 2].size; ++j)
            paths[i - 2].nodes[j] = -1;
    }

    int i = 0;
    bool flag = false;
    while (!flag) {
        flag = true;
        if (i < ants) {
            bool used = false;
            int tmp = 0;
            int tmp_size = paths[0].size;
            for (int j = 0; j < npaths; ++j) {
                if (tmp_size != paths[j].size) {
                    while (tmp-- && i < ants)
                        paths[tmp].nodes[0] = i++;
                    tmp = 0;
                }
                tmp_size = paths[j].size;
                if (i < ants && ants > paths[j].size) {
                    used = true;
                    tmp++;
                }
            }

            while (tmp-- && i < ants)
                paths[tmp].nodes[0] = i++;

            if (!used && i < ants) {
                for (int j = 0; j < npaths; ++j) {
                    if (paths[j].size == paths[0].size)
                        paths[0].nodes[0] = i++;
                }
            }
        }
        for (int j = 0; j < npaths; ++j) {
            for (int k = paths[j].size - 1; k; --k) {
                if (paths[j].nodes[k - 1] != -1)
                    printf("p%d[%d]=%d ", j, k - 1, paths[j].nodes[k - 1]);
                paths[j].nodes[k] = paths[j].nodes[k - 1];
                if (paths[j].nodes[k] != -1)
                    flag = false;
            }
            paths[j].nodes[0] = -1;
        }
        if (!flag)
            printf("\n");
    }

    return 0;
}
