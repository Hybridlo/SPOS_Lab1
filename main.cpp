#include <iostream>
#include <boost/process.hpp>
#include <boost/process/extend.hpp>
#include <boost/winapi/process.hpp>
#include <boost/detail/winapi/show_window.hpp>
#include <boost/asio.hpp>
#pragma warning(disable:4996)

using namespace std;
using namespace boost::process;

void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool IsConsoleVisible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}

void update_value(boost::asio::io_service* ios, char* readFrom, double* result)
{
	std::chrono::milliseconds time(10);
	if (strcmp(readFrom, ""))
	{
		*result = atof(readFrom);
		strcpy(readFrom, "");
	}
	ios->run_one_for(time);
}

bool check_escape(boost::asio::io_service* iosF, boost::asio::io_service* iosG, char* readFromF, char* readFromG, double* fResult, double* gResult, child* F, child *G)
{
	if (GetAsyncKeyState(VK_ESCAPE) == -32767)
	{
		auto set_own_window = [](auto& exec)
		{
			exec.creation_flags |=
				boost::detail::winapi::CREATE_NEW_CONSOLE_;
		};

		opstream sendToCancel;
		child cancel("cancel_process", boost::process::extend::on_setup = set_own_window);

		HideConsole();

		while (true)
		{
			Sleep(10);

			update_value(iosF, readFromF, fResult);
			update_value(iosG, readFromG, gResult);

			if (GetAsyncKeyState(0x59) == -32767 || !cancel.running())
			{

				ShowConsole();

				cout << "Abortion complete\n";

				if (*gResult != -1)
					cout << "G function computed, but F was not\n";

				if (*fResult != -1)
					cout << "F function computed, but G was not\n";

				TerminateProcess(cancel.native_handle(), 0);
				return true;
			}

			if (GetAsyncKeyState(0x4E) == -32767)
			{
				ShowConsole();
				TerminateProcess(cancel.native_handle(), 0);
				cout << "Abortion stopped by user\n";
				break;
			}

			if (*fResult == 0 || *gResult == 0 || (*fResult != -1 && *gResult != -1))
			{
				ShowConsole();
				TerminateProcess(cancel.native_handle(), 0);
				cout << "Abortion stopped because result has been computed\n";
				break;
			}
		}
	}

	return false;
}

void start()
{
	cout << "Enter the x value:";

	int x;

	cin >> x;

	boost::asio::io_service iosF, iosG;

	opstream sendToF, sendToG;
	char readFromF[4096] = "", readFromG[4096] = "";


	child F("func_process", std_out > boost::asio::buffer(readFromF), iosF, std_in < sendToF, std_err > stderr);
	child G("func_process", std_out > boost::asio::buffer(readFromG), iosG, std_in < sendToG, std_err > stderr);

	sendToF << 1 << endl << x << endl;
	sendToG << 2 << endl << x << endl;

	double fResult = -1, gResult = -1;
	int i = 0;

	cout << "Start computing functions, press ESC to abort\n";

	while (true)
	{
		update_value(&iosF, readFromF, &fResult);

		if (fResult == 0)
		{
			cout << "F equals to zero, computing result prematurely...\n";
			gResult = 1;
			break;
		}

		update_value(&iosG, readFromG, &gResult);

		if (gResult == 0)
		{
			cout << "G equals to zero, computing result prematurely...\n";
			fResult = 1;
			break;
		}

		if (gResult > -1 && fResult > -1)
		{
			cout << "Both functions computed, computing result...\n";
			cout << "F value: " << fResult << "\nG value: " << gResult << endl;
			break;
		}

		if (check_escape(&iosF, &iosG, readFromF, readFromG, &fResult, &gResult, &F, &G))
		{
			return;
		}
	}

	cout << "Computed result: " << (fResult < gResult ? fResult : gResult) << endl;
}

int main()
{
	while (true)
	{
		start();
		cout << endl;
	}
	
}
