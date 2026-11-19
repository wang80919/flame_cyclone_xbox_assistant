#pragma once
#include "CWindow/CDialogBase.h"
#include "CWin32Utils/CXInputHelper.h"
#include "CWin32Utils/CStrUtils.h"
#include "CWin32Utils/CIniHelper.h"

#define NORMAL_COLOR                                    RGB(200, 200, 255)
#define ACTIVE_COLOR                                    RGB(255, 255, 0)
#define LAST_BUTTON_COLOR                               RGB(128, 128, 255)
#define VALUE_COLOR                                     RGB(255, 0, 255)
#define LAST_VALUE_COLOR                                RGB(96, 96, 255)

#define MOUSE_MOVE_THRESHOLD                            (1024 * 6)     //鼠标移动摇杆阈值
#define MOUSE_SRCOLL_THRESHOLD                          (32768 / 2)     //鼠标滚轮摇杆阈值

enum eMouseType
{
    eMouseNone,
    eMouseMove,
    eMouseWheel,
    eMouseLeft,
    eMouseRight,
    eMouseMiddle
};


class CMainFrame: public CDialogBase, CXInputHelper
{
public:
    LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
    LRESULT OnClose(WPARAM wParam, LPARAM lParam);
    LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
    LRESULT OnEraseBkgnd(WPARAM wParam, LPARAM lParam);
    LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
    LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
    LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
    LRESULT OnCtlColorDlg(WPARAM wParam, LPARAM lParam);

    LRESULT OnReset(WORD wNotify, WORD wID, HWND hWnd);
    LRESULT OnVibrationLeft(WORD wNotify, WORD wID, HWND hWnd);
    LRESULT OnVibrationRight(WORD wNotify, WORD wID, HWND hWnd);
    LRESULT OnKey(WORD wNotify, WORD wID, HWND hWnd);
    LRESULT OnMouse(WORD wNotify, WORD wID, HWND hWnd);
    LRESULT OnNone(WORD wNotify, WORD wID, HWND hWnd);

    LRESULT OnModifierChange(WORD wNotify, WORD wID, HWND hWnd);
    LRESULT OnKeyChange(WORD wNotify, WORD wID, HWND hWnd);
    LRESULT OnMouseChange(WORD wNotify, WORD wID, HWND hWnd);

    LRESULT OnDrawButton(WORD wNotify, WORD wID, HWND hWnd);

private:

    RECT GetItemRect(WORD wID);
    void DrawButtonItem(DWORD wID);
    void DrawThumb(LPDRAWITEMSTRUCT lpDrawItem, WORD wID);
    void DrawNormalButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID);
    void DrawABXYButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID);
    void DrawDpadButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID);
    void DrawTriggerButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID);
    void UpdateButtonFunction(WORD wID);
    void ShowButtonFunction(WORD wID);

    void InitList();
    void SaveConfig();
    void LoadConfig();
    void DoFunction(WORD wID, const INPUT_EVENT_STATE& state);
    void DoMouseMove(int x, int y);
    void DoMouseWheel(int x, int y);
private:

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


protected:

    DECLARE_DLG_MESSAGE_MAP()

private:

    HDC             m_hMemDC = NULL;            // 内存DC
    HBITMAP         m_hMemBitmap = NULL;        // 内存位图
    HBRUSH          m_hBgBrush = NULL;

    XINPUT_STATE    m_lastState;
    bool m_fWndQuit = false;
    bool m_fVibrationLeft = false;
    bool m_fVibrationRight = false;
    HWND m_hModifier;
    HWND m_hKeys;
    HWND m_hMouse;
    HWND m_hNone;
    WORD m_wFocusID = 0;

    XINPUT_VIBRATION m_Vibration = { 0 };
    CIniHelper m_config;

    LONG m_sThumbLX_Min = 0;
    LONG m_sThumbLY_Min = 0;
    LONG m_sThumbRX_Min = 0;
    LONG m_sThumbRY_Min = 0;
    LONG m_sThumbLX_Max = 0;
    LONG m_sThumbLY_Max = 0;
    LONG m_sThumbRX_Max = 0;
    LONG m_sThumbRY_Max = 0;
};
