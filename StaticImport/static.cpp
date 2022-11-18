#include <windows.h>
#include <iostream>;
using namespace std;

__declspec(dllimport) INT ChangeString(DWORD processId, const char* oldString, const char* newString);

int main()
{
	string str = "I like puppeys";
	cout << str << "\n";
	Sleep(1000);
	cout << "Replacing..." << "\n";
	ChangeString(GetCurrentProcessId(), "puppeys", "kittens");
	Sleep(1000);
	cout << str;

	return 0;
}


