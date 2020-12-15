int main(int argc, char *argv[])
{
    int n = 5;
	int ants = 30;
	int paths[] = {10, 11, 20, 21, 40};
	int path_ants[] = {0, 0, 0, 0, 0};


	int reset = ants / n;
	int i = 0;
	while (i < n) {
		path_ants[i] = reset;
		i++;
	}

	int mod = ants % n;

	while (mod) {
		i = 0;
		while (i < n && mod) {
			path_ants[i] += 1;
			mod--;
			i++;
		}
	}

    return 0;
}
