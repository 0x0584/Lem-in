#include <iostream>

int main(int argc, char *argv[])
{
  for (int i = 1; i <= 10; ++i) {
	for (int j = 1; j <= 10; ++j)
	  if (i != j)
		std::cout << i << "-" << j << std::endl;
  }
  return 0;
}
