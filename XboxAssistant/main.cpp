// JoystickAssistant.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <string>
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "CWin32Utils/CXinputHelper.h"
#include "CWin32Utils/CInputUtils.h"
#include "CMainFrame.h"
#include "resource.h"

#ifdef _UNICODE
using _tstring = std::wstring;
#else
using _tstring = std::string;
#endif

void LogPrintf(LPCTSTR pFormat, ...);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    CMainFrame dlg;
    dlg.DoModalEx(IDD_DIALOG_MAIN, NULL, TRUE, true);

    return 0;
}

int main()
{
    CXInputHelper obj;
    obj.Initialize();

    obj.IgnoreLeftThumbEmpty(false);
    obj.IgnoreRightThumbEmpty(false);
    obj.SetThumbThreshold(4096);

    obj.SetSingleChangeCallback([](const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)-> void {
        LogPrintf(_T("Index: %d bRight: %d Trigger: %3d Thumb: %6d,%6d Button: %04X Press: %d Release: %d Holding: %d\r\n"),
        state.dwIndex,
        state.bRight, state.bTrigger, state.sThumbX, state.sThumbY, state.wButton, state.fDown, state.fUp, state.fHold);

        }
    );

    system("pause");

    return 0;
}

void LogPrintf(LPCTSTR pFormat, ...)
{
    size_t nCchCount = MAX_PATH;
    _tstring strResult(nCchCount, 0);
    va_list args;

    va_start(args, pFormat);

    do
    {
        //格式化输出字符串
        int nSize = _vsntprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
        if (-1 != nSize)
        {
            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
            ::WriteConsole(console, strResult.c_str(), nSize, NULL, NULL);
            break;
        }

        //缓冲大小超限终止
        if (nCchCount >= INT32_MAX)
        {
            break;
        }

        //重新分配缓冲
        nCchCount *= 2;
        strResult.resize(nCchCount);

    } while (true);

    va_end(args);
}