// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

using namespace std;

 __declspec(dllexport) INT ChangeString(DWORD processId, const char* oldString, const char* newString);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        //ChangeString(GetCurrentProcessId(), "monkey", "bebrik");
        //break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


__declspec(dllexport) INT ChangeString(DWORD processId, const char* oldString, const char* newString) {

    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE 
        | PROCESS_VM_READ 
        | PROCESS_QUERY_INFORMATION, // уровень доступа к объекту процессу
        FALSE, // дескриптор наследования
        processId); // идентфикатор открытого процесса

    if (hProcess == NULL) {
        return 1;
    }
    
    // получаем данные о системы, в которых будут указатели на наибольший и наименьший адреса памяти
    // доступные для приложений
    SYSTEM_INFO msi;
    GetSystemInfo(&msi);

    char* currAddr = 0;
    MEMORY_BASIC_INFORMATION mbi;
    vector<char> buffer;

    size_t oldStrLength = strlen(oldString);
    size_t newStrLength = strlen(newString);

    while (currAddr < msi.lpMaximumApplicationAddress)
    {
        // получаем информацию о диапазоне страниц и заполняем структуру mbi
        if (!VirtualQueryEx(
            hProcess, // дескриптор процесса, информация о памяти которого запрашивается
            (void*)currAddr, // указатель на адрес памяти запрашвиаемых страниц
            &mbi, // указатель на структуру mbi
            sizeof(mbi))) // размер структуры
            break;

        if ((mbi.State == MEM_COMMIT)            //если регион в состоянии передачи памяти
            &&                                    //и
            (mbi.Protect != PAGE_READONLY)        //регион не только для чнения
            &&                                    //и
            (mbi.Protect != PAGE_EXECUTE_READ)    //регион не тролько с исполнением программного кода и чтением
            &&                                    //и
            (mbi.Protect != PAGE_GUARD)            //регион без сигнала доступа к странице
            &&                                    //и
            (mbi.Protect != PAGE_NOACCESS)        //регион не защищён доступом к нему
            )//то
        {
            currAddr = (char*)mbi.BaseAddress;
            buffer.resize(mbi.RegionSize);
            size_t nReadBytes;
            if (ReadProcessMemory(
                hProcess, // дескриптор процесса
                currAddr, // указатель на базовый адрес 
                &buffer[0], // указатель на буфер, в который поместится содержимое адресного пр-ва
                mbi.RegionSize, // количество байтов, которое необходимо прочитать
                &nReadBytes)) // указатель на переменную, в которую поместится количество прочитанных байт
            {
                for (size_t i = 0; i < (nReadBytes - oldStrLength); ++i) {
                    if (memcmp(oldString, &buffer[i], oldStrLength) == 0 && currAddr + i + newStrLength <= currAddr + mbi.RegionSize) {
                        char* ch = (char*)currAddr + i;
                        for (int j = 0; j < newStrLength; ++j) {
                            ch[j] = newString[j];
                        }
                    }
                }

            }
        }
        // некст регион
        currAddr += mbi.RegionSize;
    }
    //закрываем хэндл процесса
    CloseHandle(hProcess);
    return 0;
}
