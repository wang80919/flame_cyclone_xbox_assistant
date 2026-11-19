#include "CXinputHelper.h"
#include <stdint.h>
#include <Winerror.h>
#include <map>

typedef struct _XINPUT_GAMEPAD_EX
{
    DWORD                               wError;
    DWORD                               dwPacketNumber;
    WORD                                wButtons;
    BYTE                                bLeftTrigger;
    BYTE                                bRightTrigger;
    SHORT                               sThumbLX;
    SHORT                               sThumbLY;
    SHORT                               sThumbRX;
    SHORT                               sThumbRY;
} XINPUT_GAMEPAD_EX, *PXINPUT_GAMEPAD_EX;

CXInputHelper::CXInputHelper()
    :
    m_bIgnoreLeftThumbEmpty(true),
    m_bIgnoreRightThumbEmpty(true),
    m_fQuit(false),
    m_ThumbThreshold(200),
    m_DataInterval(10)
{
    memset(&m_xCurStateMixed, 0, sizeof(m_xCurStateMixed));
}

CXInputHelper::~CXInputHelper()
{
    Uninitialize();
}

bool CXInputHelper::Initialize()
{
    Uninitialize();

    m_gamepadTask = std::move(std::thread([this]() {
        _GamepadLoop();
        }));

    return true;
}

void CXInputHelper::Uninitialize()
{
    if (m_gamepadTask.joinable())
    {
        m_fQuit = true;
        m_gamepadTask.join();
    }

    m_fQuit = false;
}

void CXInputHelper::IgnoreLeftThumbEmpty(
    bool bEnable
)
{
    m_bIgnoreLeftThumbEmpty = bEnable;
}

void CXInputHelper::IgnoreRightThumbEmpty(
    bool bEnable
)
{
    m_bIgnoreRightThumbEmpty = bEnable;
}

void CXInputHelper::SetInterval(
    int millisecond
)
{
    m_DataInterval = millisecond;
}

void CXInputHelper::SetThumbThreshold(
    int nThumbThreshold/* = 500*/
)
{
    m_ThumbThreshold = nThumbThreshold;
}

void CXInputHelper::SetSingleStateCallback(
    std::function<void(
        DWORD dwIndex,
        const XINPUT_GAMEPAD& xGamepad
        )> cb
)
{
    m_cbSingleState = cb;
}

void CXInputHelper::SetMergeStateCallback(
    std::function<void(
        const XINPUT_GAMEPAD& xGamepad
        )> cb
)
{
    m_cbMergeState = cb;
}

void CXInputHelper::SetSingleChangeCallback(
    std::function<void(
        const XINPUT_GAMEPAD& xGamepad,
        const INPUT_EVENT_STATE& state
        )> cb
)
{
    m_cbSingleChange = cb;
}

void CXInputHelper::SetMergeChangeCallback(
    std::function<void(
        const XINPUT_GAMEPAD& xGamepad,
        const INPUT_EVENT_STATE& state
        )> cb
)
{
    m_cbMergeChange = cb;
}

void CXInputHelper::_Sleep(int millisecond)
{
    const int span = 10;
    if (millisecond < span)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
        return;
    }

    clock_t tmBegin = clock();
    while (true)
    {
        if (clock() - tmBegin > millisecond)
        {
            break;
        }

        if (m_fQuit)
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(span));
    }
}

void CXInputHelper::PrintText(LPCTSTR pFormat, ...)
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

void CXInputHelper::_GamepadLoop()
{
    typedef bool (CXInputHelper::*pFn)(const XINPUT_GAMEPAD& data, const INPUT_EVENT_STATE& state);
    static std::map<DWORD, pFn> buttonEvents = {
        {XINPUT_GAMEPAD_A,                          &CXInputHelper::OnChangeA},
        {XINPUT_GAMEPAD_B,                          &CXInputHelper::OnChangeB},
        {XINPUT_GAMEPAD_X,                          &CXInputHelper::OnChangeX},
        {XINPUT_GAMEPAD_Y,                          &CXInputHelper::OnChangeY},
        {XINPUT_GAMEPAD_BACK,                       &CXInputHelper::OnChangeBack},
        {XINPUT_GAMEPAD_START,                      &CXInputHelper::OnChangeStart},
        {XINPUT_GAMEPAD_GUIDE,                      &CXInputHelper::OnChangeGuide},
        {XINPUT_GAMEPAD_GUIDE,                      &CXInputHelper::OnChangeGuide},
        {XINPUT_GAMEPAD_DPAD_UP,                    &CXInputHelper::OnChangeDpadUp},
        {XINPUT_GAMEPAD_DPAD_DOWN,                  &CXInputHelper::OnChangeDpadDown},
        {XINPUT_GAMEPAD_DPAD_LEFT,                  &CXInputHelper::OnChangeDpadLeft},
        {XINPUT_GAMEPAD_DPAD_RIGHT,                 &CXInputHelper::OnChangeDpadRight},
        {XINPUT_GAMEPAD_LEFT_SHOULDER,              &CXInputHelper::OnChangeLeftShoulder},
        {XINPUT_GAMEPAD_RIGHT_SHOULDER,             &CXInputHelper::OnChangeRightShoulder},
        {XINPUT_GAMEPAD_LEFT_THUMB,                 &CXInputHelper::OnChangeLeftThumb},
        {XINPUT_GAMEPAD_RIGHT_THUMB,                &CXInputHelper::OnChangeRightThumb}
    };

    static std::map<DWORD, INPUT_EVENT_STATE> buttonInfos = {
        {XINPUT_GAMEPAD_A,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_B,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_X,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_Y,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_BACK,                       INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_START,                      INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_GUIDE,                      INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_GUIDE,                      INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_UP,                    INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_DOWN,                  INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_LEFT,                  INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_RIGHT,                 INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_LEFT_SHOULDER,              INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_RIGHT_SHOULDER,             INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_LEFT_THUMB,                 INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_RIGHT_THUMB,                INPUT_EVENT_STATE()},
    };

    static std::map<DWORD, INPUT_EVENT_STATE> buttonMergedInfos = {
        {XINPUT_GAMEPAD_A,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_B,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_X,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_Y,                          INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_BACK,                       INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_START,                      INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_GUIDE,                      INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_GUIDE,                      INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_UP,                    INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_DOWN,                  INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_LEFT,                  INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_DPAD_RIGHT,                 INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_LEFT_SHOULDER,              INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_RIGHT_SHOULDER,             INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_LEFT_THUMB,                 INPUT_EVENT_STATE()},
        {XINPUT_GAMEPAD_RIGHT_THUMB,                INPUT_EVENT_STATE()},
    };

    XINPUT_STATE xLastStateArray[XUSER_MAX_COUNT] = { 0 };
    XINPUT_STATE xLastStateMixed = { 0 };
    LONG nLastThumbLX = 0;
    LONG nLastThumbLY = 0;
    LONG nLastThumbRX = 0;
    LONG nLastThumbRY = 0;
    LONG nLastLeftTrigger = 0;
    LONG nLastRightTrigger = 0;

    // 按钮检测
    auto HitButtonState = [this](bool& bPress, bool& bRelease, bool& bHolding, const XINPUT_GAMEPAD& last, const XINPUT_GAMEPAD& cur, WORD wButton) {

        bPress = false;
        bRelease = false;
        bHolding = false;

        if ((0 == (last.wButtons & wButton)) && (cur.wButtons & wButton)) bPress = true;
        if ((last.wButtons & wButton) && (0 == (cur.wButtons & wButton))) bRelease = true;
        if ((last.wButtons & wButton) && (cur.wButtons & wButton)) bHolding = true;

        };

    // 扳机检测
    auto HitTriggerState = [this](bool& bPress, bool& bRelease, bool& bHolding, const LONG& nTrigger, const LONG& nLastTrigger) {

        bPress = false;
        bRelease = false;
        bHolding = false;

        if (0 == nLastTrigger && nTrigger) bPress = true;
        if (nLastTrigger && 0 == nTrigger) bRelease = true;
        if (nLastTrigger && nTrigger) bHolding = true;

        };

    HINSTANCE hModule = LoadLibrary(_T("XInput1_4.dll"));
    if (NULL == hModule)
    {
        hModule = LoadLibrary(_T("xinput1_3.dll"));
    }

    typedef int(__stdcall* _XInputGetStateEx)(DWORD dwUserIndex, XINPUT_STATE* pState);
    typedef int(__stdcall* _XInputWaitForGuideButton)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_GAMEPAD_EX* pState);
    typedef int(__stdcall* _XInputCancelGuideButtonWait)(DWORD dwUserIndex);
    typedef int(__stdcall* _XInputPowerOffController)(DWORD dwUserIndex);

    _XInputGetStateEx XInputGetStateEx = (_XInputGetStateEx)::GetProcAddress(hModule, (LPCSTR)100);
    _XInputWaitForGuideButton XInputWaitForGuideButton = (_XInputWaitForGuideButton)::GetProcAddress(hModule, (LPCSTR)101);
    _XInputCancelGuideButtonWait XInputCancelGuideButtonWait = (_XInputCancelGuideButtonWait)::GetProcAddress(hModule, (LPCSTR)102);
    _XInputPowerOffController XInputPowerOffController = (_XInputPowerOffController)::GetProcAddress(hModule, (LPCSTR)103);

    if (nullptr == XInputGetStateEx)
    {
        XInputGetStateEx = (_XInputGetStateEx)::GetProcAddress(hModule, "XInputGetState");
    }

    while (!m_fQuit && XInputGetStateEx)
    {
        XINPUT_STATE xCurStateArray[XUSER_MAX_COUNT] = { 0 };
        XINPUT_STATE xCurStateMixed = { 0 };
        LONG nThumbLX = 0;
        LONG nThumbLY = 0;
        LONG nThumbRX = 0;
        LONG nThumbRY = 0;
        LONG nLeftTrigger = 0;
        LONG nRightTrigger = 0;

        // 遍历控制器输入
        for (int i = 0; i < XUSER_MAX_COUNT; ++i)
        {
            XINPUT_STATE xTmpState = { 0 };
            DWORD dwResult = XInputGetStateEx(i, &xTmpState);

            //触发值限定
            if (abs(xTmpState.Gamepad.sThumbLX) < m_ThumbThreshold) xTmpState.Gamepad.sThumbLX = 0;
            if (abs(xTmpState.Gamepad.sThumbLY) < m_ThumbThreshold) xTmpState.Gamepad.sThumbLY = 0;
            if (abs(xTmpState.Gamepad.sThumbRX) < m_ThumbThreshold) xTmpState.Gamepad.sThumbRX = 0;
            if (abs(xTmpState.Gamepad.sThumbRY) < m_ThumbThreshold) xTmpState.Gamepad.sThumbRY = 0;

            xCurStateArray[i] = xTmpState;

            //未连接控制器
            if (ERROR_DEVICE_NOT_CONNECTED == dwResult)
            {
                xLastStateArray[i] = xCurStateArray[i];
                continue;
            }

            // 累加多输入数据
            {
                xCurStateMixed.Gamepad.wButtons |= xTmpState.Gamepad.wButtons;
                nLeftTrigger += xTmpState.Gamepad.bLeftTrigger;
                nRightTrigger += xTmpState.Gamepad.bRightTrigger;
                nThumbLX += xTmpState.Gamepad.sThumbLX;
                nThumbLY += xTmpState.Gamepad.sThumbLY;
                nThumbRX += xTmpState.Gamepad.sThumbRX;
                nThumbRY += xTmpState.Gamepad.sThumbRY;
            }

            //连接控制器
            if (ERROR_SUCCESS == dwResult)
            {
                // 单控制器输入回调
                if (m_cbSingleState)
                {
                    m_cbSingleState(i, xTmpState.Gamepad);
                }

                // 单控制器变化回调
                if (m_cbSingleChange)
                {
                    XINPUT_GAMEPAD& lastState = xLastStateArray[i].Gamepad;
                    XINPUT_GAMEPAD& curState = xCurStateArray[i].Gamepad;

                    // 按键变化处理
                    for (const auto& item : buttonEvents)
                    {
                        INPUT_EVENT_STATE& state = buttonInfos.find(item.first)->second;
                        state.dwIndex = i;

                        bool bPress = false;
                        bool bRelease = false;
                        bool bHolding = false;

                        HitButtonState(bPress, bRelease, bHolding, lastState, curState, item.first);
                        if (bPress || bRelease || bHolding)
                        {
                            state.fDown = bPress;
                            state.fUp = bRelease;
                            state.fHold = bHolding;
                            state.wButton = item.first;

                            if (bPress)
                            {
                                state.dwTime = ::clock();
                            }

                            m_cbSingleChange(curState, state);

                            if (bRelease)
                            {
                                state.dwTime = 0;
                            }
                        }
                    }

                    // 扳机变化处理
                    {
                        bool bPress = false;
                        bool bRelease = false;
                        bool bHolding = false;

                        // 左扳机
                        HitTriggerState(bPress, bRelease, bHolding, curState.bLeftTrigger, lastState.bLeftTrigger);
                        if (bPress || bRelease || bHolding)
                        {
                            INPUT_EVENT_STATE state;
                            state.dwIndex = i;
                            state.fDown = bPress;
                            state.fUp = bRelease;
                            state.fHold = bHolding;
                            state.bRight = false;
                            state.bTrigger = nLeftTrigger;

                            m_cbSingleChange(curState, state);
                        }

                        // 右扳机
                        HitTriggerState(bPress, bRelease, bHolding, curState.bRightTrigger, lastState.bRightTrigger);
                        if (bPress || bRelease || bHolding)
                        {
                            INPUT_EVENT_STATE state;
                            state.dwIndex = i;
                            state.fDown = bPress;
                            state.fUp = bRelease;
                            state.fHold = bHolding;
                            state.bRight = true;
                            state.bTrigger = nRightTrigger;

                            m_cbSingleChange(curState, state);
                        }
                    }

                    // 摇杆变化处理
                    {
                        // 左摇杆
                        if (curState.sThumbLX || curState.sThumbLY || m_bIgnoreLeftThumbEmpty)
                        {
                            INPUT_EVENT_STATE state;
                            state.dwIndex = i;
                            state.sThumbX = curState.sThumbLX;
                            state.sThumbY = curState.sThumbLY;
                            state.bRight = false;

                            m_cbSingleChange(curState, state);
                        }

                        // 右摇杆
                        if (curState.sThumbRX || curState.sThumbRY || m_bIgnoreRightThumbEmpty)
                        {
                            INPUT_EVENT_STATE state;
                            state.dwIndex = i;
                            state.sThumbX = curState.sThumbRX;
                            state.sThumbY = curState.sThumbRY;
                            state.bRight = true;

                            m_cbSingleChange(curState, state);
                        }
                    }
                }
            }

            // 记录状态
            xLastStateArray[i] = xCurStateArray[i];
        }

        // 多输入合并
        {
            // 限制范围
            if (nThumbLX > INT16_MAX) nThumbLX = INT16_MAX;
            if (nThumbLY > INT16_MAX) nThumbLY = INT16_MAX;
            if (nThumbRX > INT16_MAX) nThumbRX = INT16_MAX;
            if (nThumbRY > INT16_MAX) nThumbRY = INT16_MAX;
            if (nThumbLX < INT16_MIN) nThumbLX = INT16_MIN;
            if (nThumbLY < INT16_MIN) nThumbLY = INT16_MIN;
            if (nThumbRX < INT16_MIN) nThumbRX = INT16_MIN;
            if (nThumbRY < INT16_MIN) nThumbRY = INT16_MIN;
            if (nLeftTrigger > UINT8_MAX) nLeftTrigger = UINT8_MAX;
            if (nRightTrigger > UINT8_MAX) nRightTrigger = UINT8_MAX;

            xCurStateMixed.Gamepad.sThumbLX = nThumbLX;
            xCurStateMixed.Gamepad.sThumbLY = nThumbLY;
            xCurStateMixed.Gamepad.sThumbRX = nThumbRX;
            xCurStateMixed.Gamepad.sThumbRY = nThumbRY;
            xCurStateMixed.Gamepad.bLeftTrigger = nLeftTrigger;
            xCurStateMixed.Gamepad.bRightTrigger = nRightTrigger;
        }

        m_xCurStateMixed = xCurStateMixed;

        // 合并状态回调处理
        if (m_cbMergeState)
        {
            m_cbMergeState(xCurStateMixed.Gamepad);
        }

        // 按键变化处理
        for (const auto& item : buttonEvents)
        {
            bool bPress = false;
            bool bRelease = false;
            bool bHolding = false;

            HitButtonState(bPress, bRelease, bHolding, xLastStateMixed.Gamepad, xCurStateMixed.Gamepad, item.first);
            if (bPress || bRelease || bHolding)
            {
                INPUT_EVENT_STATE& state = buttonInfos.find(item.first)->second;
                state.fDown = bPress;
                state.fUp = bRelease;
                state.fHold = bHolding;
                state.wButton = item.first;

                if (bPress)
                {
                    state.dwTime = ::clock();
                }

                if (bHolding)
                {
                    state.dwDuration = ::clock() - state.dwTime;
                }

                (this->*item.second)(xCurStateMixed.Gamepad, state);

                // 合并数据变化回调
                if (m_cbMergeChange)
                {
                    m_cbMergeChange(xCurStateMixed.Gamepad, state);
                }

                if (bRelease)
                {
                    state.dwTime = 0;
                    state.dwDuration = 0;
                }
            }
        }

        // 扳机变化处理
        {
            bool bPress = false;
            bool bRelease = false;
            bool bHolding = false;

            // 左扳机
            HitTriggerState(bPress, bRelease, bHolding, nLeftTrigger, nLastLeftTrigger);
            if (bPress || bRelease || bHolding)
            {
                static INPUT_EVENT_STATE state;
                state.fDown = bPress;
                state.fUp = bRelease;
                state.fHold = bHolding;
                state.bRight = false;
                state.bTrigger = nLeftTrigger;

                if (bPress)
                {
                    state.dwTime = ::clock();
                }

                if (bHolding)
                {
                    state.dwDuration = ::clock() - state.dwTime;
                }

                OnChangeLeftTrigger(xCurStateMixed.Gamepad, state);

                // 变化回调处理
                if (m_cbMergeChange)
                {
                    m_cbMergeChange(xCurStateMixed.Gamepad, state);
                }

                if (bRelease)
                {
                    state.dwTime = 0;
                    state.dwDuration = 0;
                }
            }

            // 右扳机
            HitTriggerState(bPress, bRelease, bHolding, nRightTrigger, nLastRightTrigger);
            if (bPress || bRelease || bHolding)
            {
                static INPUT_EVENT_STATE state;
                state.fDown = bPress;
                state.fUp = bRelease;
                state.fHold = bHolding;
                state.bRight = true;
                state.bTrigger = nRightTrigger;

                if (bPress)
                {
                    state.dwTime = ::clock();
                }

                if (bHolding)
                {
                    state.dwDuration = ::clock() - state.dwTime;
                }

                OnChangeRightTrigger(xCurStateMixed.Gamepad, state);

                // 合并数据变化回调
                if (m_cbMergeChange)
                {
                    m_cbMergeChange(xCurStateMixed.Gamepad, state);
                }

                if (bRelease)
                {
                    state.dwTime = 0;
                    state.dwDuration = 0;
                }
            }
        }

        // 摇杆变化处理
        {
            // 左摇杆
            if (xCurStateMixed.Gamepad.sThumbLX || xCurStateMixed.Gamepad.sThumbLY || m_bIgnoreLeftThumbEmpty)
            {
                static INPUT_EVENT_STATE state;
                state.sThumbX = xCurStateMixed.Gamepad.sThumbLX;
                state.sThumbY = xCurStateMixed.Gamepad.sThumbLY;
                state.bRight = false;

                OnChangeLeftStick(xCurStateMixed.Gamepad, state);

                // 合并数据变化回调
                if (m_cbMergeChange)
                {
                    m_cbMergeChange(xCurStateMixed.Gamepad, state);
                }
            }

            // 右摇杆
            if (xCurStateMixed.Gamepad.sThumbRX || xCurStateMixed.Gamepad.sThumbRY || m_bIgnoreRightThumbEmpty)
            {
                static INPUT_EVENT_STATE state;
                state.sThumbX = xCurStateMixed.Gamepad.sThumbRX;
                state.sThumbY = xCurStateMixed.Gamepad.sThumbRY;
                state.bRight = true;

                OnChangeRightStick(xCurStateMixed.Gamepad, state);

                // 合并数据变化回调
                if (m_cbMergeChange)
                {
                    m_cbMergeChange(xCurStateMixed.Gamepad, state);
                }
            }
        }

        // 记录上次状态
        xLastStateMixed = xCurStateMixed;
        nLastThumbLX= nThumbLX;
        nLastThumbLY= nThumbLY;
        nLastThumbRX= nThumbRX;
        nLastThumbRY= nThumbRY;
        nLastLeftTrigger = nLeftTrigger ;
        nLastRightTrigger = nRightTrigger;

        _Sleep(m_DataInterval);
    }

    if (hModule)
    {
        ::FreeLibrary(hModule);
    }
}

bool CXInputHelper::OnChangeA(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeB(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeX(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeY(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeBack(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeStart(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeGuide(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeDpadUp(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeDpadDown(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeDpadLeft(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeDpadRight(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeLeftShoulder(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeRightShoulder(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeLeftThumb(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeRightThumb(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeLeftTrigger(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeRightTrigger(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeLeftStick(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

bool CXInputHelper::OnChangeRightStick(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    PrintState(state);
    return true;
}

void CXInputHelper::PrintState(const INPUT_EVENT_STATE& state)
{
    return;
    PrintText(_T("OnRightThumb Index: %d bRight: %d Trigger: %3d %6d,%6d Button: %04X Press: %d Release: %d Holding: %d\r\n"),
        state.dwIndex, state.bRight, state.bTrigger, state.sThumbX, state.sThumbY, state.wButton, state.fDown, state.fUp, state.fHold);
}
