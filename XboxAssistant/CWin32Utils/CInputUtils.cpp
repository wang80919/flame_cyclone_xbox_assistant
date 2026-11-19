#include "CInputUtils.h"
#include <iostream>

std::map<_tstring, KBDATA_INFO> CInputUtils::m_KeyMap =
{
    { _T("None"),           KBDATA_INFO(0x00, 0x00, 0)},
    { _T("Esc"),            KBDATA_INFO(0x1B, 0x01, 0)},
    { _T("F1"),             KBDATA_INFO(0x70, 0x3B, 0)},
    { _T("F2"),             KBDATA_INFO(0x71, 0x3C, 0)},
    { _T("F3"),             KBDATA_INFO(0x72, 0x3D, 0)},
    { _T("F4"),             KBDATA_INFO(0x73, 0x3E, 0)},
    { _T("F5"),             KBDATA_INFO(0x74, 0x3F, 0)},
    { _T("F6"),             KBDATA_INFO(0x75, 0x40, 0)},
    { _T("F7"),             KBDATA_INFO(0x76, 0x41, 0)},
    { _T("F8"),             KBDATA_INFO(0x77, 0x42, 0)},
    { _T("F9"),             KBDATA_INFO(0x78, 0x43, 0)},
    { _T("F10"),            KBDATA_INFO(0x79, 0x44, 0)},
    { _T("F11"),            KBDATA_INFO(0x7A, 0x57, 0)},
    { _T("F12"),            KBDATA_INFO(0x7B, 0x58, 0)},
    { _T("F15"),            KBDATA_INFO(0x7E, 0x66, 0)},
    { _T("F16"),            KBDATA_INFO(0x7F, 0x67, 0)},
    { _T("F17"),            KBDATA_INFO(0x80, 0x68, 0)},
                            
    { _T("`"),              KBDATA_INFO(0xC0, 0x29, 0)},
    { _T("1"),              KBDATA_INFO(0x31, 0x02, 0)},
    { _T("2"),              KBDATA_INFO(0x32, 0x03, 0)},
    { _T("3"),              KBDATA_INFO(0x33, 0x04, 0)},
    { _T("4"),              KBDATA_INFO(0x34, 0x05, 0)},
    { _T("5"),              KBDATA_INFO(0x35, 0x06, 0)},
    { _T("6"),              KBDATA_INFO(0x36, 0x07, 0)},
    { _T("7"),              KBDATA_INFO(0x37, 0x08, 0)},
    { _T("8"),              KBDATA_INFO(0x38, 0x09, 0)},
    { _T("9"),              KBDATA_INFO(0x39, 0x0A, 0)},
    { _T("0"),              KBDATA_INFO(0x30, 0x0B, 0)},
    { _T("-"),              KBDATA_INFO(0xBD, 0x0C, 0)},
    { _T("="),              KBDATA_INFO(0xBB, 0x0D, 0)},
    { _T("Backspace"),      KBDATA_INFO(0x08, 0x0E, 0)},

    { _T("LCtrl"),          KBDATA_INFO(0xA2, 0x1D, 0)},
    { _T("RCtrl"),          KBDATA_INFO(0xA3, 0x1D, 1)},
    { _T("LAlt"),           KBDATA_INFO(0xA4, 0x38, 0)},
    { _T("RAlt"),           KBDATA_INFO(0xA5, 0x38, 1)},
    { _T("LWin"),           KBDATA_INFO(0x5B, 0x5B, 1)},
    { _T("RWin"),           KBDATA_INFO(0x5C, 0x5C, 1)},
    { _T("LShift"),         KBDATA_INFO(0xA0, 0x2A, 0)},
    { _T("RShift"),         KBDATA_INFO(0xA1, 0x36, 1)},

    { _T("Application"),    KBDATA_INFO(0x5D, 0x5D, 1)},

    { _T("Tab"),            KBDATA_INFO(0x09, 0x0F, 0)},
    { _T("Space"),          KBDATA_INFO(0x20, 0x39, 0)},
    { _T("Enter"),          KBDATA_INFO(0x0D, 0x1C, 0)},

    { _T("["),              KBDATA_INFO(0xDB, 0x1A, 0)},
    { _T("]"),              KBDATA_INFO(0xDD, 0x1B, 0)},
    { _T("\\"),             KBDATA_INFO(0xDC, 0x2B, 0)},

    { _T(";"),              KBDATA_INFO(0xBA, 0x27, 0)},
    { _T("\'"),             KBDATA_INFO(0xDE, 0x28, 0)},

    { _T(","),              KBDATA_INFO(0xBC, 0x33, 0)},
    { _T("."),              KBDATA_INFO(0xBE, 0x34, 0)},
    { _T("/"),              KBDATA_INFO(0xBF, 0x35, 0)},

    { _T("A"),              KBDATA_INFO('A', 0x1E, 0)},
    { _T("B"),              KBDATA_INFO('B', 0x30, 0)},
    { _T("C"),              KBDATA_INFO('C', 0x2E, 0)},
    { _T("D"),              KBDATA_INFO('D', 0x20, 0)},
    { _T("E"),              KBDATA_INFO('E', 0x12, 0)},
    { _T("F"),              KBDATA_INFO('F', 0x21, 0)},
    { _T("G"),              KBDATA_INFO('G', 0x22, 0)},
    { _T("H"),              KBDATA_INFO('H', 0x23, 0)},
    { _T("I"),              KBDATA_INFO('I', 0x17, 0)},
    { _T("J"),              KBDATA_INFO('J', 0x24, 0)},
    { _T("K"),              KBDATA_INFO('K', 0x25, 0)},
    { _T("L"),              KBDATA_INFO('L', 0x26, 0)},
    { _T("M"),              KBDATA_INFO('M', 0x32, 0)},
    { _T("N"),              KBDATA_INFO('N', 0x31, 0)},
    { _T("O"),              KBDATA_INFO('O', 0x18, 0)},
    { _T("P"),              KBDATA_INFO('P', 0x19, 0)},
    { _T("Q"),              KBDATA_INFO('Q', 0x10, 0)},
    { _T("R"),              KBDATA_INFO('R', 0x13, 0)},
    { _T("S"),              KBDATA_INFO('S', 0x1F, 0)},
    { _T("T"),              KBDATA_INFO('T', 0x14, 0)},
    { _T("U"),              KBDATA_INFO('U', 0x16, 0)},
    { _T("V"),              KBDATA_INFO('V', 0x2F, 0)},
    { _T("W"),              KBDATA_INFO('W', 0x11, 0)},
    { _T("X"),              KBDATA_INFO('X', 0x2D, 0)},
    { _T("Y"),              KBDATA_INFO('Y', 0x15, 0)},
    { _T("Z"),              KBDATA_INFO('Z', 0x2C, 0)},

    { _T("Up"),             KBDATA_INFO(0x26, 0x48, 1)},
    { _T("Down"),           KBDATA_INFO(0x28, 0x50, 1)},
    { _T("Left"),           KBDATA_INFO(0x25, 0x4B, 1)},
    { _T("Right"),          KBDATA_INFO(0x27, 0x4D, 1)},

    { _T("UpArrow"),        KBDATA_INFO(0x26, 0x48, 1)},
    { _T("DownArrow"),      KBDATA_INFO(0x28, 0x50, 1)},
    { _T("LeftArrow"),      KBDATA_INFO(0x25, 0x4B, 1)},
    { _T("RightArrow"),     KBDATA_INFO(0x27, 0x4D, 1)},

    { _T("Num Up"),         KBDATA_INFO(0x26, 0x48, 0)},
    { _T("Num Down"),       KBDATA_INFO(0x28, 0x50, 0)},
    { _T("Num Left"),       KBDATA_INFO(0x25, 0x4B, 0)},
    { _T("Num Right"),      KBDATA_INFO(0x27, 0x4D, 0)},

    { _T("Insert"),         KBDATA_INFO(0x2d, 0x52, 1)},
    { _T("Home"),           KBDATA_INFO(0x24, 0x47, 1)},
    { _T("Page Up"),        KBDATA_INFO(0x21, 0x49, 1)},
    { _T("Delete"),         KBDATA_INFO(0x2E, 0x53, 1) },
    { _T("End"),            KBDATA_INFO(0x23, 0x4F, 1)},
    { _T("Page Down"),      KBDATA_INFO(0x22, 0x51, 1)},

    { _T("Prnt Scrn"),      KBDATA_INFO(0x2C, 0x37, 1)},
    { _T("PrintScreen"),      KBDATA_INFO(0x2C, 0x37, 1) },
    
    { _T("Scroll Lock"),    KBDATA_INFO(0x91, 0x46, 0)},
    { _T("Num Lock"),       KBDATA_INFO(0x90, 0x45, 1)},
    { _T("Caps Lock"),      KBDATA_INFO(0x14, 0x3A, 0) },
    { _T("Pause"),          KBDATA_INFO(0x13, 0x45, 1)},

    { _T("Num 0"),          KBDATA_INFO(0x60, 0x52, 0) },
    { _T("Num 1"),          KBDATA_INFO(0x61, 0x4F, 0) },
    { _T("Num 2"),          KBDATA_INFO(0x62, 0x50, 0) },
    { _T("Num 3"),          KBDATA_INFO(0x63, 0x51, 0) },
    { _T("Num 4"),          KBDATA_INFO(0x64, 0x4B, 0) },
    { _T("Num 5"),          KBDATA_INFO(0x65, 0x4C, 0) },
    { _T("Num 6"),          KBDATA_INFO(0x66, 0x4D, 0) },
    { _T("Num 7"),          KBDATA_INFO(0x67, 0x47, 0) },
    { _T("Num 8"),          KBDATA_INFO(0x68, 0x48, 0) },
    { _T("Num 9"),          KBDATA_INFO(0x69, 0x49, 0) },

    { _T("Num Home"),       KBDATA_INFO(0x24, 0x47, 0) },
    { _T("Num End"),        KBDATA_INFO(0x23, 0x4F, 0) },
    { _T("Num Page Up"),    KBDATA_INFO(0x21, 0x49, 0) },
    { _T("Num Page Down"),  KBDATA_INFO(0x22, 0x51, 0) },

    { _T("Clear"),          KBDATA_INFO(0x0C, 0x4C, 0) },

    { _T("Num /"),          KBDATA_INFO(0x6f, 0x35, 1) },
    { _T("Num *"),          KBDATA_INFO(0x6A, 0x37, 0) },
    { _T("Num -"),          KBDATA_INFO(0x6D, 0x4A, 0) },
    { _T("Num +"),          KBDATA_INFO(0x6B, 0x4E, 0) },
    { _T("Num Enter"),      KBDATA_INFO(0x0D, 0x1C, 1) },
    { _T("Num Del"),        KBDATA_INFO(0x2E, 0x53, 0) },
    { _T("Num Insert"),     KBDATA_INFO(0x2D, 0x52, 0) },
    { _T("Num ."),          KBDATA_INFO(0x6E, 0x53, 0) },

};  //118个按键

//USB按键映射表
std::map<DWORD, _tstring> CInputUtils::m_UsbKeyMap =
{
    { 0x04, _T("A")},
    { 0x05, _T("B")},
    { 0x06, _T("C")},
    { 0x07, _T("D")},
    { 0x08, _T("E")},
    { 0x09, _T("F")},
    { 0x0A, _T("G")},
    { 0x0B, _T("H")},
    { 0x0C, _T("I")},
    { 0x0D, _T("J")},
    { 0x0E, _T("K")},
    { 0x0F, _T("L")},
    { 0x10, _T("M")},
    { 0x11, _T("N")},
    { 0x12, _T("O")},
    { 0x13, _T("P")},
    { 0x14, _T("Q")},
    { 0x15, _T("R")},
    { 0x16, _T("S")},
    { 0x17, _T("T")},
    { 0x18, _T("U")},
    { 0x19, _T("V")},
    { 0x1A, _T("W")},
    { 0x1B, _T("X")},
    { 0x1C, _T("Y")},
    { 0x1D, _T("Z")},
    { 0x1E, _T("1")},            // !
    { 0x1F, _T("2")},            // @
    { 0x20, _T("3")},            // #
    { 0x21, _T("4")},            // $
    { 0x22, _T("5")},            // %
    { 0x23, _T("6")},            // ^
    { 0x24, _T("7")},            // &
    { 0x25, _T("8")},            // *
    { 0x26, _T("9")},            // (
    { 0x27, _T("0")},            // )
    { 0x28, _T("Enter")},
    { 0x29, _T("Esc")},
    { 0x2A, _T("Backspace")},
    { 0x2B, _T("Tab")},
    { 0x2C, _T("Space")},
    { 0x2D, _T("-")},            //_
    { 0x2E, _T("=")},            //+
    { 0x2F, _T("[")},            //{
    { 0x30, _T("]")},            //}
    { 0x31, _T("\\")},           //|
    { 0x33, _T(";")},            //:
    { 0x34, _T("\'")},           //"
    { 0x35, _T("`")},            //~
    { 0x36, _T(",")},            //<
    { 0x37, _T(".")},            //>
    { 0x38, _T("/")},            //?
    { 0x39, _T("Caps Lock")},    
    { 0x3A, _T("F1")},
    { 0x3B, _T("F2")},
    { 0x3C, _T("F3")},
    { 0x3D, _T("F4")},
    { 0x3E, _T("F5")},
    { 0x3F, _T("F6")},
    { 0x40, _T("F7")},
    { 0x41, _T("F8")},
    { 0x42, _T("F9")},
    { 0x43, _T("F10")},
    { 0x44, _T("F11")},
    { 0x45, _T("F12")},
    { 0x46, _T("PrintScreen")},  // PrintScreen
    { 0x47, _T("Scroll Lock")},  // 
    { 0x48, _T("Pause")},        //
    { 0x49, _T("Insert")},
    { 0x4A, _T("Home")},
    { 0x4B, _T("Page Up")},
    { 0x4C, _T("Delete")},
    { 0x4D, _T("End")},
    { 0x4E, _T("Page Down")},
    { 0x4F, _T("RightArrow")},
    { 0x50, _T("LeftArrow")},
    { 0x51, _T("DownArrow")},
    { 0x52, _T("UpArrow")},
    { 0x53, _T("Num Lock")},     //Clear
    { 0x54, _T("Num /")},
    { 0x55, _T("Num *")},
    { 0x56, _T("Num -")},
    { 0x57, _T("Num +")},
    { 0x58, _T("Num Enter")},
    { 0x59, _T("Num 1")},        //End
    { 0x5A, _T("Num 2")},        //Down Arrow
    { 0x5B, _T("Num 3")},        //PageDn
    { 0x5C, _T("Num 4")},        //Left Arrow
    { 0x5D, _T("Num 5")},
    { 0x5E, _T("Num 6")},        //Right Arrow
    { 0x5F, _T("Num 7")},        //Home
    { 0x60, _T("Num 8")},        //Up Arrow
    { 0x61, _T("Num 9")},        //Page Up
    { 0x62, _T("Num 0")},        //Insert
    { 0x63, _T("Num .")},        //Delete
    { 0x64, _T("\\(Non-US)")},
    { 0x65, _T("Application")},

    { 0x66, _T("Power")},
    { 0x67, _T("=") },

    { 0x68, _T("F13")}, 
    { 0x69, _T("F14")}, 
    { 0x6A, _T("F15")}, 
    { 0x6B, _T("F16")}, 
    { 0x6C, _T("F17")},
    { 0x6D, _T("F18")}, 
    { 0x6E, _T("F19")}, 
    { 0x6F, _T("F20")}, 
    { 0x70, _T("F21")}, 
    { 0x71, _T("F22")}, 
    { 0x72, _T("F23")},
    { 0x73, _T("F24")},
};

bool CInputUtils::KeyboardByVkCode(
    WORD wVk, 
    bool bExtended/* = false*/, 
    bool bDown/* = true*/, 
    bool bUp/* = true*/,
    WORD wScan/* = 0*/,
    bool bScan/* = false*/
)
{
    INPUT inputs[0x02] = { 0 };
    UINT uSnedCount = 0;
    DWORD dwFlag = 0;

    if (bExtended) dwFlag |= KEYEVENTF_EXTENDEDKEY;
    if (bScan) dwFlag |= KEYEVENTF_SCANCODE;

    if (bDown)
    {
        inputs[uSnedCount].type = INPUT_KEYBOARD;
        inputs[uSnedCount].ki.wVk = wVk;
        inputs[uSnedCount].ki.wScan = wScan;
        inputs[uSnedCount].ki.dwFlags = dwFlag;
        uSnedCount++;
    }

    if (bUp)
    {
        inputs[uSnedCount].type = INPUT_KEYBOARD;
        inputs[uSnedCount].ki.wVk = wVk;
        inputs[uSnedCount].ki.wScan = wScan;
        inputs[uSnedCount].ki.dwFlags = dwFlag | KEYEVENTF_KEYUP;
        uSnedCount++;
    }

    UINT uCount = ::SendInput(uSnedCount, inputs, sizeof(INPUT));

    return uSnedCount == uCount;
}

bool CInputUtils::KeyboardByName(const _tstring& strKey, bool bDown/* = true*/, bool bUp/* = true*/)
{
    bool bFind = false;
    KBDATA_INFO keyData;

    auto itFind = m_KeyMap.find(strKey);

    if (m_KeyMap.end() == itFind)
    {
        return false;
    }

    keyData = itFind->second;

    if(0 == keyData.vkCode)
    {
        return false;
    }

    INPUT inputs[0x02] = { 0 };
    UINT uSendCount = 0;

    if (bDown)
    {
        inputs[uSendCount].type = INPUT_KEYBOARD;
        inputs[uSendCount].ki.wVk = keyData.vkCode;
        inputs[uSendCount].ki.wScan = keyData.scanCode;
        inputs[uSendCount].ki.dwFlags = keyData.flags;
        //inputs[uSendCount].ki.time = ::clock();
        uSendCount++;
    }

    if (bUp)
    {
        inputs[uSendCount].type = INPUT_KEYBOARD;
        inputs[uSendCount].ki.wVk = keyData.vkCode;
        inputs[uSendCount].ki.wScan = keyData.scanCode;
        inputs[uSendCount].ki.dwFlags = keyData.flags | KEYEVENTF_KEYUP;
        //inputs[uSendCount].ki.time = ::clock();
        uSendCount++;
    }

    UINT uCount = ::SendInput(uSendCount, inputs, sizeof(INPUT));

    return uSendCount == uCount;
}

bool CInputUtils::KeyboardByUsbKeyID(
    uint8_t code,
    bool bDown/* = true*/,
    bool bUp/* = true*/
)
{
    auto itKey = m_UsbKeyMap.find(code);
    if (itKey != m_UsbKeyMap.end())
    {
        return KeyboardByName(itKey->second, bDown, bUp);
    }

    return false;
}

bool CInputUtils::KeyboardChar(TCHAR ch)
{
    INPUT inputs[0x02] = { 0 };

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0;
    inputs[0].ki.wScan = ch;
    inputs[0].ki.dwFlags = KEYEVENTF_UNICODE;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0;
    inputs[1].ki.wScan = ch;
    inputs[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

    UINT uCount = ::SendInput(_countof(inputs), inputs, sizeof(INPUT));

    return _countof(inputs) == uCount;
}

bool CInputUtils::KeyboardText(_tstring strText)
{
    INPUT inputs[0x02] = { 0 };
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0;
    inputs[0].ki.dwFlags = KEYEVENTF_UNICODE;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0;
    inputs[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

    int uCount = 0;
    for (const auto& item : strText)
    {
        inputs[0].ki.wScan = item;
        inputs[1].ki.wScan = item;
        uCount = ::SendInput(_countof(inputs), inputs, sizeof(INPUT));
    }

    return _countof(inputs) == uCount;
}

bool CInputUtils::MouseMove(LONG x, LONG y, bool bAbsolute)
{
    if (0 == x && 0 == y)
    {
        return true;
    }

    LONG dwWidth = ::GetSystemMetrics(SM_CXSCREEN);
    LONG dwHeight = ::GetSystemMetrics(SM_CYSCREEN);

    INPUT inputs[0x01] = { 0 };
    inputs[0].type = INPUT_MOUSE;

    if (bAbsolute)
    {
        inputs[0].mi.dx = (LONG)((double)65535 / (double)dwWidth * x);
        inputs[0].mi.dy = (LONG)((double)65535 / (double)dwHeight * y);
    }
    else
    {
        inputs[0].mi.dx = x;
        inputs[0].mi.dy = y;
    }

    inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;
    if (bAbsolute)
    {
        inputs[0].mi.dwFlags |= MOUSEEVENTF_ABSOLUTE;
    }

    UINT uCount = ::SendInput(_countof(inputs), inputs, sizeof(INPUT));

    return _countof(inputs) == uCount;
}

bool CInputUtils::MouseKey(
    eInputMouseButton eButton, 
    bool bDown/* = true*/, 
    bool bUp/* = true*/)
{
    INPUT inputs[0x02] = { 0 };
    UINT uSendCount = 0;

    if (bDown)
    {
        DWORD dwFlags = 0;
        if (eButton & eInputMouseButton::eMButton_Left)
        {
            dwFlags |= MOUSEEVENTF_LEFTDOWN;
        }

        if (eButton & eInputMouseButton::eMButton_Right)
        {
            dwFlags |= MOUSEEVENTF_RIGHTDOWN;
        }

        if (eButton & eInputMouseButton::eMButton_Middle)
        {
            dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
        }

        if (eButton & eInputMouseButton::eMButton_XButton1)
        {
            dwFlags |= MOUSEEVENTF_XDOWN;
        }

        inputs[uSendCount].type = INPUT_MOUSE;
        inputs[uSendCount].mi.dwFlags = dwFlags;
        uSendCount++;
    }

    if (bUp)
    {
        DWORD dwFlags = 0;
        if (eButton & eInputMouseButton::eMButton_Left)
        {
            dwFlags |= MOUSEEVENTF_LEFTUP;
        }

        if (eButton & eInputMouseButton::eMButton_Right)
        {
            dwFlags |= MOUSEEVENTF_RIGHTUP;
        }

        if (eButton & eInputMouseButton::eMButton_Middle)
        {
            dwFlags |= MOUSEEVENTF_MIDDLEUP;
        }

        if (eButton & eInputMouseButton::eMButton_XButton1)
        {
            dwFlags |= MOUSEEVENTF_XUP;
        }

        inputs[uSendCount].type = INPUT_MOUSE;
        inputs[uSendCount].mi.dwFlags = dwFlags;
        uSendCount++;
    }

    UINT uCount = ::SendInput(uSendCount, inputs, sizeof(INPUT));

    return uSendCount == uCount;
}

bool CInputUtils::MouseWheel(LONG nVRoll, LONG nHRoll/* = 0*/)
{
    if (0 == nVRoll && 0 == nHRoll)
    {
        return true;
    }

    INPUT inputs[0x02] = { 0 };

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_WHEEL;
    inputs[0].mi.mouseData = (WHEEL_DELTA * nVRoll);

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_HWHEEL;
    inputs[1].mi.mouseData = (WHEEL_DELTA * nHRoll);

    UINT uCount = ::SendInput(_countof(inputs), inputs, sizeof(INPUT));

    return _countof(inputs) == uCount;
}
