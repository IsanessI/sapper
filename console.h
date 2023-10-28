#pragma once
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <ctime>

//TODO buffer to out figures
//TODO con.print()   con.print(raytracing& rtx)
class console
{
private:
	HANDLE handle;
	HWND hwnd;
    COORD wnd_size; //size of window in cells(?)
    const COORD fnt_size = { 8, 16 };
    COORD pxl_size; //size of window in pixels
    COORD fls_size; //size of fullscreen resolution
public:
    bool game_over;
    //colors
    enum class colors
    {
        Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
        Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
        Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
        White = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        Gray = FOREGROUND_INTENSITY,
        Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
        Magenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
        Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
        DarkRed = FOREGROUND_RED,
        DarkBlue = FOREGROUND_BLUE,
        DarkMagenta = FOREGROUND_RED | FOREGROUND_BLUE,
        DarkYellow = FOREGROUND_RED | FOREGROUND_GREEN,
        DarkGreen = FOREGROUND_GREEN,
        Black = 0
    };
    
    //constructions
    console()
    {
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
        char* Title[1024] = {'\0'};
        GetConsoleTitle((LPWSTR)Title, 1024);
        hwnd = FindWindow(NULL, (LPWSTR)Title); //set handle && hwnd

        setlocale(LC_ALL, "RUSSIAN");
        game_over = 0;
        
        fls_size.X = GetSystemMetrics(SM_CXSCREEN); //set fullscreen size
        fls_size.Y = GetSystemMetrics(SM_CYSCREEN);

        change_size_of_window(fls_size.X/2, fls_size.Y/2, true);
        CONSOLE_FONT_INFOEX font_info;
        GetCurrentConsoleFontEx(handle, true, &font_info);
        font_info.dwFontSize = fnt_size;
        SetCurrentConsoleFontEx(handle, true, &font_info);
    }
    console(const console& con) = delete;
    console& operator=(const console& con) = delete;
    console(console&& con) = delete;
    console& operator=(console&& con) = delete;
    
    //destruction
    ~console()
    {
        handle = NULL;
        hwnd = NULL;
    }

    //functions
    void change_size_of_window(int x_size, int y_size, bool SetBuffer = 0)
    {
        MoveWindow(hwnd, (fls_size.X-x_size)/2, (fls_size.Y-y_size)/2, x_size, y_size, true);
        pxl_size = { (short)x_size, (short)y_size };
        get_size();

        if (SetBuffer)
        {
            COORD buf = wnd_size;
            buf.Y = 1000;
            SetConsoleScreenBufferSize(handle, buf);
        }
        else
            SetConsoleScreenBufferSize(handle, wnd_size);
    } 
    void get_size()
    {
        CONSOLE_SCREEN_BUFFER_INFO console_info;
        GetConsoleScreenBufferInfo(handle, &console_info);

        wnd_size.X = console_info.srWindow.Right - console_info.srWindow.Left + 1;
        wnd_size.Y = console_info.srWindow.Bottom - console_info.srWindow.Top + 1;
    }
    void change_color(colors Color)
    {
        SetConsoleTextAttribute(handle, (int)Color);
    }
    void set_position(COORD coord)
    {
        SetConsoleCursorPosition(handle, coord);
    }

    template <typename T>
    console& operator>>(T& obj)
    {
        std::cin >> obj;
        
        return *this;
    }
    
    template <typename T>
    console& operator<<(const T& obj)
    {
        std::cout << obj;

        return *this;
    }

    void sleep(int time)
    {
        Sleep(time);
    }
    char getch()
    {
        char symbol = _getch();
        return symbol;
    }
    void clear()
    {
        system("cls");
    }
    void fullscreen(bool SetBuffer = 0)
    {
        MoveWindow(hwnd, 0, 0, fls_size.X, fls_size.Y, true);

        change_size_of_window(fls_size.X, fls_size.Y, SetBuffer);
        SendMessage(hwnd, WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
    }
    void windowscreen(bool SetBuffer = 0)
    {
        SendMessage(hwnd, WM_SYSKEYUP, VK_RETURN, 0x20000000);
        change_size_of_window(pxl_size.X, pxl_size.Y, SetBuffer);
    }
    COORD& pixel_size()
    {
        return pxl_size;
    }
    COORD& window_size()
    {
        return wnd_size;
    }
    COORD font_size()
    {
        return fnt_size;
    }
    operator std::istream& ()
    {
        return std::cin;
    }
    operator std::ostream& ()
    {
        return std::cout;
    }
};

