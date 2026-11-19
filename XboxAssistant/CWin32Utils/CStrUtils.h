#pragma once

#include <wtypesbase.h>
#include <string>
#include <vector>
#include <tchar.h>

#ifdef _UNICODE
using _tstring = std::wstring;
#else
using _tstring = std::string;
#endif

// 字符串转换实用类
class CStrUtils
{
public:

    // 从文件载入字符串
    static std::string FromFileA(const std::string& strFile);
    static std::wstring FromFileW(const std::wstring& strFile);
    static _tstring FromFile(const _tstring& strFile);

    // 字符串保存到文件
    static bool ToFileA(const std::string& strFile, const std::string& str);
    static bool ToFileW(const std::wstring& strFile, const std::wstring& str);
    static bool ToFile(const _tstring& strFile, const _tstring& str);

    // 转换大写
    static std::string ToUpperStrA(const std::string& str);
    static std::wstring ToUpperStrW(const std::wstring& str);
    static _tstring ToUpperStr(const _tstring& str);

    // 转换小写
    static std::string ToLowerStrA(const std::string& str);
    static std::wstring ToLowerStrW(const std::wstring& str);
    static _tstring ToLowerStr(const _tstring& str);

    // 分割字符串
    static std::vector<std::string> SplitStrA(const std::string& str, const std::string& delim);
    static std::vector<std::wstring> SplitStrW(const std::wstring& str, const std::wstring& delim);
    static std::vector<_tstring> SplitStr(const _tstring& str, const _tstring& delim);

    // 替换字符串
    static std::string& ReplaceA(std::string& strSrc, const std::string& strFind, const std::string& strReplace, bool bCase = false);
    static std::wstring& ReplaceW(std::wstring& strSrc, const std::wstring& strFind, const std::wstring& strReplace, bool bCase = false);
    static _tstring& Replace(_tstring& strSrc, const _tstring& strFind, const _tstring& strReplace, bool bCase = false);

    // 比较字符串
    static int CompareA(const std::string& strSrc, const std::string& strDest, bool bCase = true);
    static int CompareW(const std::wstring& strSrc, const std::wstring& strDest, bool bCase = true);
    static int Compare(const _tstring& strSrc, const _tstring& strDest, bool bCase = true);

    // 格式化字符串
    static std::string FormatA(LPCSTR pFormat, ...);
    static std::wstring FormatW(LPCWSTR pFormat, ...);
    static _tstring Format(LPCTSTR pFormat, ...);

    // 输出调试字符串
    static void OutputDebugA(LPCSTR pFormat, ...);
    static void OutputDebugW(LPCWSTR pFormat, ...);
    static void OutputDebug(LPCTSTR pFormat, ...);

    // 查找字符串位置
    static size_t FindNoCaseA(const std::string& str1, const std::string& str2);
    static size_t FindNoCaseW(const std::wstring& str1, const std::wstring& str2);
    static size_t FindNoCase(const _tstring& str1, const _tstring& str2);

    // 宽字符串转换
    static std::string WStrToU8Str(const std::wstring& str);
    static std::string WStrToAStr(const std::wstring& str);
    static _tstring WStrToTStr(const std::wstring& str);

    // ANSI字符转换
    static std::wstring AStrToWStr(const std::string& str);
    static std::string AStrToU8Str(const std::string& str);
    static _tstring AStrToTStr(const std::string& str);

    // UTF-8字符串转换
    static std::wstring U8StrToWStr(const std::string& str);
    static std::string U8StrToAStr(const std::string& str);
    static _tstring U8StrToTStr(const std::string& str);

    // 中立字符串转换
    static std::string TStrToAStr(const _tstring& str);
    static std::wstring TStrToWStr(const _tstring& str);
    static std::string TStrToU8Str(const _tstring& str);

    // 获取GetLastError()的错误信息文本
    static std::string GetErrorMessageA(DWORD dwMessageId, DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
    static std::wstring GetErrorMessageW(DWORD dwMessageId, DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
    static _tstring GetErrorMessage(DWORD dwMessageId, DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));

    // 
    // @brief: 字节大小格式化
    // @param: nBytesSize       输入字节大小
    // @param: bSpace           输出是否需要空格
    // @param: nPrecision       精度(指定小数点后多少位数字)
    // @param: bTruncate        是否截断未显示的数字(true: 放弃未显示的数字 false: 舍入到最接近的显示数字)
    // @ret: 长路径
    static _tstring FormatByteSize(
        uint64_t nBytesSize, 
        bool bSpace = true, 
        int nPrecision = 3, 
        bool bTruncate = false
    );

    // 控制台输出
    static void ConsoleOutput(LPCTSTR pFormat, ...);

private:

    // 字符串查找
    template<typename T>
    static size_t _FindNoCase(const T& str1, const T& str2);

    // 宽字符转其他
    static std::string _WStrToMultiStr(UINT CodePage, const std::wstring& str);

    // 其他转宽字符
    static std::wstring _MultiStrToWStr(UINT CodePage, const std::string& str);
};
