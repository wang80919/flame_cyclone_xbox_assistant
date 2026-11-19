#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <string>

#pragma warning(disable:4100)

#ifdef UNICODE
using _tstring = std::wstring;
#else
using _tstring = std::string;
#endif

class CDialogBase;

typedef LRESULT(CDialogBase::* pMessageFn)(WPARAM wParam, LPARAM lParam);
typedef LRESULT(CDialogBase::* pCommandFn)(WORD wNotify, WORD wID, HWND hWnd);
typedef LRESULT(CDialogBase::* pNotifyFn)(WORD wID, LPNMHDR pNmhdr);

struct DLG_MSGMAP_ENTRY
{
    union
    {
        pMessageFn m_pFnMessage;
        pCommandFn m_pFnCommand;
        pNotifyFn m_pFnNotify;

    }m_pFunc;               //要调用的例程（或特殊值）
    UINT m_nMessage;        //Windows消息
    UINT m_nCode;           //控件码或 or WM_NOTIFY code
    UINT m_nID;             //控件ID (Windows消息则为0)
    UINT m_nLastID;         //用于指定控件 ID 范围的条目

    DLG_MSGMAP_ENTRY()
        :
        m_nMessage(0),
        m_nCode(0),
        m_nID(0),
        m_nLastID(0)
    {
        m_pFunc.m_pFnMessage = nullptr;
    }

    DLG_MSGMAP_ENTRY(UINT uMsg, pMessageFn pfn)
        :
        m_nMessage(uMsg),
        m_nCode(0),
        m_nID(0),
        m_nLastID(0)
    {
        m_pFunc.m_pFnMessage = pfn;
    }

    DLG_MSGMAP_ENTRY(UINT uMsg, UINT wNotifyCode, UINT id, UINT idLast, pNotifyFn pfn)
        :
        m_nMessage(uMsg),
        m_nCode(wNotifyCode),
        m_nID(id),
        m_nLastID(idLast)
    {
        m_pFunc.m_pFnNotify = pfn;
    }

    DLG_MSGMAP_ENTRY(UINT uMsg, UINT wNotifyCode, UINT id, UINT idLast, pCommandFn pfn)
        :
        m_nMessage(uMsg),
        m_nCode(wNotifyCode),
        m_nID(id),
        m_nLastID(idLast)
    {
        m_pFunc.m_pFnCommand = pfn;
    }
};

struct DLG_MSGMAP
{
    const DLG_MSGMAP* (*pfnGetBaseMap)();
    const DLG_MSGMAP_ENTRY* lpEntries;
};

#ifndef dlg_msg
#define dlg_msg
#endif

#ifndef dlg_command
#define dlg_command
#endif

#ifndef dlg_notify
#define dlg_notify
#endif

#define DECLARE_DLG_MESSAGE_MAP()                               \
protected:                                                      \
static const DLG_MSGMAP* GetThisMessageMap();                   \
virtual const DLG_MSGMAP* GetMessageMap() const;                \

#define BEGIN_DLG_MESSAGE_MAP(theClass, baseClass)              \
const DLG_MSGMAP* theClass::GetMessageMap() const               \
{                                                               \
    return GetThisMessageMap();                                 \
}                                                               \
const DLG_MSGMAP* theClass::GetThisMessageMap()                 \
{                                                               \
    typedef baseClass TheBaseClass;                                \
static const DLG_MSGMAP_ENTRY _messageEntries[] =               \
{                                                               \

#define ON_DLG_MESSAGE(_msg, _Fn)                               \
DLG_MSGMAP_ENTRY(_msg, static_cast< LRESULT (CDialogBase::*)(WPARAM, LPARAM) >(_Fn)),

#define ON_DLG_COMMAND(_id, _Fn)                                \
DLG_MSGMAP_ENTRY(WM_COMMAND, 0, _id, _id, static_cast< LRESULT (CDialogBase::*)(WORD wNotify, WORD wID, HWND hWnd) >(_Fn)),

#define ON_DLG_COMMAND_NOTIFY(_wNotifycode, _id, _Fn)                                \
DLG_MSGMAP_ENTRY(WM_COMMAND, (UINT)_wNotifycode, _id, _id,  static_cast< LRESULT (CDialogBase::*)(WORD wNotify, WORD wID, HWND hWnd) >(_Fn)),

#define ON_DLG_COMMAND_RANGE(_id, _idLast, _Fn)                 \
DLG_MSGMAP_ENTRY(WM_COMMAND, 0, _id, _idLast, static_cast< LRESULT (CDialogBase::*)(WORD wNotify, WORD wID, HWND hWnd) >(_Fn)),

#define ON_DLG_NOTIFY(_wNotifycode, _id, _Fn)                   \
DLG_MSGMAP_ENTRY(WM_NOTIFY, (UINT)_wNotifycode, _id, _id,  static_cast< LRESULT (CDialogBase::*)(WORD wID, LPNMHDR pNmhdr) >(_Fn)),

#define ON_DLG_NOTIFY_RANGE(_wNotifycode, _id, _idLast, _Fn)    \
DLG_MSGMAP_ENTRY(WM_NOTIFY, (UINT)_wNotifycode, _id, _idLast,  static_cast< LRESULT (CDialogBase::*)(WORD wID, LPNMHDR pNmhdr) >(_Fn)),

#define END_DLG_MESSAGE_MAP()  DLG_MSGMAP_ENTRY(0, NULL) };     \
static const DLG_MSGMAP messageMap =                            \
{&TheBaseClass::GetThisMessageMap , &_messageEntries[0] };      \
    return &messageMap;                                         \
}                                                               \

#define DECLARE_DLG_BASE_MESSAGE_MAP()                          \
protected:                                                      \
static const DLG_MSGMAP* GetThisMessageMap();                   \
virtual const DLG_MSGMAP* GetMessageMap() const;                \
static const DLG_MSGMAP* GetBaseMessageMap();                   \

#define BEGIN_DLG_BASE_MESSAGE_MAP(theClass)                    \
typedef theClass TheThisClass;                                    \
const DLG_MSGMAP* TheThisClass::GetMessageMap() const           \
{                                                               \
    return GetThisMessageMap();                                 \
}                                                               \
const DLG_MSGMAP* TheThisClass::GetBaseMessageMap()             \
{                                                               \
    return NULL;                                                \
}                                                               \
const DLG_MSGMAP* TheThisClass::GetThisMessageMap()             \
{                                                               \
static const DLG_MSGMAP_ENTRY _messageEntries[] =               \
{                                                               \

#define END_DLG_BASE_MESSAGE_MAP()  DLG_MSGMAP_ENTRY(0, nullptr) }; \
static const DLG_MSGMAP messageMap =                            \
{&TheThisClass::GetBaseMessageMap, &_messageEntries[0] };       \
return &messageMap;                                             \
}                                                               \

class CDialogBase
{
public:
    CDialogBase();
    CDialogBase(const CDialogBase& r) = delete;
    CDialogBase& operator = (const CDialogBase& r) = delete;
    ~CDialogBase();

    INT_PTR DoModal(HWND hWndParent, bool bCenter = true);
    INT_PTR DoModal(UINT resourceID, HWND hWndParent, bool bCenter = true);
    INT_PTR DoModalEx(UINT resourceID, HWND hWndParent, BOOL isShow = TRUE, bool bCenter = true);
    HWND DoDialog(UINT resourceID, HWND hWndParent, BOOL isShow = TRUE, bool bCenter = true);

    static SIZE GetWindowCenterPos(const LPRECT pRtChild, HWND hParent);
    static void MoveWindowToCenter(HWND hWnd, HWND hParent);

public:
    dlg_msg LRESULT OnClose(WPARAM wParam, LPARAM lParam);

    dlg_msg LRESULT OnMsgTest(WPARAM wParam, LPARAM lParam);
    dlg_command LRESULT OnCommandTest(WORD wNotify, WORD wID, HWND hWnd);
    dlg_notify LRESULT OnNotifyTest(WORD wID, LPNMHDR pNmhdr);

    //命令消息处理
    virtual LRESULT OnCommand(WORD wNotify, WORD wID, HWND hWnd);

    //通知消息处理
    virtual LRESULT OnNotify(WORD wID, LPNMHDR pNmhdr);

public:

    //设置对话框资源
    void SetResourceID(UINT resourceID);

    //显示窗口
    void ShowWindow(int cmdShow = SW_SHOW);

    //检查窗口是否显示
    bool IsVisible() const;

    //移动窗口到父窗口中心
    void MoveToCenter(HWND hParent);

    //修改菜单文本
    void ModifyMenuText(HMENU hMnu, UINT uPosition, LPCTSTR lpNewItem, BOOL fByPos = TRUE);

    //修改菜单文本为资源字符串
    void ModifyMenuText(HMENU hMnu, UINT uPosition, UINT resourceID, BOOL fByPos = TRUE);

    //修改菜单状态
    void ModifyMenuState(HMENU hMnu, UINT uPosition, UINT fState = MFS_ENABLED, BOOL fByPos = TRUE);

    //获取窗口句柄
    HWND GetWndHandle() const;

    //结束对话框
    bool EndDialog(INT_PTR nResult);

    //消息
    bool PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam = 0);
    LRESULT SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam = 0);
    bool PostItemMessage(DWORD dwID, UINT uMsg, WPARAM wParam, LPARAM lParam = 0);
    LRESULT SendItemMessage(DWORD dwID, UINT uMsg, WPARAM wParam, LPARAM lParam = 0);
    bool PostCommand(WORD wID, WORD wNotify = 0, LPARAM lParam = 0);
    LRESULT SendCommand(WORD wID, WORD wNotify = 0, LPARAM lParam = 0);

    //加载资源字符串
    _tstring LoadString(UINT resourceID);

    //获取子项文本
    _tstring GetItemString(DWORD dwID);

    //设置子项文本
    void SetItemString(DWORD dwID, const _tstring& strText);

    //获取子项数值
    int GetItemInt(DWORD dwID, bool bSigned = true);

    //设置子项数值
    void SetItemInt(DWORD dwID, int val, bool bSigned = true);

    //检查子项按钮是否勾选
    bool IsButtonChecked(int nIDButton) const;

    //勾选子项按钮
    bool CheckButton(int nIDButton, bool bCheck = true);

    //获取子项窗口句柄
    HWND GetItem(DWORD dwID) const;

    //检查菜单项是否勾选
    bool IsMenuItemChecked(HMENU hMenu, UINT uId, bool fByPosition = false);

    //勾选菜单项
    void CheckMenuItem(HMENU hMenu, UINT uId, bool bCheck, bool fByPosition = false);

    //检查菜单项是否启用
    bool IsMenuItemEnable(HMENU hMenu, UINT uId, bool fByPosition = false);

    //启用菜单项
    void EnableMenuItem(HMENU hMenu, UINT uId, bool bCheck, bool fByPosition = false);

    //获取菜单项文本
    _tstring GetMenuItemString(HMENU hMenu, UINT uId, bool bCheck, bool fByPosition = false);

    //设置菜单项文本
    bool SetMenuItemString(HMENU hMenu, UINT uId, const _tstring strText, bool fByPosition = false);

    operator HWND() const
    {
        return m_hWnd;
    }

    // 加速键
    HACCEL GetAccel() const;
    bool LoadAccel(DWORD dwId);

protected:

    //假模态对话框
    dlg_msg LRESULT DoFakeModal(UINT resourceID, HWND hWndParent, BOOL isShow = TRUE);
    static INT_PTR WINAPI DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool BuildDcBitmap(HDC hdc, HDC* pDC, HBITMAP* pBitmap, int nW, int nH, DWORD dwColor = RGB(128,128,128));
    void DeleteDcBitmap(HDC* pDC, HBITMAP* pBitmap);

public:

    virtual BOOL PreTranslateMessage(LPMSG pMsg);
    virtual BOOL DialogMessage(LPMSG pMsg);

protected:
    HWND m_hWnd;
    UINT m_resID;
    BOOL m_bModel;
    BOOL m_bFakeModel;
    BOOL m_bCenter;
    BOOL m_bShow = true;

    HACCEL      m_hAccel;                           // 快捷键
    DECLARE_DLG_BASE_MESSAGE_MAP()
};