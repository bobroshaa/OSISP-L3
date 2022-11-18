#include <windows.h>
#include <iostream>;

using namespace std;

#define NAME_LIB L"C:\\Users\\bobro\\source\\repos\\OSISP_3\\x64\\Debug\\MemoryDll.dll"
#define MEMORY_SEARCH "MemorySearch"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
		return DefWindowProc(hWnd, message, wParam, lParam);
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HMODULE hDLL = LoadLibrary(L"C:\\Users\\bobro\\source\\repos\\OSISP_3\\x64\\Debug\\MemoryDll.dll");
	if (!hDLL) {
		DWORD dw = GetLastError();
		return 1;
	}
	typedef void TMemorySearch(LPCWSTR programmName);
	TMemorySearch* memorySearch;
	memorySearch = (TMemorySearch*)GetProcAddress(hDLL, MEMORY_SEARCH);
	if (!memorySearch) {
		DWORD dw = GetLastError();
		return 1;
	}
	memorySearch(L"TextApp");

	return 0;
}


