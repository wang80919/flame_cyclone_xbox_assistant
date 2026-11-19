#pragma once
#include <windows.h>
#include <xinput.h>
#include <tchar.h>
#include <thread>
#include <functional>
#include <string>
#pragma comment(lib, "xinput.lib")

#define XINPUT_GAMEPAD_GUIDE        (0x0400)

#ifdef _UNICODE
using _tstring = std::wstring;
#else
using _tstring = std::string;
#endif

// 单个变化状态
typedef struct _INPUT_EVENT_STATE
{
    DWORD dwIndex;      // 当前手柄索引
    WORD wButton;       // 当前触发的单个按键
    SHORT sThumbX;      // 当前摇杆X值
    SHORT sThumbY;      // 当前摇杆Y值
    BYTE bTrigger;      // 扳机数据
    bool bRight;        // 是否为右摇杆/扳机(false: 左, true: 右)
    bool fDown;         // 是否触发按钮按下(从未按下 -> 按下)
    bool fUp;           // 是否触发按钮释放(从按下 -> 未按下)
    bool fHold;         // 按住触发按钮按住(从按下 -> 按下)

    DWORD dwTime;       //触发时间
    DWORD dwDuration;   //持续时长

    _INPUT_EVENT_STATE()
    {
        memset(this, 0, sizeof(*this));
    }

}INPUT_EVENT_STATE;

class CXInputHelper
{
public:

    CXInputHelper();
    virtual ~CXInputHelper();

    bool Initialize();

    void Uninitialize();

    void IgnoreLeftThumbEmpty(
        bool bEnable = true                     // 允许空数据触发回调
    );

    void IgnoreRightThumbEmpty(
        bool bEnable = true                     // 允许空数据触发回调
    );

    void SetInterval(
        int millisecond = 10                    // 数据采集间隔时间(毫秒)
    );

    void SetThumbThreshold(
        int nThumbThreshold = 500               // 摇杆触发阈值(死区)
    );

    // 设置检测每个控制器输入数据回调
    void SetSingleStateCallback(
        std::function<void(
            DWORD dwIndex,                      // 控制器索引(有效值: 0 - 3)
            const XINPUT_GAMEPAD& xGamepad      // 控制器数据
            )> cb
    );

    // 设置合并多个控制器输入数据回调
    void SetMergeStateCallback(
        std::function<void(
            const XINPUT_GAMEPAD& xGamepad      // 控制器数据
            )> cb
    );

    // 设置检测每个控制器输入状态变化回调(发生按钮按下, 释放, 按住时触发)
    void SetSingleChangeCallback(
        std::function<void(
            const XINPUT_GAMEPAD& xGamepad,     // 原始控制器数据
            const INPUT_EVENT_STATE& state           // 回调信息
            )> cb
    );

    // 设置合并多个控制器输入状态变化回调(发生按钮按下, 释放, 按住时触发)
    void SetMergeChangeCallback(
        std::function<void(
            const XINPUT_GAMEPAD& xGamepad,     // 原始控制器数据
            const INPUT_EVENT_STATE& state           // 回调信息
            )> cb
    );

    // 按钮变化回调
    virtual bool OnChangeDpadUp(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeDpadDown(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeDpadLeft(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeDpadRight(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeBack(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeStart(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeGuide(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeA(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeB(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeX(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeY(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeLeftShoulder(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeRightShoulder(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeLeftThumb(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeRightThumb(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeLeftTrigger(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeRightTrigger(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeLeftStick(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);
    virtual bool OnChangeRightStick(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state);

    // 打印状态信息
    virtual void PrintState(const INPUT_EVENT_STATE& state);

    // 打印
    void PrintText(LPCTSTR pFormat, ...);

private:

    // 任务循环
    void _GamepadLoop();

    // 睡眠间隔
    void _Sleep(int millisecond);

private:

    std::function<void(DWORD dwIndex, const XINPUT_GAMEPAD& xGamepad)> m_cbSingleState;                 // 检测每个控制器输入数据回调
    std::function<void(const XINPUT_GAMEPAD& xGamepad)> m_cbMergeState;                                 // 合并多个控制器输入数据回调
    std::function<void(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)> m_cbSingleChange;    // 检测每个控制器输入状态变化回调(发生按钮按下, 释放, 按住时触发)
    std::function<void(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)> m_cbMergeChange;     // 合并多个控制器输入状态变化回调(发生按钮按下, 释放, 按住时触发)

    std::thread m_gamepadTask;          // 控制器状态检测任务线程
    int m_ThumbThreshold;               // 摇杆触发阈值
    int m_DataInterval;                 // 采集间隔时间
    bool m_bIgnoreLeftThumbEmpty;       // 左摇杆 X,Y 轴数据均为 0 时也触发回调
    bool m_bIgnoreRightThumbEmpty;      // 右摇杆 X,Y 轴数据均为 0 时也触发回调
    bool m_fQuit = false;               // 退出标志

public:

    XINPUT_STATE m_xCurStateMixed;      // 当前混合输入
};
