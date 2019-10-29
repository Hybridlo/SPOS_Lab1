#include <iostream>
#include "demofuncs"

using namespace std;

int main()
{
	int x;
	cin >> x;
	if (x == 1)
	{
		cin >> x;
		cout << spos::lab1::demo::f_func<spos::lab1::demo::DOUBLE>(x);
	}
	if (x == 2)
	{
		cin >> x;
		cout << spos::lab1::demo::g_func<spos::lab1::demo::DOUBLE>(x);
	}
	return 0;
}