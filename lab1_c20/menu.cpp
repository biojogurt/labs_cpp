#define NOMINMAX
#include "menu.h"
#include <iostream>
#include <string>
#include <windows.h>

int Menu::menu(int choicesAmount)
{
    std::wcout << L"\x1b[?25l\x1b[" + std::to_wstring(choicesAmount) + L"A";
    int position = 1;
    while (true)
    {
        INPUT_RECORD keys[128];
        DWORD keysRead;
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), keys, 128, &keysRead);
        for (DWORD i = 0; i < keysRead; i++)
        {
            switch (keys[i].Event.KeyEvent.wVirtualKeyCode)
            {
                case VK_DOWN:
                    if (keys[i].Event.KeyEvent.bKeyDown && position < choicesAmount)
                    {
                        std::wcout << " \r\x1b[B>\r";
                        position++;
                    }
                    break;
                case VK_UP:
                    if (keys[i].Event.KeyEvent.bKeyDown && position > 1)
                    {
                        std::wcout << " \r\x1b[A>\r";
                        position--;
                    }
                    break;
                case VK_RETURN:
                    if (keys[i].Event.KeyEvent.bKeyDown)
                    {
                        std::wcout << L"\x1b[" + std::to_wstring(choicesAmount + 1 - position) + L"B\x1b[?25h";
                        return position;
                    }
            }
        }
    }
}