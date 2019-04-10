#include <string_view>
#include <iostream>

void PrintSomething(const std::string_view & message)
{
	std::cout << message << std::endl;
}

int main()
{
	PrintSomething("Hallo world");
}
