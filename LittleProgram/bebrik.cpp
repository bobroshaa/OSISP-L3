#include <iostream>
#include <Windows.h>
using namespace std;

int main() {
	string str = "Hello little monkey!";
	while (true) {
		cout << str << '\n';
		Sleep(1000);
	}
}