#include <unistd.h>

int main()
{
    int i = 5;
    int i_old = 0;

	char array[] = "kkkk llll hhhh";

	array[0] = 'c';
	*(array + 6) = 'b';

    write(1, &str[i_old], i - i_old);
    write(1, str + i_old, i - i_old);

    return 0;
}
