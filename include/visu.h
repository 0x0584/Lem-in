#ifndef VISU_H
#define VISU_H

#include <sys/ioctl.h>

#include "netflow.h"

#define DEFAULT_COLOR "%{reset}"
#define BORDER_COLOR "%{bold}%{magenta_fg}"
#define EMPTY_CELL_COLOR "%{blue_fg}"
#define UNIT_CELL_COLOR "%{yellow_fg}"

#define TOP_BAR BORDER_COLOR "-------" DEFAULT_COLOR
#define CORNER BORDER_COLOR "+" DEFAULT_COLOR
#define SIDE_BAR BORDER_COLOR " | " DEFAULT_COLOR
#define EMPTY_CELL EMPTY_CELL_COLOR " /// " DEFAULT_COLOR
#define UNIT_CELL SIDE_BAR UNIT_CELL_COLOR "%-4zu " DEFAULT_COLOR

void show(t_network net, size_t maxflow);

#endif /* VISU_H */
