#include <stdio.h>
#include <sys/ioctl.h>

typedef unsigned short ushort;

ushort xdim, ydim;

void set_window_bounds(ushort *x, ushort *y) {
    struct winsize ws;

    ioctl(1, TIOCGWINSZ, &ws);
    *x = ws.ws_row, *y = ws.ws_col;
}

void print_bar(size_t size) {
    for (size_t i = 0; i < size; ++i)
        printf("+-------");
    puts("+");
}

void print_cells(int path[], size_t size) {
    size_t i = 0;

    for (; i < size && path[i] < 0; ++i) {
        printf("| %-5d ", path[i]);
    }
    puts("|");
}

void print_path(int path[], size_t size) {
    print_bar(size);
    print_cells(path, size);
    print_bar(size);
}

int main(int argc, char *argv[]) {
    set_window_bounds(&xdim, &ydim);

    int path[] = {-1, -1, 20, 11, 5};

    print_path(path, sizeof path / sizeof path[0]);

    return 0;
}
