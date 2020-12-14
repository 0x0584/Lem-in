#include "visu.h"

static size_t last_flow;

static bool visualize = true;
static bool output_maxflow = true;
static bool output_flow_info = true;

static unsigned short xdim;
static unsigned short ydim;

static void set_window_bounds(unsigned short *x, unsigned short *y) {
    struct winsize ws;

    ioctl(1, TIOCGWINSZ, &ws);
    *x = ws.ws_row, *y = ws.ws_col;
}

static void print_flow_info(void *pflow) {
    t_flow flow;
    size_t i;

    if (!(flow = pflow))
        return;
    ft_printf("n edges: %zu ", flow->size);
    ft_printf("total units: %zu\n", flow->total_units);
    i = 0;
    while (i < flow->size) {
        ft_printf(" | %{magenta_fg}[%d]%{italic}%5s%{reset} ", i,
                  flow->stage[i].vertex);
        i++;
    }
    ft_putendl(" |");
}

static void flow_print(void *pflow) {
    size_t i;
    t_flow flow;

    if (!(flow = pflow))
        return;
    i = 0;
    while (i < flow->size) {
        if (flow->stage[i].unit != NIL_ANT)
            ft_printf(" | %{magenta_fg}%5s: %{italic}%{yellow_fg}%4zu%{reset}",
                      flow->stage[i].vertex, flow->stage[i].unit);
        else
            ft_printf(" | %{bold} *no ant*  %{reset}");
        i++;
    }
    ft_putendl(" |");
}

static void print_top_bar(size_t size) {
    size_t i;

    i = 0;
    ft_putstr("  ");
    while (i < size)
        ft_printf(CORNER TOP_BAR), i++;
    ft_printf(CORNER "\n ");
}

static void flow_ascii(void *pflow) {
    size_t i;
    t_flow flow;

    if (!(flow = pflow))
        return;
    print_top_bar(flow->size);
    i = 0;
    while (i < flow->size) {
        if (flow->stage[i].unit != NIL_ANT)
            ft_printf(UNIT_CELL, flow->stage[i].unit);
        else
            ft_printf(SIDE_BAR EMPTY_CELL);
        i++;
    }
    ft_printf(SIDE_BAR "\n");
    last_flow = flow->size;
}

static void print_flows_ascii(t_lst flows) {
    ft_putendl("");
    lst_iter(flows, true, flow_ascii);
    print_top_bar(last_flow);
}

static void flow_out(t_flow flow) {
    size_t i;

    i = flow->size;
    while (i--)
        if (flow->stage[i].unit != NIL_ANT)
            ft_printf("L%zu-%s ", flow->stage[i].unit, flow->stage[i].vertex);
}

static void print_flow_lines(t_network net) {
    t_lstnode walk;

    walk = lst_front(net->flows);
    while (walk) {
        flow_out(walk->blob);
        lst_node_forward(&walk);
    }
}

void show(t_network net, size_t maxflow) {
    if (visualize)
        system("clear");
    if (output_maxflow) {
        ft_printf("\n  %{underline}%{italic}maxflow is%{reset}%{underline}");
        if (maxflow == lst_size(net->flows))
            ft_printf(" %{green_fg}%zu%{reset}", maxflow);
        else if (maxflow >= lst_size(net->flows) / 2)
            ft_printf(" %{yellow_fg}%zu%{reset}", maxflow);
        else
            ft_printf(" %{red_fg}%zu%{reset}", maxflow);
    }
    if (output_flow_info) {
        /* ft_putendl(""); */
        /* ft_printf("n_flows: %zu\n", lst_size(net->flows)); */
        /* ft_printf("n_units: %zu\n", net->n_units); */
        /* lst_iter(net->flows, true, print_flow_info); */

        ft_putendl("\n");
        lst_iter(net->flows, true, flow_print);
    }
    if (visualize) {
        print_flows_ascii(net->flows);
        system("sleep 1");
    }
}
