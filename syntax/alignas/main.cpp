#include <iostream>

struct alignas(32) s32
{
	int val;
};

struct alignas(64) s64
{
	int val;
};

int main()
{
	std::ios::sync_with_stdio(false);

	std::cout << "alignment_of:" << std::endl;
	std::cout << "char: " << std::alignment_of<char>::value << std::endl;
	std::cout << "int:  " << std::alignment_of<int>::value << std::endl;
	std::cout << "s32:  " << std::alignment_of<s32>::value << std::endl;
	std::cout << "s64:  " << std::alignment_of<s64>::value << std::endl;

	return 0;
}
