#include "CMainFrame.h"
#include "resource.h"
#include <map>
#include <vector>
#include "CWin32Utils/CInputUtils.h"

enum eButtonFunction
{
    eNone,
    eKey,
    eMouse
};

typedef struct _BUTTON_FUNCTION_INFO
{
    eButtonFunction eType;                  // 0: 无 1: 键盘 2: 鼠标
    std::vector<int> vModifier;             // 修饰键名
    std::vector<int> vKeys;                 // 键盘名
    std::vector<int> vMouse;                // 鼠标功能
}BUTTON_FUNCTION_INFO;

std::map<DWORD, BUTTON_FUNCTION_INFO> g_ButtonFunctionList;

//原始按键
LPCTSTR g_ModifierName[] = {
    _T("None"),
    _T("LCtrl"),        
    _T("RCtrl"),        
    _T("LAlt"),         
    _T("RAlt"),         
    _T("LWin"),         
    _T("RWin"),         
    _T("LShift"),       
    _T("RShift")     
};

//原始按键
LPCTSTR g_KeysName[] = {
    _T("None"),
    _T("Esc"),          
    _T("F1"),           
    _T("F2"),           
    _T("F3"),           
    _T("F4"),           
    _T("F5"),           
    _T("F6"),           
    _T("F7"),           
    _T("F8"),           
    _T("F9"),           
    _T("F10"),          
    _T("F11"),          
    _T("F12"),          

    _T("`"),            
    _T("1"),            
    _T("2"),            
    _T("3"),            
    _T("4"),            
    _T("5"),            
    _T("6"),            
    _T("7"),            
    _T("8"),            
    _T("9"),            
    _T("0"),            
    _T("-"),            
    _T("="),            
    _T("Backspace"),    

    _T("Application"),  

    _T("Tab"),          
    _T("Space"),        
    _T("Enter"),        

    _T("["),            
    _T("]"),            
    _T("\\"),           

    _T(";"),            
    _T("\'"),           

    _T(","),            
    _T("."),            
    _T("/"),            

    _T("A"),            
    _T("B"),            
    _T("C"),            
    _T("D"),            
    _T("E"),            
    _T("F"),            
    _T("G"),            
    _T("H"),            
    _T("I"),            
    _T("J"),            
    _T("K"),            
    _T("L"),            
    _T("M"),            
    _T("N"),            
    _T("O"),            
    _T("P"),            
    _T("Q"),            
    _T("R"),            
    _T("S"),            
    _T("T"),            
    _T("U"),            
    _T("V"),            
    _T("W"),            
    _T("X"),            
    _T("Y"),            
    _T("Z"),            

    _T("Up"),           
    _T("Down"),         
    _T("Left"),         
    _T("Right"),        

    /*_T("UpArrow"),      
    _T("DownArrow"),    
    _T("LeftArrow"),    
    _T("RightArrow"),   */

    _T("Num Up"),       
    _T("Num Down"),     
    _T("Num Left"),     
    _T("Num Right"),    

    _T("Insert"),       
    _T("Home"),         
    _T("Page Up"),      
    _T("Delete"),       
    _T("End"),          
    _T("Page Down"),    

    _T("Prnt Scrn"),    
    _T("PrintScreen"),

    _T("Scroll Lock"),  
    _T("Num Lock"),     
    _T("Caps Lock"),    
    _T("Pause"),        

    _T("Num 0"),        
    _T("Num 1"),        
    _T("Num 2"),        
    _T("Num 3"),        
    _T("Num 4"),        
    _T("Num 5"),        
    _T("Num 6"),        
    _T("Num 7"),        
    _T("Num 8"),        
    _T("Num 9"),        

    _T("Num Home"),     
    _T("Num End"),      
    _T("Num Page Up"),  
    _T("Num Page Down"),

    _T("Clear"),        

    _T("Num /"),        
    _T("Num *"),        
    _T("Num -"),        
    _T("Num +"),        
    _T("Num Enter"),    
    _T("Num Del"),      
    _T("Num Insert"),   
    _T("Num ."),        

};  //118个按键

//原始按键
LPCTSTR g_MouseName[] = {
    _T("None"),
    _T("Move"),
    _T("Wheel"),
    _T("Left Button"),
    _T("Right Button"),
    _T("Middle Button"),
};

BEGIN_DLG_MESSAGE_MAP(CMainFrame, CDialogBase)
    ON_DLG_MESSAGE(WM_INITDIALOG, &CMainFrame::OnInitDialog)
    ON_DLG_MESSAGE(WM_CLOSE, &CMainFrame::OnClose)
    ON_DLG_MESSAGE(WM_DESTROY, &CMainFrame::OnDestroy)
    ON_DLG_MESSAGE(WM_ERASEBKGND, &CMainFrame::OnEraseBkgnd)
    ON_DLG_MESSAGE(WM_PAINT, &CMainFrame::OnPaint)
    //ON_DLG_MESSAGE(WM_CTLCOLORDLG, &CMainFrame::OnCtlColorDlg)
    ON_DLG_MESSAGE(WM_LBUTTONDOWN, &CMainFrame::OnLButtonDown)
    ON_DLG_MESSAGE(WM_DRAWITEM, &CMainFrame::OnDrawItem)
    ON_DLG_COMMAND(IDC_CHECK_VIBRATION_LEFT, &CMainFrame::OnVibrationLeft)
    ON_DLG_COMMAND(IDC_CHECK_VIBRATION_RIGHT, &CMainFrame::OnVibrationRight)
    ON_DLG_COMMAND(IDC_BUTTON_RESET, &CMainFrame::OnReset)
    ON_DLG_COMMAND(IDC_RADIO_KEY, &CMainFrame::OnKey)
    ON_DLG_COMMAND(IDC_RADIO_MOUSE, &CMainFrame::OnMouse)
    ON_DLG_COMMAND(IDC_RADIO_NONE, &CMainFrame::OnNone)

    ON_DLG_COMMAND_NOTIFY(LBN_SELCHANGE, IDC_LIST_MODIFIER, &CMainFrame::OnKeyChange)
    ON_DLG_COMMAND_NOTIFY(LBN_SELCHANGE, IDC_LIST_KEYS, &CMainFrame::OnKeyChange)
    ON_DLG_COMMAND_NOTIFY(LBN_SELCHANGE, IDC_LIST_MOUSE, &CMainFrame::OnMouseChange)

    ON_DLG_COMMAND(IDC_BUTTON_LS            , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_RS            , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_LEFTSTICK     , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_RIGHTSTICK    , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_DPAD_UP       , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_DPAD_DOWN     , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_DPAD_LEFT     , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_DPAD_RIGHT    , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_Y             , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_A             , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_X             , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_B             , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_BACK          , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_START         , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_GUIDE         , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_LB            , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_RB            , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_LT            , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_RT            , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_BUTTON_RESET         , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_CHECK_VIBRATION_LEFT , &CMainFrame::OnDrawButton)
    ON_DLG_COMMAND(IDC_CHECK_VIBRATION_RIGHT, &CMainFrame::OnDrawButton)

END_DLG_MESSAGE_MAP()

LRESULT CMainFrame::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
    memset(&m_lastState, 0, sizeof(m_lastState));

    RECT rect = { 0 };
    ::GetClientRect(m_hWnd, &rect);
    WORD cxClient = rect.right - rect.left;
    WORD cyClient = rect.bottom - rect.top;

    HDC hdc = ::GetDC(m_hWnd);

    BuildDcBitmap(hdc, &m_hMemDC, &m_hMemBitmap, cxClient, cyClient);

    HFONT hFont = (HFONT)SendMessage(WM_GETFONT, 0, 0);
    m_hBgBrush = (HBRUSH)SendMessage(WM_CTLCOLORDLG, 0, 0);
    ::FillRect(m_hMemDC, &rect, m_hBgBrush);

    ::SelectObject(m_hMemDC, hFont);

    // 设置图标
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    if (hIcon)
    {
        ::SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        ::SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

    PostCommand(IDC_BUTTON_RESET);

    Initialize();

    m_hModifier = GetItem(IDC_LIST_MODIFIER);
    m_hKeys = GetItem(IDC_LIST_KEYS);
    m_hMouse = GetItem(IDC_LIST_MOUSE);
    InitList();

    ::EnableWindow(m_hModifier, FALSE);
    ::EnableWindow(m_hKeys, FALSE);
    ::EnableWindow(m_hMouse, FALSE);
    CheckButton(IDC_RADIO_NONE, true);
    CheckButton(IDC_RADIO_KEY, false);
    CheckButton(IDC_RADIO_MOUSE, false);

    LoadConfig();

    return (LRESULT)TRUE;
}

LRESULT CMainFrame::OnCtlColorDlg(WPARAM wParam, LPARAM lParam)
{
    return (LRESULT)GetStockBrush(BLACK_BRUSH);
}

LRESULT CMainFrame::OnClose(WPARAM wParam, LPARAM lParam)
{
    m_fWndQuit = true;
    Uninitialize();
    DeleteDcBitmap(&m_hMemDC, &m_hMemBitmap);

    SaveConfig();

    return (LRESULT)FALSE;
}

LRESULT CMainFrame::OnDestroy(WPARAM wParam, LPARAM lParam)
{
    return (LRESULT)FALSE;
}

LRESULT CMainFrame::OnEraseBkgnd(WPARAM wParam, LPARAM lParam)
{
    return (LRESULT)TRUE;
}

LRESULT CMainFrame::OnPaint(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps = { 0 };
    HDC hdc = ::BeginPaint(m_hWnd, &ps);

    RECT rect = { 0 };
    ::GetClientRect(m_hWnd, &rect);

    {
        ::BitBlt(hdc, 0, 0, rect.right, rect.bottom, m_hMemDC, 0, 0, SRCCOPY);
    }

    ::EndPaint(m_hWnd, &ps);

    return FALSE;
}

void CMainFrame::DrawButtonItem(DWORD wID)
{
    PostCommand(wID);

    return;

    WORD wIDs[] = {
        IDC_BUTTON_LEFTSTICK        ,
        IDC_BUTTON_RIGHTSTICK       ,
        IDC_BUTTON_DPAD_UP          ,
        IDC_BUTTON_DPAD_DOWN        ,
        IDC_BUTTON_DPAD_LEFT        ,
        IDC_BUTTON_DPAD_RIGHT       ,
        IDC_BUTTON_Y                ,
        IDC_BUTTON_A                ,
        IDC_BUTTON_X                ,
        IDC_BUTTON_B                ,
        IDC_BUTTON_BACK             ,
        IDC_BUTTON_START            ,
        IDC_BUTTON_GUIDE            ,
        IDC_BUTTON_LB               ,
        IDC_BUTTON_RB               ,
        IDC_BUTTON_LT               ,
        IDC_BUTTON_RT               ,
        IDC_BUTTON_LS               ,
        IDC_BUTTON_RS               ,
    };

    // 刷新显示
    for (auto item : wIDs)
    {
        DRAWITEMSTRUCT DrawItem = { 0 };
        DrawItem.CtlID = item;
        OnDrawItem(0, (LPARAM)&DrawItem);
        InvalidateRect(GetItem(item), NULL, FALSE);
    }
}

LRESULT CMainFrame::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    WORD wIDs[] = {
        IDC_BUTTON_LEFTSTICK        ,
        IDC_BUTTON_RIGHTSTICK       ,
        IDC_BUTTON_DPAD_UP          ,
        IDC_BUTTON_DPAD_DOWN        ,
        IDC_BUTTON_DPAD_LEFT        ,
        IDC_BUTTON_DPAD_RIGHT       ,
        IDC_BUTTON_Y                ,
        IDC_BUTTON_A                ,
        IDC_BUTTON_X                ,
        IDC_BUTTON_B                ,
        IDC_BUTTON_BACK             ,
        IDC_BUTTON_START            ,
        IDC_BUTTON_GUIDE            ,
        IDC_BUTTON_LB               ,
        IDC_BUTTON_RB               ,
        IDC_BUTTON_LT               ,
        IDC_BUTTON_RT               ,
        IDC_BUTTON_LS               ,
        IDC_BUTTON_RS               ,
    };

    for (const auto& item : wIDs)
    {
        RECT reReset = GetItemRect(item);
        POINT pt = { 0 };
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);

        if (PtInRect(&reReset, pt))
        {
            m_wFocusID = item;
        }
    }

    // 刷新显示
    for (auto item : wIDs)
    {
        DRAWITEMSTRUCT DrawItem = { 0 };
        DrawItem.CtlID = item;
        OnDrawItem(0, (LPARAM)&DrawItem);
        InvalidateRect(GetItem(item), NULL, FALSE);
    }

    ShowButtonFunction(m_wFocusID);

    return FALSE;
}

LRESULT CMainFrame::OnVibrationLeft(WORD wNotify, WORD wID, HWND hWnd)
{
    m_fVibrationLeft = IsButtonChecked(wID);

    return TRUE;
}

LRESULT CMainFrame::OnVibrationRight(WORD wNotify, WORD wID, HWND hWnd)
{
    m_fVibrationRight = IsButtonChecked(wID);

    return TRUE;
}

LRESULT CMainFrame::OnKey(WORD wNotify, WORD wID, HWND hWnd)
{
    EnableWindow(m_hModifier, true);
    EnableWindow(m_hKeys, true);
    EnableWindow(m_hMouse, false);

    UpdateButtonFunction(wID);
    return TRUE;
}

LRESULT CMainFrame::OnMouse(WORD wNotify, WORD wID, HWND hWnd)
{
    EnableWindow(m_hModifier, false);
    EnableWindow(m_hKeys, false);
    EnableWindow(m_hMouse, true);

    UpdateButtonFunction(wID);
    return TRUE;
}

LRESULT CMainFrame::OnNone(WORD wNotify, WORD wID, HWND hWnd)
{
    EnableWindow(m_hModifier, false);
    EnableWindow(m_hKeys, false);
    EnableWindow(m_hMouse, false);

    UpdateButtonFunction(wID);
    return TRUE;
}

LRESULT CMainFrame::OnReset(WORD wNotify, WORD wID, HWND hWnd)
{
    RECT rtClient = { 0 };

    ::GetClientRect(m_hWnd, &rtClient);

    ::FillRect(m_hMemDC, &rtClient, (HBRUSH)SendMessage(WM_CTLCOLORDLG, 0, 0));

    m_sThumbLX_Min = 0;
    m_sThumbLY_Min = 0;
    m_sThumbRX_Min = 0;
    m_sThumbRY_Min = 0;
    m_sThumbLX_Max = 0;
    m_sThumbLY_Max = 0;
    m_sThumbRX_Max = 0;
    m_sThumbRY_Max = 0;

    memset(&m_lastState, 0, sizeof(m_lastState));

    WORD wIDs[] = {
    IDC_BUTTON_LS       ,
    IDC_BUTTON_RS       ,
    IDC_BUTTON_LEFTSTICK        ,
    IDC_BUTTON_RIGHTSTICK       ,
    IDC_BUTTON_DPAD_UP          ,
    IDC_BUTTON_DPAD_DOWN        ,
    IDC_BUTTON_DPAD_LEFT        ,
    IDC_BUTTON_DPAD_RIGHT       ,
    IDC_BUTTON_Y                ,
    IDC_BUTTON_A                ,
    IDC_BUTTON_X                ,
    IDC_BUTTON_B                ,
    IDC_BUTTON_BACK             ,
    IDC_BUTTON_START            ,
    IDC_BUTTON_GUIDE            ,
    IDC_BUTTON_LB               ,
    IDC_BUTTON_RB               ,
    IDC_BUTTON_LT               ,
    IDC_BUTTON_RT               ,
    };

    for (auto item : wIDs)
    {
        PostCommand(item);
    }

    return (LRESULT)TRUE;
}

LRESULT CMainFrame::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
    lpDrawItem->hDC = m_hMemDC;
    lpDrawItem->rcItem = GetItemRect(lpDrawItem->CtlID);

    if (lpDrawItem->CtlID == IDC_BUTTON_A ||
        lpDrawItem->CtlID == IDC_BUTTON_B ||
        lpDrawItem->CtlID == IDC_BUTTON_X ||
        lpDrawItem->CtlID == IDC_BUTTON_Y
        )
    {
        DrawABXYButton(lpDrawItem, lpDrawItem->CtlID);
    }

    if (lpDrawItem->CtlID == IDC_BUTTON_DPAD_UP || 
        lpDrawItem->CtlID == IDC_BUTTON_DPAD_DOWN || 
        lpDrawItem->CtlID == IDC_BUTTON_DPAD_LEFT || 
        lpDrawItem->CtlID == IDC_BUTTON_DPAD_RIGHT
        )
    {
        DrawDpadButton(lpDrawItem, lpDrawItem->CtlID);
    }

    if (lpDrawItem->CtlID == IDC_BUTTON_LB ||
        lpDrawItem->CtlID == IDC_BUTTON_RB ||
        lpDrawItem->CtlID == IDC_BUTTON_LS ||
        lpDrawItem->CtlID == IDC_BUTTON_RS ||
        lpDrawItem->CtlID == IDC_BUTTON_BACK ||
        lpDrawItem->CtlID == IDC_BUTTON_GUIDE ||
        lpDrawItem->CtlID == IDC_BUTTON_START || 
        lpDrawItem->CtlID == IDC_BUTTON_RESET
        )
    {
        DrawNormalButton(lpDrawItem, lpDrawItem->CtlID);
    }

    if (lpDrawItem->CtlID == IDC_BUTTON_LT || 
        lpDrawItem->CtlID == IDC_BUTTON_RT
        )
    {
        DrawTriggerButton(lpDrawItem, lpDrawItem->CtlID);
    }

    if (lpDrawItem->CtlID == IDC_BUTTON_LEFTSTICK || 
        lpDrawItem->CtlID == IDC_BUTTON_RIGHTSTICK
        )
    {
        DrawThumb(lpDrawItem, lpDrawItem->CtlID);
    }

    ::InvalidateRect(m_hWnd, NULL, FALSE);

    return (LRESULT)TRUE;
}

RECT CMainFrame::GetItemRect(WORD wID)
{
    RECT rtItem = { 0 };
    ::GetWindowRect(GetItem(wID), &rtItem);

    POINT ptClient = { rtItem.left, rtItem.top };
    ::ScreenToClient(m_hWnd, &ptClient);

    RECT rtResult = { 0 };
    rtResult.left = ptClient.x;
    rtResult.top = ptClient.y;
    rtResult.right = ptClient.x + (rtItem.right - rtItem.left);
    rtResult.bottom = ptClient.y + (rtItem.bottom - rtItem.top);

    return rtResult;
}

void CMainFrame::DrawThumb(LPDRAWITEMSTRUCT lpDrawItem, WORD wID)
{
    HBRUSH hBkgBrush = m_hBgBrush;
    HBRUSH hNormalBrush = ::CreateSolidBrush(NORMAL_COLOR);
    HBRUSH hActiveBrush = ::CreateSolidBrush(ACTIVE_COLOR);
    HBRUSH hValueBrush = ::CreateSolidBrush(VALUE_COLOR);
    HBRUSH hLastBrush = ::CreateSolidBrush(LAST_BUTTON_COLOR);
    HBRUSH hLastValueBrush = ::CreateSolidBrush(LAST_VALUE_COLOR);

    HDC hdc = lpDrawItem->hDC;
    RECT rcItem = lpDrawItem->rcItem;

    LONG nWidth = rcItem.right - rcItem.left;
    LONG nHeight = rcItem.bottom - rcItem.top;

    BYTE bTrigger = 0;

    LONG sThumbX = 0;
    LONG sThumbY = 0;

    _tstring strText = GetItemString(wID);

    ::SetBkMode(hdc, TRANSPARENT);
    {
        RECT rcBg = lpDrawItem->rcItem;
        rcBg.right += 1;
        rcBg.bottom += 1;
        ::FillRect(hdc, &rcBg, hBkgBrush);
    }

    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNormalBrush);
    ::Ellipse(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
    ::SelectObject(hdc, hOldBrush);

    if (wID == IDC_BUTTON_LEFTSTICK)
    {
        sThumbX = m_xCurStateMixed.Gamepad.sThumbLX;
        sThumbY = -m_xCurStateMixed.Gamepad.sThumbLY;

        _tstring strValue = CStrUtils::Format(_T("X: %6d"), sThumbX);
        RECT rtText = GetItemRect(IDC_STATIC_LEFT_STICK_X);
        ::FillRect(hdc, &rtText, hBkgBrush);
        ::TextOut(hdc, rtText.left, rtText.top, strValue.c_str(), strValue.size());

        strValue = CStrUtils::Format(_T("Y: %6d"), -sThumbY);
        rtText = GetItemRect(IDC_STATIC_LEFT_STICK_Y);
        ::FillRect(hdc, &rtText, hBkgBrush);
        ::TextOut(hdc, rtText.left, rtText.top, strValue.c_str(), strValue.size());
    }

    if (wID == IDC_BUTTON_RIGHTSTICK)
    {
        sThumbX = m_xCurStateMixed.Gamepad.sThumbRX;
        sThumbY = -m_xCurStateMixed.Gamepad.sThumbRY;

        _tstring strValue = CStrUtils::Format(_T("X: %6d"), sThumbX);
        RECT rtText = GetItemRect(IDC_STATIC_RIGHT_STICK_X);
        ::FillRect(hdc, &rtText, hBkgBrush);
        ::TextOut(hdc, rtText.left, rtText.top, strValue.c_str(), strValue.size());

        strValue = CStrUtils::Format(_T("Y: %6d"), -sThumbY);
        rtText = GetItemRect(IDC_STATIC_RIGHT_STICK_Y);
        ::FillRect(hdc, &rtText, hBkgBrush);
        ::TextOut(hdc, rtText.left, rtText.top, strValue.c_str(), strValue.size());
    }

    if (wID == IDC_BUTTON_LEFTSTICK)
    {
        RECT rtLast = {};
        rtLast.left = rcItem.left + nWidth / 2 + (double)m_sThumbLX_Min / (double)32768 * nWidth / 2;
        rtLast.top = rcItem.top + nHeight / 2 + (double)m_sThumbLY_Min / (double)32768 * nHeight / 2;
        rtLast.right = rcItem.left + nWidth / 2 + (double)m_sThumbLX_Max / (double)32768 * nWidth / 2 + 1;
        rtLast.bottom = rcItem.top + nWidth / 2 + (double)m_sThumbLY_Max / (double)32768 * nHeight / 2 + 1;

        ::FillRect(hdc, &rtLast, hLastValueBrush);
    }

    if (wID == IDC_BUTTON_RIGHTSTICK)
    {
        RECT rtLast = {};
        rtLast.left = rcItem.left + nWidth / 2 + (double)m_sThumbRX_Min / (double)32768 * nWidth / 2;
        rtLast.top = rcItem.top + nHeight / 2 + (double)m_sThumbRY_Min / (double)32768 * nHeight / 2;
        rtLast.right = rcItem.left + nWidth / 2 + (double)m_sThumbRX_Max / (double)32768 * nWidth / 2 + 1;
        rtLast.bottom = rcItem.top + nWidth / 2 + (double)m_sThumbRY_Max / (double)32768 * nHeight / 2 + 1;

        ::FillRect(hdc, &rtLast, hLastValueBrush);
    }

    {
        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
        HPEN hOldPen = (HPEN)::SelectObject(hdc, m_wFocusID == wID ? GetStockObject(BLACK_PEN) :  GetStockObject(WHITE_PEN));
        ::Ellipse(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
        ::SelectObject(hdc, hOldBrush);
        ::SelectObject(hdc, hOldPen);
    }

    {

        HPEN hOldPen = (HPEN)::SelectObject(hdc, GetStockObject(WHITE_PEN));
        ::MoveToEx(hdc, rcItem.left, rcItem.top + nHeight / 2, nullptr);
        ::LineTo(hdc, rcItem.right, rcItem.top + nHeight / 2);

        ::MoveToEx(hdc, rcItem.left + nWidth/ 2, rcItem.top, nullptr);
        ::LineTo(hdc, rcItem.left + nWidth/ 2, rcItem.bottom);
        ::SelectObject(hdc, hOldPen);
    }

    RECT rcThumb = rcItem;
    LONG nX = (double)sThumbX / (double)32768 * nWidth / 2;
    LONG nY = (double)sThumbY / (double)32768 * nHeight / 2;

    rcThumb.right = rcThumb.left + nX;
    rcThumb.bottom = rcThumb.top + nY;

    if (rcThumb.right < rcThumb.left)
    {
        LONG tmp = rcThumb.right;
        rcThumb.right = rcThumb.left;
        rcThumb.left = tmp;
    }

    if (rcThumb.bottom < rcThumb.top)
    {
        LONG tmp = rcThumb.bottom;
        rcThumb.bottom = rcThumb.top;
        rcThumb.top = tmp;
    }

    ::OffsetRect(&rcThumb, nWidth / 2, nHeight / 2);
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, VALUE_COLOR);
        HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hValueBrush);

        rcThumb.right += 1;
        rcThumb.bottom += 1;

        ::Rectangle(hdc, rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);

        ::SelectObject(hdc, hOldBrush);
        ::SelectObject(hdc, hOldPen);
        ::DeleteObject(hPen);
    }

    ::DeleteObject(hLastBrush);
    ::DeleteObject(hValueBrush);
    ::DeleteObject(hLastValueBrush);
    ::DeleteObject(hNormalBrush);
    ::DeleteObject(hActiveBrush);
}

void CMainFrame::DrawTriggerButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID)
{
    HBRUSH hBkgBrush = m_hBgBrush;
    HBRUSH hNormalBrush = ::CreateSolidBrush(NORMAL_COLOR);
    HBRUSH hActiveBrush = ::CreateSolidBrush(ACTIVE_COLOR);
    HBRUSH hValueBrush = ::CreateSolidBrush(VALUE_COLOR);
    HBRUSH hLastBrush = ::CreateSolidBrush(LAST_BUTTON_COLOR);

    HDC hdc = lpDrawItem->hDC;
    RECT rcItem = lpDrawItem->rcItem;

    BYTE bTrigger = 0;
    BYTE bLastTrigger = 0;

    if (wID == IDC_BUTTON_LT)
    {
        bTrigger = m_xCurStateMixed.Gamepad.bLeftTrigger;
        bLastTrigger = m_lastState.Gamepad.bLeftTrigger;
    }

    if (wID == IDC_BUTTON_RT)
    {
        bTrigger = m_xCurStateMixed.Gamepad.bRightTrigger;
        bLastTrigger = m_lastState.Gamepad.bRightTrigger;
    }

    _tstring strText = GetItemString(wID);

    ::SetBkMode(hdc, TRANSPARENT);
    ::FillRect(hdc, &rcItem, hNormalBrush);

    {
        RECT rcTrigger = rcItem;
        rcTrigger.top = rcItem.bottom - ((rcItem.bottom - rcItem.top) * (double)((double)bLastTrigger / (double)255));
        ::FillRect(hdc, &rcTrigger, hLastBrush);
    }

    {
        RECT rcTrigger = rcItem;
        rcTrigger.top = rcItem.bottom - ((rcItem.bottom - rcItem.top) * (double)((double)bTrigger / (double)255));
        ::FillRect(hdc, &rcTrigger, hActiveBrush);
    }

    ::FrameRect(hdc, &rcItem, hBkgBrush);
    ::DrawText(hdc, strText.c_str(), strText.size(), &rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    _tstring strValue = CStrUtils::Format(_T("%d"), bTrigger);
    rcItem.bottom -= 4;
    ::DrawText(hdc, strValue.c_str(), strValue.size(), &rcItem, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);

    {
        rcItem = lpDrawItem->rcItem;
        ::FrameRect(hdc, &rcItem, wID == m_wFocusID ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH));
    }

    ::DeleteObject(hLastBrush);
    ::DeleteObject(hValueBrush);
    ::DeleteObject(hNormalBrush);
    ::DeleteObject(hActiveBrush);
}

void CMainFrame::DrawABXYButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID)
{
    HBRUSH hBkgBrush = m_hBgBrush;
    HBRUSH hNormalBrush = ::CreateSolidBrush(NORMAL_COLOR);
    HBRUSH hActiveBrush = ::CreateSolidBrush(ACTIVE_COLOR);
    HBRUSH hValueBrush = ::CreateSolidBrush(VALUE_COLOR);
    HBRUSH hLastBrush = ::CreateSolidBrush(LAST_BUTTON_COLOR);


    HDC hdc = lpDrawItem->hDC;
    RECT rcItem = lpDrawItem->rcItem;

    bool fHitState = 0;
    bool fLastState = 0;
    DWORD dwTextColor = RGB(0, 0, 0);

    if (wID == IDC_BUTTON_X)
    {
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_X;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
        dwTextColor = RGB(0, 51, 103);
    }

    if (wID == IDC_BUTTON_Y)
    {
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
        dwTextColor = RGB(255, 255, 0);
    }

    if (wID == IDC_BUTTON_B)
    {
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_B;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
        dwTextColor = RGB(255, 0, 0);
    }

    if (wID == IDC_BUTTON_A)
    {
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_A;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
        dwTextColor = RGB(0, 255, 0);
    }

    _tstring strText = GetItemString(wID);

    ::SetBkMode(hdc, TRANSPARENT);

    HPEN hOldPen = (HPEN)::SelectObject(hdc, wID == m_wFocusID ? GetStockObject(BLACK_PEN) : GetStockObject(WHITE_PEN));

    // 焦点状态
    if (fHitState)
    {
        // 绘制按键
        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hActiveBrush);
        ::Ellipse(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
        ::SelectObject(hdc, hOldBrush);
    }
    else
    {
        if (fLastState)
        {
            HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hLastBrush);
            ::Ellipse(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
            ::SelectObject(hdc, hOldBrush);
        }
        else
        {
            HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNormalBrush);
            ::Ellipse(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
            ::SelectObject(hdc, hOldBrush);
        }
    }
    ::SelectObject(hdc, hOldPen);

    // 绘制文本
    DWORD dwLastColor = ::SetTextColor(hdc, dwTextColor);
    ::DrawText(hdc, strText.c_str(), strText.size(), &rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ::SetTextColor(hdc, dwLastColor);

    ::DeleteObject(hLastBrush);
    ::DeleteObject(hValueBrush);
    ::DeleteObject(hNormalBrush);
    ::DeleteObject(hActiveBrush);
}

void CMainFrame::DrawDpadButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID)
{
    HBRUSH hBkgBrush = m_hBgBrush;
    HBRUSH hNormalBrush = ::CreateSolidBrush(NORMAL_COLOR);
    HBRUSH hActiveBrush = ::CreateSolidBrush(ACTIVE_COLOR);
    HBRUSH hValueBrush = ::CreateSolidBrush(VALUE_COLOR);
    HBRUSH hLastBrush = ::CreateSolidBrush(LAST_BUTTON_COLOR);


    HDC hdc = lpDrawItem->hDC;
    RECT rcItem = lpDrawItem->rcItem;
    LONG nWidth = rcItem.right - rcItem.left;
    LONG nHeight = rcItem.bottom - rcItem.top;


    POINT ptButton[3] = { 0 };
    bool fHitState = 0;
    bool fLastState = 0;

    if (wID == IDC_BUTTON_DPAD_UP)
    {
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;

        ptButton[0] = {rcItem.left + nWidth / 4, rcItem.bottom - nHeight / 4};
        ptButton[1] = {rcItem.right - nWidth / 4, rcItem.bottom - nHeight / 4};
        ptButton[2] = {rcItem.left + nWidth / 2, rcItem.top + nHeight / 3};
    }

    if (wID == IDC_BUTTON_DPAD_DOWN)
    { 
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;

        ptButton[0] = {rcItem.left + nWidth / 4, rcItem.top + nHeight / 4};
        ptButton[1] = {rcItem.right - nWidth / 4, rcItem.top + nHeight / 4};
        ptButton[2] = {rcItem.left + nWidth / 2, rcItem.bottom - nHeight / 3};
    }

    if (wID == IDC_BUTTON_DPAD_LEFT)
    { 
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;

        ptButton[0] = {rcItem.right - nWidth / 4, rcItem.top + nHeight / 4};
        ptButton[1] = {rcItem.right - nWidth / 4, rcItem.bottom - nHeight / 4};
        ptButton[2] = {rcItem.left + nWidth / 3, rcItem.top + nHeight / 2};
    }

    if (wID == IDC_BUTTON_DPAD_RIGHT)
    { 
        fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
        fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

        ptButton[0] = {rcItem.left + nWidth / 4, rcItem.top + nHeight / 4};
        ptButton[1] = {rcItem.left + nWidth / 4, rcItem.bottom - nHeight / 4};
        ptButton[2] = {rcItem.right - nWidth / 3, rcItem.top + nHeight / 2};
    }

    _tstring strText = GetItemString(wID);

    ::SetBkMode(hdc, TRANSPARENT);

    // 焦点状态
    if (fHitState)
    {
        ::FillRect(hdc, &rcItem, hActiveBrush);
        ::FrameRect(hdc, &rcItem, hBkgBrush);
    }
    else
    {
        if (fLastState)
        {
            ::FillRect(hdc, &rcItem, hLastBrush);
            ::FrameRect(hdc, &rcItem, hBkgBrush);
        }
        else
        {
            ::FillRect(hdc, &rcItem, hNormalBrush);
            ::FrameRect(hdc, &rcItem, hBkgBrush);
        }
    }

    {
        // 绘制按键
        HPEN hPen = CreatePen(PS_NULL, 1, VALUE_COLOR);
        HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
        ::Polygon(hdc, ptButton, _countof(ptButton));
        ::SelectObject(hdc, hOldPen);
        ::DeleteObject(hPen);
    }

    ::FrameRect(hdc, &rcItem, wID == m_wFocusID ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH));

    ::DeleteObject(hLastBrush);
    ::DeleteObject(hValueBrush);
    ::DeleteObject(hNormalBrush);
    ::DeleteObject(hActiveBrush);
}

void CMainFrame::DrawNormalButton(LPDRAWITEMSTRUCT lpDrawItem, WORD wID)
{
    HBRUSH hBkgBrush = m_hBgBrush;
    HBRUSH hNormalBrush = ::CreateSolidBrush(NORMAL_COLOR);
    HBRUSH hActiveBrush = ::CreateSolidBrush(ACTIVE_COLOR);
    HBRUSH hValueBrush = ::CreateSolidBrush(VALUE_COLOR);
    HBRUSH hLastBrush = ::CreateSolidBrush(LAST_BUTTON_COLOR);


    HDC hdc = lpDrawItem->hDC;
    RECT rcItem = lpDrawItem->rcItem;
    LONG nWidth = rcItem.right - rcItem.left;
    LONG nHeight = rcItem.bottom - rcItem.top;

    bool fHitState = 0;
    bool fLastState = 0;

    if (wID == IDC_BUTTON_LS) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    if (wID == IDC_BUTTON_RS) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
    if (wID == IDC_BUTTON_X) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_X;
    if (wID == IDC_BUTTON_Y) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
    if (wID == IDC_BUTTON_B) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_B;
    if (wID == IDC_BUTTON_A) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_A;
    if (wID == IDC_BUTTON_LB) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    if (wID == IDC_BUTTON_RB) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
    if (wID == IDC_BUTTON_BACK) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
    if (wID == IDC_BUTTON_GUIDE) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_GUIDE;
    if (wID == IDC_BUTTON_START) fHitState = m_xCurStateMixed.Gamepad.wButtons & XINPUT_GAMEPAD_START;

    if (wID == IDC_BUTTON_LS) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    if (wID == IDC_BUTTON_RS) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
    if (wID == IDC_BUTTON_X) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
    if (wID == IDC_BUTTON_Y) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
    if (wID == IDC_BUTTON_B) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
    if (wID == IDC_BUTTON_A) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
    if (wID == IDC_BUTTON_LB) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    if (wID == IDC_BUTTON_RB) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
    if (wID == IDC_BUTTON_BACK) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
    if (wID == IDC_BUTTON_GUIDE) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_GUIDE;
    if (wID == IDC_BUTTON_START) fLastState = m_lastState.Gamepad.wButtons & XINPUT_GAMEPAD_START;

    _tstring strText = GetItemString(wID);

    ::SetBkMode(hdc, TRANSPARENT);

    HBRUSH hFillBrush = hActiveBrush;
    // 焦点状态
    if (fHitState)
    {
        hFillBrush = hActiveBrush;
    }
    else
    {
        if (fLastState)
        {
            hFillBrush = hLastBrush;
        }
        else
        {
            hFillBrush = hNormalBrush;
        }
    }

    // 焦点状态
    if (IDC_BUTTON_GUIDE != wID)
    {
        ::FillRect(hdc, &rcItem, hFillBrush);
        ::FrameRect(hdc, &rcItem, hBkgBrush);
    }

    if (IDC_BUTTON_GUIDE == wID)
    {
        // 绘制背景
        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hFillBrush);

        HPEN hOldPen = (HPEN)::SelectObject(hdc, wID == m_wFocusID ? GetStockObject(BLACK_PEN) : GetStockObject(WHITE_PEN));
        ::Ellipse(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
        ::SelectObject(hdc, hOldPen);

        ::SelectObject(hdc, hOldBrush);
        ::DrawText(hdc, strText.c_str(), strText.size(), &rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else if (IDC_BUTTON_BACK == wID)
    {
        RECT rtButton = rcItem;
        rtButton.top = rcItem.top + nHeight / 8;
        rtButton.bottom = rtButton.top + nHeight / 2;
        rtButton.left = rcItem.left + nWidth / 8;
        rtButton.right = rcItem.right - nWidth / 3;
        ::FillRect(hdc, &rtButton, GetStockBrush(WHITE_BRUSH));

        ::InflateRect(&rtButton, -4, -4);
        ::FillRect(hdc, &rtButton, hFillBrush);

        rtButton = rcItem;
        rtButton.top = rcItem.top + nHeight / 8;
        rtButton.bottom = rtButton.top + nHeight / 2;
        rtButton.left = rcItem.left + nWidth / 8;
        rtButton.right = rcItem.right - nWidth / 3;

        ::OffsetRect(&rtButton, nWidth / 8, nHeight / 8);
        ::FillRect(hdc, &rtButton, hFillBrush);

        ::OffsetRect(&rtButton, nWidth / 8, nHeight / 8);

        ::FillRect(hdc, &rtButton, GetStockBrush(WHITE_BRUSH));
        ::InflateRect(&rtButton, -4, -4);
        ::FillRect(hdc, &rtButton, hFillBrush);

        ::FrameRect(hdc, &rcItem, wID == m_wFocusID ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH));
    }
    else if (IDC_BUTTON_START == wID)
    {
        RECT rtButton = rcItem;
        rtButton.top = rcItem.top + nHeight / 6;
        rtButton.bottom = rtButton.top + nHeight / 8;
        rtButton.left = rcItem.left + nWidth / 8;
        rtButton.right = rcItem.right - nWidth / 8;
        ::FillRect(hdc, &rtButton, GetStockBrush(WHITE_BRUSH));

        ::OffsetRect(&rtButton, 0, nHeight / 4);
        ::FillRect(hdc, &rtButton, GetStockBrush(WHITE_BRUSH));

        ::OffsetRect(&rtButton, 0, nHeight / 4);
        ::FillRect(hdc, &rtButton, GetStockBrush(WHITE_BRUSH));

        ::FrameRect(hdc, &rcItem, wID == m_wFocusID ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH));
    }
    else
    {
        ::FrameRect(hdc, &rcItem, wID == m_wFocusID ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH));

        ::DrawText(hdc, strText.c_str(), strText.size(), &rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    ::DeleteObject(hLastBrush);
    ::DeleteObject(hValueBrush);
    ::DeleteObject(hNormalBrush);
    ::DeleteObject(hActiveBrush);
}

LRESULT CMainFrame::OnDrawButton(WORD wNotify, WORD wID, HWND hWnd)
{
    DRAWITEMSTRUCT DrawItem = { 0 };
    DrawItem.CtlID = wID;
    OnDrawItem(0, (LPARAM)&DrawItem);

    return 0;
}

bool CMainFrame::OnChangeDpadUp(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
    }

    DrawButtonItem(IDC_BUTTON_DPAD_UP);
    DoFunction(IDC_BUTTON_DPAD_UP, state);

    return true;
}

bool CMainFrame::OnChangeDpadDown(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    }

    DrawButtonItem(IDC_BUTTON_DPAD_DOWN);
    DoFunction(IDC_BUTTON_DPAD_DOWN, state);

    return true;
}

bool CMainFrame::OnChangeDpadLeft(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    }

    DrawButtonItem(IDC_BUTTON_DPAD_LEFT);
    DoFunction(IDC_BUTTON_DPAD_LEFT, state);

    return true;
}

bool CMainFrame::OnChangeDpadRight(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
    }

    DrawButtonItem(IDC_BUTTON_DPAD_RIGHT);
    DoFunction(IDC_BUTTON_DPAD_RIGHT, state);

    return true;
}

bool CMainFrame::OnChangeBack(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_BACK)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_BACK;
    }

    DrawButtonItem(IDC_BUTTON_BACK);
    DoFunction(IDC_BUTTON_BACK, state);

    return true;
}

bool CMainFrame::OnChangeStart(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_START)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_START;
    }

    DrawButtonItem(IDC_BUTTON_START);
    DoFunction(IDC_BUTTON_START, state);

    return true;
}

bool CMainFrame::OnChangeGuide(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_GUIDE)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_GUIDE;
    }

    DrawButtonItem(IDC_BUTTON_GUIDE);
    ::Sleep(100);
    DoFunction(IDC_BUTTON_GUIDE, state);

    return true;
}

bool CMainFrame::OnChangeA(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_A)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_A;
    }

    DrawButtonItem(IDC_BUTTON_A);
    DoFunction(IDC_BUTTON_A, state);

    return true;
}

bool CMainFrame::OnChangeB(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_B)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_B;
    }

    DrawButtonItem(IDC_BUTTON_B);
    DoFunction(IDC_BUTTON_B, state);

    return true;
}

bool CMainFrame::OnChangeX(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_X)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_X;
    }

    DrawButtonItem(IDC_BUTTON_X);
    DoFunction(IDC_BUTTON_X, state);

    return true;
}

bool CMainFrame::OnChangeY(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_Y)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_Y;
    }

    DrawButtonItem(IDC_BUTTON_Y);
    DoFunction(IDC_BUTTON_Y, state);

    return true;
}

bool CMainFrame::OnChangeLeftShoulder(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    }

    DrawButtonItem(IDC_BUTTON_LB);
    DoFunction(IDC_BUTTON_LB, state);

    return true;
}

bool CMainFrame::OnChangeRightShoulder(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
    }

    DrawButtonItem(IDC_BUTTON_RB);
    DoFunction(IDC_BUTTON_RB, state);

    return true;
}

bool CMainFrame::OnChangeLeftThumb(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    }

    DrawButtonItem(IDC_BUTTON_LS);
    DoFunction(IDC_BUTTON_LS, state);

    return true;
}

bool CMainFrame::OnChangeRightThumb(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
    {
        m_lastState.Gamepad.wButtons |= xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
    }

    DrawButtonItem(IDC_BUTTON_RS);
    DoFunction(IDC_BUTTON_RS, state);

    return true;
}

bool CMainFrame::OnChangeLeftTrigger(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.bLeftTrigger >= m_lastState.Gamepad.bLeftTrigger)
    {
        m_lastState.Gamepad.bLeftTrigger = xGamepad.bLeftTrigger;
    }

    DrawButtonItem(IDC_BUTTON_LT);
    if (m_fVibrationLeft)
    {
        m_Vibration.wLeftMotorSpeed = xGamepad.bLeftTrigger * 65536 / 256;
        ::XInputSetState(0, &m_Vibration);
    }

    DoFunction(IDC_BUTTON_LT, state);

    return true;
}

bool CMainFrame::OnChangeRightTrigger(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.bRightTrigger >= m_lastState.Gamepad.bRightTrigger)
    {
        m_lastState.Gamepad.bRightTrigger = xGamepad.bRightTrigger;
    }

    DrawButtonItem(IDC_BUTTON_RT);
    if (m_fVibrationRight)
    {
        m_Vibration.wRightMotorSpeed = xGamepad.bRightTrigger * 65536 / 256;
        ::XInputSetState(0, &m_Vibration);
    }

    DoFunction(IDC_BUTTON_RT, state);

    return true;
}

bool CMainFrame::OnChangeLeftStick(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.sThumbLX <= m_sThumbLX_Min)
    {
        m_sThumbLX_Min = xGamepad.sThumbLX;
    }

    if (xGamepad.sThumbLX >= m_sThumbLX_Max)
    {
        m_sThumbLX_Max = xGamepad.sThumbLX;
    }

    if (-xGamepad.sThumbLY <= m_sThumbLY_Min)
    {
        m_sThumbLY_Min = -xGamepad.sThumbLY;
    }

    if (-xGamepad.sThumbLY >= m_sThumbLY_Max)
    {
        m_sThumbLY_Max = -xGamepad.sThumbLY;
    }

    DrawButtonItem(IDC_BUTTON_LEFTSTICK);
    DrawButtonItem(IDC_BUTTON_LS);
    DoFunction(IDC_BUTTON_LEFTSTICK, state);

    return true;
}

bool CMainFrame::OnChangeRightStick(const XINPUT_GAMEPAD& xGamepad, const INPUT_EVENT_STATE& state)
{
    if (xGamepad.sThumbRX <= m_sThumbRX_Min)
    {
        m_sThumbRX_Min = xGamepad.sThumbRX;
    }

    if (xGamepad.sThumbRX >= m_sThumbRX_Max)
    {
        m_sThumbRX_Max = xGamepad.sThumbRX;
    }

    if (-xGamepad.sThumbRY <= m_sThumbRY_Min)
    {
        m_sThumbRY_Min = -xGamepad.sThumbRY;
    }

    if (-xGamepad.sThumbRY >= m_sThumbRY_Max)
    {
        m_sThumbRY_Max = -xGamepad.sThumbRY;
    }

    DrawButtonItem(IDC_BUTTON_RIGHTSTICK);
    DrawButtonItem(IDC_BUTTON_RS);
    DoFunction(IDC_BUTTON_RIGHTSTICK, state);

    return true;
}

void CMainFrame::InitList()
{
    for (int i = 0; i < _countof(g_ModifierName); i++)
    {
        ::SendMessage(m_hModifier, LB_ADDSTRING, 0, (LPARAM)g_ModifierName[i]);
    }

    for (int i = 0; i < _countof(g_KeysName); i++)
    {
        ::SendMessage(m_hKeys, LB_ADDSTRING, 0, (LPARAM)g_KeysName[i]);
    }

    for (int i = 0; i < _countof(g_MouseName); i++)
    {
        ::SendMessage(m_hMouse, LB_ADDSTRING, 0, (LPARAM)g_MouseName[i]);
    }
}

void CMainFrame::UpdateButtonFunction(WORD wID)
{
    eButtonFunction eType = eButtonFunction::eNone;

    ::EnableWindow(m_hModifier, false);
    ::EnableWindow(m_hKeys, false);
    ::EnableWindow(m_hMouse, false);

    if (IDC_RADIO_NONE == wID)
    {
        eType = eButtonFunction::eNone;
    }

    if (IDC_RADIO_KEY == wID)
    {
        eType = eButtonFunction::eKey;
        ::EnableWindow(m_hModifier, true);
        ::EnableWindow(m_hKeys, true);
    }

    if (IDC_RADIO_MOUSE == wID)
    {
        eType = eButtonFunction::eMouse;
        ::EnableWindow(m_hMouse, true);
    }

    if (0 == m_wFocusID)
    {
        return;
    }

    auto itFind = g_ButtonFunctionList.find(m_wFocusID);
    if (g_ButtonFunctionList.end() != itFind)
    {
        itFind->second.eType = eType;

    }
}

LRESULT CMainFrame::OnModifierChange(WORD wNotify, WORD wID, HWND hWnd)
{
    if (0 == m_wFocusID)
    {
        return TRUE;
    }

    auto itFind = g_ButtonFunctionList.find(m_wFocusID);
    if (itFind != g_ButtonFunctionList.end())
    {
        std::vector<int> vModifier;
        int nIndex = 0;
        for (const auto& item : g_ModifierName)
        {
            LRESULT bSel = ::SendMessage(m_hModifier, LB_GETSEL, nIndex, 0);
            if (bSel)
            {
                vModifier.push_back(nIndex);
            }

            nIndex++;
        }

        itFind->second.vModifier = vModifier;
    }

    return TRUE;
}

LRESULT CMainFrame::OnKeyChange(WORD wNotify, WORD wID, HWND hWnd)
{
    if (0 == m_wFocusID)
    {
        return TRUE;
    }

    auto itFind = g_ButtonFunctionList.find(m_wFocusID);
    if (itFind != g_ButtonFunctionList.end())
    {
        {
            std::vector<int> vModifier;
            int nIndex = 0;
            for (const auto& item : g_ModifierName)
            {
                LRESULT bSel = ::SendMessage(m_hModifier, LB_GETSEL, nIndex, 0);
                if (bSel)
                {
                    vModifier.push_back(nIndex);
                }

                nIndex++;
            }

            itFind->second.vModifier = vModifier;
        }
        {
            std::vector<int> vKeys;
            int nIndex = 0;
            for (const auto& item : g_KeysName)
            {
                LRESULT bSel = ::SendMessage(m_hKeys, LB_GETSEL, nIndex, 0);
                if (bSel)
                {
                    vKeys.push_back(nIndex);
                }

                nIndex++;
            }

            itFind->second.vKeys = vKeys;
        }
    }

    return TRUE;
}

LRESULT CMainFrame::OnMouseChange(WORD wNotify, WORD wID, HWND hWnd)
{
    if (0 == m_wFocusID)
    {
        return TRUE;
    }

    auto itFind = g_ButtonFunctionList.find(m_wFocusID);
    if (itFind != g_ButtonFunctionList.end())
    {
        std::vector<int> vMouse;
        int nIndex = 0;
        for (const auto& item : g_MouseName)
        {
            LRESULT bSel = ::SendMessage(m_hMouse, LB_GETSEL, nIndex, 0);
            if (bSel)
            {
                vMouse.push_back(nIndex);
            }

            nIndex++;
        }

        itFind->second.vMouse = vMouse;
    }

    return TRUE;
}

void CMainFrame::ShowButtonFunction(WORD wID)
{
    ::EnableWindow(m_hModifier, FALSE);
    ::EnableWindow(m_hKeys, FALSE);
    ::EnableWindow(m_hMouse, FALSE);

    if (0 == wID)
    {
    }
    else
    {
        auto itFind = g_ButtonFunctionList.find(wID);
        if (itFind != g_ButtonFunctionList.end())
        {
            CheckButton(IDC_RADIO_NONE, false);
            CheckButton(IDC_RADIO_KEY, false);
            CheckButton(IDC_RADIO_MOUSE, false);

            if (eButtonFunction::eNone == itFind->second.eType)
            {
                CheckButton(IDC_RADIO_NONE, true);
            }
            if (eButtonFunction::eKey == itFind->second.eType)
            {
                CheckButton(IDC_RADIO_KEY, true);
                ::EnableWindow(m_hModifier, TRUE);
                ::EnableWindow(m_hKeys, TRUE);
            }
            if (eButtonFunction::eMouse == itFind->second.eType)
            {
                CheckButton(IDC_RADIO_MOUSE, true);
                ::EnableWindow(m_hMouse, TRUE);
            }

            ::SendMessage(m_hModifier, LB_SETSEL, FALSE, -1);
            for (const auto& item : itFind->second.vModifier)
            {
                LRESULT bSel = ::SendMessage(m_hModifier, LB_SETSEL, TRUE, item);
            }

            ::SendMessage(m_hKeys, LB_SETSEL, FALSE, -1);
            for (const auto& item : itFind->second.vKeys)
            {
                LRESULT bSel = ::SendMessage(m_hKeys, LB_SETSEL, TRUE, item);
            }

            ::SendMessage(m_hMouse, LB_SETSEL, FALSE, -1);
            for (const auto& item : itFind->second.vMouse)
            {
                LRESULT bSel = ::SendMessage(m_hMouse, LB_SETSEL, TRUE, item);
            }
        }
        else
        {
            BUTTON_FUNCTION_INFO info;
            info.eType = eButtonFunction::eNone;

            g_ButtonFunctionList.insert(std::make_pair(wID, info));

            ::SendMessage(m_hKeys, LB_SETSEL, FALSE, -1);
            ::SendMessage(m_hMouse, LB_SETSEL, FALSE, -1);

            ::EnableWindow(m_hModifier, FALSE);
            ::EnableWindow(m_hKeys, FALSE);
            ::EnableWindow(m_hMouse, FALSE);
            CheckButton(IDC_RADIO_NONE, true);
            CheckButton(IDC_RADIO_KEY, false);
            CheckButton(IDC_RADIO_MOUSE, false);
        }
    }
}

void CMainFrame::SaveConfig()
{
    WORD wIDs[] = {
        IDC_BUTTON_LEFTSTICK        ,
        IDC_BUTTON_RIGHTSTICK       ,
        IDC_BUTTON_DPAD_UP          ,
        IDC_BUTTON_DPAD_DOWN        ,
        IDC_BUTTON_DPAD_LEFT        ,
        IDC_BUTTON_DPAD_RIGHT       ,
        IDC_BUTTON_Y                ,
        IDC_BUTTON_A                ,
        IDC_BUTTON_X                ,
        IDC_BUTTON_B                ,
        IDC_BUTTON_BACK             ,
        IDC_BUTTON_START            ,
        IDC_BUTTON_GUIDE            ,
        IDC_BUTTON_LB               ,
        IDC_BUTTON_RB               ,
        IDC_BUTTON_LT               ,
        IDC_BUTTON_RT               ,
    };

    for (auto item : wIDs)
    {
        auto itFind = g_ButtonFunctionList.find(item);
        if (itFind != g_ButtonFunctionList.end())
        {
            WORD wID = itFind->first;

            _tstring strModifier;
            _tstring strKey;
            _tstring strMouse;

            for (auto item : itFind->second.vModifier)
            {
                strModifier += CStrUtils::Format(_T("%d,"), item);
            }

            for (auto item : itFind->second.vKeys)
            {
                strKey += CStrUtils::Format(_T("%d,"), item);
            }

            for (auto item : itFind->second.vMouse)
            {
                strMouse += CStrUtils::Format(_T("%d,"), item);
            }

            m_config.SetNumber(CStrUtils::Format(_T("%04X"), wID),
                _T("type"), itFind->second.eType);

            m_config.SetString(CStrUtils::Format(_T("%04X"), wID),
                _T("modifier"), strModifier.c_str());

            m_config.SetString(CStrUtils::Format(_T("%04X"), wID),
                _T("keys"), strKey.c_str());

            m_config.SetString(CStrUtils::Format(_T("%04X"), wID),
                _T("mouse"), strMouse.c_str());

        }
    }
}

void CMainFrame::LoadConfig()
{
    WORD wIDs[] = {
        IDC_BUTTON_LEFTSTICK        ,
        IDC_BUTTON_RIGHTSTICK       ,
        IDC_BUTTON_DPAD_UP          ,
        IDC_BUTTON_DPAD_DOWN        ,
        IDC_BUTTON_DPAD_LEFT        ,
        IDC_BUTTON_DPAD_RIGHT       ,
        IDC_BUTTON_Y                ,
        IDC_BUTTON_A                ,
        IDC_BUTTON_X                ,
        IDC_BUTTON_B                ,
        IDC_BUTTON_BACK             ,
        IDC_BUTTON_START            ,
        IDC_BUTTON_GUIDE            ,
        IDC_BUTTON_LB               ,
        IDC_BUTTON_RB               ,
        IDC_BUTTON_LT               ,
        IDC_BUTTON_RT               ,
    };

    for (auto item : wIDs)
    {
        _tstring strModifier = m_config.GetString(CStrUtils::Format(_T("%04X"), item), _T("modifier"));
        _tstring strKey = m_config.GetString(CStrUtils::Format(_T("%04X"), item), _T("keys"));
        _tstring strMouse = m_config.GetString(CStrUtils::Format(_T("%04X"), item), _T("mouse"));

        std::vector<_tstring> vModifier = CStrUtils::SplitStr(strModifier, _T(","));
        std::vector<_tstring> vKeys = CStrUtils::SplitStr(strKey, _T(","));
        std::vector<_tstring> vMouse = CStrUtils::SplitStr(strMouse, _T(","));

        BUTTON_FUNCTION_INFO info;
        info.eType = (eButtonFunction)m_config.GetNumber(CStrUtils::Format(_T("%04X"), item),
            _T("type"), eButtonFunction::eNone);

        for (const auto& item : vModifier)
        {
            info.vModifier.push_back(_tcstol(item.c_str(), NULL, 10));
        }

        for (const auto& item : vKeys)
        {
            info.vKeys.push_back(_tcstol(item.c_str(), NULL, 10));
        }

        for (const auto& item : vMouse)
        {
            info.vMouse.push_back(_tcstol(item.c_str(), NULL, 10));
        }

        g_ButtonFunctionList.insert(std::make_pair(item, info));
    }
}

void CMainFrame::DoMouseMove(int x, int y)
{
    enum eDirectionType
    {
        eUp = 1,
        eDown = 2,
        eLeft = 4,
        eRight = 8,
    };

    int nStickType = 0;
    nStickType |= y > MOUSE_MOVE_THRESHOLD ? eUp : 0;
    nStickType |= y < -MOUSE_MOVE_THRESHOLD ? eDown : 0;

    nStickType |= x < -MOUSE_MOVE_THRESHOLD ? eLeft : 0;
    nStickType |= x > MOUSE_MOVE_THRESHOLD ? eRight : 0;


    float left = 0;
    float up = 0;
    float right = 0;
    float down = 0;

    int limit = (double)(32767 - MOUSE_MOVE_THRESHOLD);
    int speed_level = 10;
    int speed = limit / speed_level;
    int ThumbLX = x;
    int ThumbLY = y;

    if (ThumbLX < 0)
    {
        if (ThumbLX < -MOUSE_MOVE_THRESHOLD)
        {
            ThumbLX += MOUSE_MOVE_THRESHOLD;
            ThumbLX -= speed;
        }
        else
        {
            ThumbLX = 0;
        }
    }

    if (ThumbLX > 0)
    {
        if (ThumbLX > MOUSE_MOVE_THRESHOLD)
        {
            ThumbLX -= MOUSE_MOVE_THRESHOLD;
            ThumbLX += speed;
        }
        else
        {
            ThumbLX = 0;
        }
    }

    if (ThumbLY < 0)
    {
        if (ThumbLY < -MOUSE_MOVE_THRESHOLD)
        {
            ThumbLY += MOUSE_MOVE_THRESHOLD;
            ThumbLY -= speed;
        }
        else
        {
            ThumbLY = 0;
        }
    }

    if (ThumbLY > 0)
    {
        if (ThumbLY > MOUSE_MOVE_THRESHOLD)
        {
            ThumbLY -= MOUSE_MOVE_THRESHOLD;
            ThumbLY += speed;
        }
        else
        {
            ThumbLY = 0;
        }
    }

    if (ThumbLX < 0)
    {
        left = abs(ThumbLX / speed);
    }
    if (ThumbLX > 0)
    {
        right = abs(ThumbLX / speed);
    }

    if (ThumbLY < 0)
    {
        down = abs(ThumbLY / speed);
    }
    if (ThumbLY > 0)
    {
        up = abs(ThumbLY / speed);
    }

    float dx = (right - left);
    float dy = (down - up);

    CInputUtils::MouseMove(dx, dy, false);
}

void CMainFrame::DoMouseWheel(int x, int y)
{
    float left = 0;
    float up = 0;
    float right = 0;
    float down = 0;

    if (x < -MOUSE_SRCOLL_THRESHOLD)
    {
        left = 1;
    }
    if (x > MOUSE_SRCOLL_THRESHOLD)
    {
        right = 1;
    }

    if (y < -MOUSE_SRCOLL_THRESHOLD)
    {
        down = 1;
    }
    if (y > MOUSE_SRCOLL_THRESHOLD)
    {
        up = 1;
    }

    static float last_dx = 0;
    static float last_dy = 0;
    float dx = (right - left);
    float dy = (down - up);

    bool bHoriz = false;
    if (abs(dx) > abs(dy))
    {
        bHoriz = true;
    }

    last_dx = dx;
    last_dy = dy;

    if (0 != dx || 0 != dy)
    {
        CInputUtils::MouseWheel(-dy, dx);
    }
}

void CMainFrame::DoFunction(WORD wID, const INPUT_EVENT_STATE& state)
{
    auto itFInd = g_ButtonFunctionList.find(wID);
    if (itFInd != g_ButtonFunctionList.end())
    {
        if (IDC_BUTTON_LEFTSTICK == wID && eButtonFunction::eMouse == itFInd->second.eType)
        {
            bool fHasMouseMove = false;
            bool fHasMouseWheel = false;

            for (const auto& item: itFInd->second.vMouse)
            {
                if (item == eMouseType::eMouseMove)
                {
                    fHasMouseMove = true;
                }
                if (item == eMouseType::eMouseWheel)
                {
                    fHasMouseWheel = true;
                }
            }

            if (fHasMouseMove)
            {
                DoMouseMove(m_xCurStateMixed.Gamepad.sThumbLX, m_xCurStateMixed.Gamepad.sThumbLY);
            }

            if (fHasMouseWheel)
            {
                DoMouseWheel(m_xCurStateMixed.Gamepad.sThumbLX, m_xCurStateMixed.Gamepad.sThumbLY);
            }

            return;
        }

        if (IDC_BUTTON_RIGHTSTICK == wID && eButtonFunction::eMouse == itFInd->second.eType)
        {
            bool fHasMouseMove = false;
            bool fHasMouseWheel = false;

            for (const auto& item: itFInd->second.vMouse)
            {
                if (item == eMouseType::eMouseMove)
                {
                    fHasMouseMove = true;
                }
                if (item == eMouseType::eMouseWheel)
                {
                    fHasMouseWheel = true;
                }
            }

            if (fHasMouseMove)
            {
                DoMouseMove(m_xCurStateMixed.Gamepad.sThumbRX, m_xCurStateMixed.Gamepad.sThumbRY);
            }

            if (fHasMouseWheel)
            {
                DoMouseWheel(m_xCurStateMixed.Gamepad.sThumbRX, m_xCurStateMixed.Gamepad.sThumbRY);
            }

            return;
        }

        if (eButtonFunction::eMouse == itFInd->second.eType)
        {
            bool fHasMouseLeft = false;
            bool fHasMouseRight = false;
            bool fHasMouseMiddle = false;

            for (const auto& item: itFInd->second.vMouse)
            {
                if (item == eMouseType::eMouseLeft)
                {
                    fHasMouseLeft = true;
                }

                if (item == eMouseType::eMouseRight)
                {
                    fHasMouseRight = true;
                }

                if (item == eMouseType::eMouseMiddle)
                {
                    fHasMouseMiddle = true;
                }
            }

            if (fHasMouseLeft)
            {
                CInputUtils::MouseKey(eInputMouseButton::eMButton_Left, state.fDown, state.fUp);
            }

            if (fHasMouseRight)
            {
                CInputUtils::MouseKey(eInputMouseButton::eMButton_Right, state.fDown, state.fUp);
            }

            if (fHasMouseMiddle)
            {
                CInputUtils::MouseKey(eInputMouseButton::eMButton_Middle, state.fDown, state.fUp);
            }
        }

        if (eButtonFunction::eKey == itFInd->second.eType)
        {
            if (state.fDown)
            {
                for (const auto& item : itFInd->second.vModifier)
                {
                    CInputUtils::KeyboardByName(g_ModifierName[item], state.fDown, state.fUp);
                }

                for (const auto& item : itFInd->second.vKeys)
                {
                    CInputUtils::KeyboardByName(g_KeysName[item], state.fDown, state.fUp);
                }
            }

            if (state.fHold)
            {
                for (const auto& item : itFInd->second.vKeys)
                {
                    CInputUtils::KeyboardByName(g_KeysName[item], state.dwDuration > 500, state.fUp);
                }
            }

            if (state.fUp)
            {
                for (const auto& item : itFInd->second.vKeys)
                {
                    CInputUtils::KeyboardByName(g_KeysName[item], state.fDown, state.fUp);
                }

                for (const auto& item : itFInd->second.vModifier)
                {
                    CInputUtils::KeyboardByName(g_ModifierName[item], state.fDown, state.fUp);
                }
            }
        }
    }
}