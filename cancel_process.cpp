#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

int main()
{
	string input;
	cout << "Abortion command issued, press \"Y\" to confirm or \"N\" to continue computation\n";
	int i = 15;
	while (true)
	{
		cout << "\r" << i << " seconds left before termination...  " << flush;
		if (i == 0)
			break;
		i--;
		Sleep(1000);
	}

	return 0;
}