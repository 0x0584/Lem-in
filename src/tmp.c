
int netflow_send(t_netflow *net, int n_paths) {
	int ant;
	int i;
	int j;
	int count;
	bool flag = true;

	count = 0;
	ant = 0;
	while (flag) {
		flag = false;
		i = 0;
		while (ant < net->n_units && i < n_paths)
			net->paths[i++].ants[0] = ant++ + 1;
		i = 0;
		while (i < n_paths) {
			j = net->paths[i].size - 1;
			while (j) {
				if (net->paths[i].ants[j - 1] != -1)
					flag = true;
				net->paths[i].ants[j] = net->paths[i].ants[j - 1];
				j--;
			}
			net->paths[i].ants[0] = -1;
			i++;
		}
		count++;
	}

	return count;
}

int netflow_simulate(t_netflow *net) {
	int i = 0;
	int n_used = 0;
	int prev = INT_MAX;
	int tmp;

	while (n_used < net->n_paths) {
		tmp = netflow_send(net, n_used + 1);
		if (tmp > prev)
			break;
		prev = tmp;
		n_used++;
	}

	int sum;
	sum = 0;
	while (i < n_used)
	{
		net->paths[i].n_ants = (net->paths[n_used - 1].size - net->paths[i].size);
		sum += net->paths[i].n_ants;
		i++;
	}

	int mod = (net->n_units - sum) % n_used;
	int div = (net->n_units - sum) / n_used;

	i = 0;
	while (i < n_used)
	{
		net->paths[i].n_ants += div;
		if (mod--)
			net->paths[i].n_ants += 1;
		i++;
	}

	return n_used;
}

void		netflow_pushflow(t_netflow *net)
{
	int i;
	int j;
	int n_paths;
	bool flag;
	int ant;

	if (!net)
		return ;
	ant = 0;
	n_paths = netflow_simulate(net);
	flag = true;
	while (flag)
	{
		i = 0;
		flag = false;
		while (i < n_paths && ant < net->n_units) {
			if (net->paths[i].n_ants)
				net->paths[i].ants[0] = ++ant;
			i++;
		}
		i = 0;
		while (i < n_paths)
		{
			j = net->paths[i].size - 1;
			while (j >= 0) {
				if (net->paths[i].ants[j] != -1)
					ft_printf("L%d-%s ", net->paths[i].ants[j],
						   net->paths[i].nodes[j]);
				j--;
			}
			i++;
		}
		i = 0;
		while (i < n_paths)
		{
			j = net->paths[i].size - 1;
			while (j >= 0) {
				if (net->paths[i].ants[j] != -1)
					flag = true;
				if (j == 0)
					break;
				net->paths[i].ants[j] = net->paths[i].ants[j - 1];
				j--;
			}
			net->paths[i].ants[0] = -1;
			i++;
		}
		if (flag)
			ft_printf("\n");
	}
}
