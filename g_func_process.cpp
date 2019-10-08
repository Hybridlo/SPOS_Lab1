#include <iostream>
#include "demofuncs"

using namespace std;

int main()
{
	int x;
	cin >> x;
	cout << spos::lab1::demo::g_func<spos::lab1::demo::DOUBLE>(x);
	return 0;
}