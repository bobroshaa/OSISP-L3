#include <windows.h>
#include <iostream>;

using namespace std;

#define CHANGE_STRING "ChangeString"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int main()
{
	HMODULE hDLL = LoadLibrary(L"MemoryDll.dll");
	if (!hDLL) {
		DWORD dw = GetLastError();
		return 1;
	}
	typedef INT TChangeString(DWORD processId, const char* oldString, const char* newString);
	TChangeString* changeString;
	changeString = (TChangeString*)GetProcAddress(hDLL, CHANGE_STRING);
	if (!changeString) {
		DWORD dw = GetLastError();
		return 1;
	}

	string str = "I like puppeys";
	cout << str << "\n";
	Sleep(1000);
	cout << "Replacing..." << "\n";
	changeString(GetCurrentProcessId(), "puppeys", "kittens");
	Sleep(1000);
	cout << str;
	FreeLibrary(hDLL);

	return 0;
}