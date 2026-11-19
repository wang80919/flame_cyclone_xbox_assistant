#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>

#ifdef _UNICODE 
using _tstring = std::wstring;
#else
using _tstring = std::string;
#endif

typedef struct _KBDATA_INFO KBDATA_INFO;

typedef struct _KBDATA_INFO {
    WORD vkCode;
    WORD scanCode;
    DWORD flags;

    _KBDATA_INFO() :vkCode(0), scanCode(0), flags(0) {};

    _KBDATA_INFO(WORD vk, WORD sc, DWORD f) : vkCode(vk), scanCode(sc), flags(f) {};

    _KBDATA_INFO(const _KBDATA_INFO& r) :vkCode(r.vkCode), scanCode(r.scanCode), flags(r.flags) {}

    bool operator == (const _KBDATA_INFO& r) const
    {
        return (this->scanCode == r.scanCode) && (this->flags & 0x01) == (r.flags & 0x01);
    }

    bool operator != (const _KBDATA_INFO& r) const
    {
        return !(*this == r);
    }

    bool operator < (const _KBDATA_INFO& r) const
    {
        if (this->scanCode != r.scanCode) return this->scanCode < r.scanCode;
        return (this->flags & 0x01) < (r.flags & 0x01);
    }

    void operator=(const _KBDATA_INFO& r)
    {
        this->vkCode = r.vkCode;
        this->scanCode = r.scanCode;
        this->flags = r.flags;
    }

} KBDATA_INFO, FAR* LPKBDATA_INFO, * PKBDATA_INFO;

// 鼠标按键按下标志
enum eInputMouseButton
{
    eMButton_Left = 0x01,
    eMButton_Right = 0x02,
    eMButton_Middle = 0x04,
    eMButton_XButton1 = 0x08,
    eMButton_XButton2 = 0x10,
};

class CInputUtils
{
public:

    //
    // @brief: 发送按键(通过虚键码) // https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes
    // @param: wVk          虚键码
    // @param: wScan        扫描码
    // @param: bExtended    是否扩展键
    // @param: bDown        是否要按下
    // @param: bUp          是否要弹起
    // @param: bScan        是否扫描码（启用后忽略虚键码）
    // @ret: bool           操作是否成功
    static bool KeyboardByVkCode(
        WORD wVk, 
        bool bExtended = false, 
        bool bDown = true, 
        bool bUp = true,
        WORD wScan = 0,
        bool bScan = false
    );

    //
    // @brief: 发送按键(通过键名)
    // @param: strKey       按键名
    // @param: bDown        是否要按下
    // @param: bUp          是否要弹起
    // @ret: bool           操作是否成功
    static bool KeyboardByName(
        const _tstring& strKey, 
        bool bDown = true, 
        bool bUp = true
    );

    //
    // @brief: 发送按键(USB键码)
    // @param: code         USB键码
    // @param: bDown        是否要按下
    // @param: bUp          是否要弹起
    // @ret: bool           操作是否成功
    static bool KeyboardByUsbKeyID(
        uint8_t code,
        bool bDown = true,
        bool bUp = true
    );

    //
    // @brief: 发送字符
    // @param: ch           字符
    // @ret: bool           操作是否成功
    static bool KeyboardChar(TCHAR ch);

    //
    // @brief: 发送字符串
    // @param: strText      字符
    // @ret: bool           操作是否成功
    static bool KeyboardText(_tstring strText);

    //
    // @brief: 鼠标移动
    // @param: x            横坐标
    // @param: y            纵坐标
    // @param: bAbsolute    是否绝对坐标, 如果为 true, 移动到指定坐标, 否则相对于当前坐标进行移动
    // @ret: bool           操作是否成功
    static bool MouseMove(
        LONG x, 
        LONG y, 
        bool bAbsolute = true
    );

    //
    // @brief: 鼠标按键
    // @param: eButton      鼠标按键
    // @param: bDown        按下按键
    // @param: bUp          弹起按键
    // @ret: bool           操作是否成功
    static bool MouseKey(
        eInputMouseButton eButton,
        bool bDown = true, 
        bool bUp = true
    );

    //
    // @brief: 鼠标滚轮
    // @param: nVRoll       垂直滚轮速度
    // @param: nHRoll       水平滚轮速度
    // @ret: bool           操作是否成功
    static bool MouseWheel(LONG nVRoll, LONG nHRoll = 0);

private:
    static std::map<_tstring, KBDATA_INFO> m_KeyMap;
    static std::map<DWORD, _tstring> m_UsbKeyMap;
};

