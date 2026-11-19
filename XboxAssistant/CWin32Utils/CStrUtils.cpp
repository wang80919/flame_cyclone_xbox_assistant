#include "CStrUtils.h"
#include <fstream>
#include <algorithm>
#include <locale>
#include <tchar.h>

template<typename charT>
struct TCharCmpEqual {
    explicit TCharCmpEqual(const std::locale& loc) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
    }
private:
    const std::locale& loc_;
};

template<typename T>
size_t CStrUtils::_FindNoCase(const T& str1, const T& str2)
{
    typename T::const_iterator it = std::search(str1.begin(), str1.end(),
        str2.begin(), str2.end(), TCharCmpEqual<typename T::value_type>(std::locale()));
    if (it != str1.end())
    {
        return it - str1.begin();
    }

    return _tstring::npos;
}

size_t CStrUtils::FindNoCaseA(const std::string& str1, const std::string& str2)
{
    return _FindNoCase(str1, str2);
}

size_t CStrUtils::FindNoCaseW(const std::wstring& str1, const std::wstring& str2)
{
    return _FindNoCase(str1, str2);
}

size_t CStrUtils::FindNoCase(const _tstring& str1, const _tstring& str2)
{
    return _FindNoCase(str1, str2);
}

std::string CStrUtils::FormatA(LPCSTR pFormat, ...)
{
    size_t nCchCount = MAX_PATH;
    std::string strResult(nCchCount, 0);
    va_list args;

    va_start(args, pFormat);

    do
    {
        //成功则赋值字符串并终止循环
        int nSize = _vsnprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
        if (-1 != nSize)
        {
            strResult.resize(nSize);
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

    return strResult;
}

std::wstring CStrUtils::FormatW(LPCWSTR pFormat, ...)
{
    size_t nCchCount = MAX_PATH;
    std::wstring strResult(nCchCount, 0);
    va_list args;

    va_start(args, pFormat);

    do
    {
        //格式化输出字符串
        int nSize = _vsnwprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
        if (-1 != nSize)
        {
            strResult.resize(nSize);
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

    return strResult;
}

_tstring CStrUtils::Format(LPCTSTR pFormat, ...)
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
            strResult.resize(nSize);
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

    return strResult;
}

void CStrUtils::OutputDebugA(LPCSTR pFormat, ...)
{
    size_t nCchCount = MAX_PATH;
    std::string strResult(nCchCount, 0);
    va_list args;

    va_start(args, pFormat);

    do
    {
        //格式化输出字符串
        int nSize = _vsnprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
        if (-1 != nSize)
        {
            ::OutputDebugStringA(strResult.c_str());
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

void CStrUtils::OutputDebugW(LPCWSTR pFormat, ...)
{
    size_t nCchCount = MAX_PATH;
    std::wstring strResult(nCchCount, 0);
    va_list args;

    va_start(args, pFormat);

    do
    {
        //格式化输出字符串
        int nSize = _vsnwprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
        if (-1 != nSize)
        {
            ::OutputDebugStringW(strResult.c_str());
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

void CStrUtils::OutputDebug(LPCTSTR pFormat, ...)
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
            ::OutputDebugString(strResult.c_str());
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

std::vector<std::string> CStrUtils::SplitStrA(const std::string& str, const std::string& delim)
{
    std::vector<std::string> vectorOut;
    size_t iStart = 0;
    size_t iEnd = 0;

    while ((iStart = str.find_first_not_of(delim, iEnd)) != std::string::npos)
    {
        iEnd = str.find(delim, iStart);
        vectorOut.push_back(str.substr(iStart, iEnd - iStart));
    }

    return vectorOut;
}

std::vector<std::wstring> CStrUtils::SplitStrW(const std::wstring& str, const std::wstring& delim)
{
    std::vector<std::wstring> vectorOut;
    size_t iStart = 0;
    size_t iEnd = 0;

    while ((iStart = str.find_first_not_of(delim, iEnd)) != std::wstring::npos)
    {
        iEnd = str.find(delim, iStart);
        vectorOut.push_back(str.substr(iStart, iEnd - iStart));
    }

    return vectorOut;
}

std::vector<_tstring> CStrUtils::SplitStr(const _tstring& str, const _tstring& delim)
{
#ifdef _UNICODE
    return SplitStrW(str, delim);
#else
    return SplitStrA(str, delim);
#endif
}

std::string& CStrUtils::ReplaceA(std::string& strSrc, const std::string& strFind, const std::string& strReplace, bool bCase/* = false*/)
{
    std::string strDest = strSrc;
    size_t nFind = strDest.find(strFind);

    if (bCase)
    {
        while (_tstring::npos != strDest.find(strFind))
        {
            strDest.replace(nFind, strFind.size(), strReplace);
        }
    }
    else
    {
        while (_tstring::npos != FindNoCaseA(strDest, strFind))
        {
            strDest.replace(nFind, strFind.size(), strReplace);
        } 
    }

    strSrc = strDest;
    return strSrc;
}

std::wstring& CStrUtils::ReplaceW(std::wstring& strSrc, const std::wstring& strFind, const std::wstring& strReplace, bool bCase/* = false*/)
{
    std::wstring strDest = strSrc;
    size_t nFind = 0;

    if (bCase)
    {
        while (_tstring::npos != (nFind = strDest.find(strFind)))
        {
            strDest.replace(nFind, strFind.size(), strReplace);
        };
    }
    else
    {
        while (_tstring::npos != (nFind = FindNoCaseW(strDest, strFind)))
        {
            strDest.replace(nFind, strFind.size(), strReplace);
        };
    }

    strSrc = strDest;
    return strSrc;
}

_tstring& CStrUtils::Replace(_tstring& strSrc, const _tstring& strFind, const _tstring& strReplace, bool bCase/* = false*/)
{
#ifdef _UNICODE
    return ReplaceW(strSrc, strFind, strReplace, bCase);
#else
    return ReplaceA(strSrc, strFind, strReplace, bCase);
#endif
}

int CStrUtils::CompareA(const std::string& strSrc, const std::string& strDest, bool bCase /* = true*/)
{
    if (bCase)
    {
        return strcmp(strSrc.c_str(), strDest.c_str());
    }

    return _stricmp(strSrc.c_str(), strDest.c_str());
}

int CStrUtils::CompareW(const std::wstring& strSrc, const std::wstring& strDest, bool bCase /* = true*/)
{
    if (bCase)
    {
        return wcscmp(strSrc.c_str(), strDest.c_str());
    }

    return _wcsicmp(strSrc.c_str(), strDest.c_str());
}

int CStrUtils::Compare(const _tstring& strSrc, const _tstring& strDest, bool bCase /* = true*/)
{
#ifdef _UNICODE
    return CompareW(strSrc, strDest, bCase);
#else
    return CompareA(strSrc, strDest, bCase);
#endif
}

std::string CStrUtils::FromFileA(const std::string& strFile)
{
    std::string strContent;

    std::ifstream inputFile(strFile.c_str(), std::ios::binary | std::ios::in);
    if (!inputFile.is_open())
    {
        return strContent;
    }

    //获取文件大小
    inputFile.seekg(0, std::ios::end);
    std::streamoff nSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::string strBuffer(nSize, 0);
    inputFile.read(&strBuffer[0], nSize);
    size_t nByteSize = (size_t)inputFile.gcount();
    if (nByteSize > 0)
    {
        strContent = strBuffer;
    }

    return strContent;
}

std::wstring CStrUtils::FromFileW(const std::wstring& strFile)
{
    std::wstring strContent;

    std::ifstream inputFile(strFile.c_str(), std::ios::binary | std::ios::in);
    if (!inputFile.is_open())
    {
        return strContent;
    }

    //获取文件大小
    inputFile.seekg(0, std::ios::end);
    std::streamoff nSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::wstring strBuffer(nSize / sizeof(wchar_t), 0);
    inputFile.read((char*)&strBuffer[0], nSize);
    size_t nByteSize = (size_t)inputFile.gcount();
    if (nByteSize > 0)
    {
        strContent = strBuffer;
    }

    inputFile.close();

    return strContent;
}

_tstring CStrUtils::FromFile(const _tstring& strFile)
{
#ifdef _UNICODE
    return FromFileW(strFile);
#else
    return FromFileA(strFile);
#endif
}

bool CStrUtils::ToFileA(const std::string& strFile, const std::string& str)
{
    std::ofstream outputFile(strFile.c_str(), std::ios::binary | std::ios::out);
    if (!outputFile.is_open())
    {
        return false;
    }

    outputFile.write(str.c_str(), str.size());
    outputFile.close();

    return true;
}

bool CStrUtils::ToFileW(const std::wstring& strFile, const std::wstring& str)
{
    std::ofstream outputFile(strFile.c_str(), std::ios::binary | std::ios::out);
    if (!outputFile.is_open())
    {
        return false;
    }

    outputFile.write((const char*)str.c_str(), str.size() * sizeof(wchar_t));
    outputFile.close();

    return true;
}

bool CStrUtils::ToFile(const _tstring& strFile, const _tstring& str)
{
#ifdef _UNICODE
    return ToFileW(strFile, str);
#else
    return ToFileA(strFile, str);
#endif
}

std::string CStrUtils::ToUpperStrA(const std::string& str)
{
    std::string strResult = str;
    std::transform(strResult.begin(), strResult.end(), strResult.begin(), [](char ch)
        {
            if ((ch >= 'a') && (ch <= 'z'))
            {
                ch = ch - ('a' - 'A');
            }

            return ch;
        }
    );

    return strResult;
}

std::wstring CStrUtils::ToUpperStrW(const std::wstring& str)
{
    std::wstring strResult = str;
    std::transform(strResult.begin(), strResult.end(), strResult.begin(), [](wchar_t ch)
        {
            if ((ch >= L'a') && (ch <= L'z'))
            {
                ch = ch - (L'a' - L'A');
            }

            return ch;
        }
    );

    return strResult;
}

_tstring CStrUtils::ToUpperStr(const _tstring& str)
{
#ifdef _UNICODE
    return ToUpperStrW(str);
#else
    return ToUpperStrA(str);
#endif
}

std::string CStrUtils::ToLowerStrA(const std::string& str)
{
    std::string strResult = str;
    std::transform(strResult.begin(), strResult.end(), strResult.begin(), [](char ch)
        {
            if ((ch >= 'A') && (ch <= 'Z'))
            {
                ch = ch + ('a' - 'A');
            }

            return ch;
        }
    );

    return strResult;
}

std::wstring CStrUtils::ToLowerStrW(const std::wstring& str)
{
    std::wstring strResult = str;
    std::transform(strResult.begin(), strResult.end(), strResult.begin(), [](wchar_t ch)
        {
            if ((ch >= L'A') && (ch <= L'Z'))
            {
                ch = ch + (L'a' - L'A');
            }

            return ch;
        }
    );

    return strResult;
}

_tstring CStrUtils::ToLowerStr(const _tstring& str)
{
#ifdef _UNICODE
    return ToLowerStrW(str);
#else
    return ToLowerStrA(str);
#endif
}

std::string CStrUtils::_WStrToMultiStr(UINT CodePage, const std::wstring& str)
{
    //计算缓冲区所需的字节长度
    int cbMultiByte = ::WideCharToMultiByte(CodePage, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
    std::string strResult(cbMultiByte, 0);

    //成功则返回写入到指示的缓冲区的字节数
    size_t nConverted = ::WideCharToMultiByte(CodePage, 0, str.c_str(), (int)str.size(), &strResult[0], (int)strResult.size(), NULL, NULL);

    //调整内容长度
    strResult.resize(nConverted);
    return strResult;
}

std::wstring CStrUtils::_MultiStrToWStr(UINT CodePage, const std::string& str)
{
    //计算缓冲区所需的字符长度
    int cchWideChar = ::MultiByteToWideChar(CodePage, 0, str.c_str(), -1, NULL, 0);
    std::wstring strResult(cchWideChar, 0);

    //成功则返回写入到指示的缓冲区的字符数
    size_t nConverted = ::MultiByteToWideChar(CodePage, 0, str.c_str(), (int)str.size(), &strResult[0], (int)strResult.size());

    //调整内容长度
    strResult.resize(nConverted);
    return strResult;
}

std::string CStrUtils::WStrToAStr(const std::wstring& str)
{
    return _WStrToMultiStr(CP_ACP, str);
}

std::string CStrUtils::WStrToU8Str(const std::wstring& str)
{
    return _WStrToMultiStr(CP_UTF8, str);
}

_tstring CStrUtils::WStrToTStr(const std::wstring& str)
{
#ifdef _UNICODE
    return str;
#else
    return _WStrToMultiStr(CP_ACP, str);
#endif
}

std::wstring CStrUtils::AStrToWStr(const std::string& str)
{
    return _MultiStrToWStr(CP_ACP, str);
}

std::string CStrUtils::AStrToU8Str(const std::string& str)
{
    return WStrToU8Str(AStrToWStr(str));
}

_tstring CStrUtils::AStrToTStr(const std::string& str)
{
#ifdef _UNICODE
    return _MultiStrToWStr(CP_ACP, str);
#else
    return str;
#endif
}

std::wstring CStrUtils::U8StrToWStr(const std::string& str)
{
    return _MultiStrToWStr(CP_UTF8, str);
}

std::string CStrUtils::U8StrToAStr(const std::string& str)
{
    return WStrToAStr(U8StrToWStr(str));
}

_tstring CStrUtils::U8StrToTStr(const std::string& str)
{
#ifdef _UNICODE
    return _MultiStrToWStr(CP_UTF8, str);
#else
    return WStrToAStr(U8StrToWStr(str));
#endif
}

std::string CStrUtils::TStrToAStr(const _tstring& str)
{
#ifdef _UNICODE
    return _WStrToMultiStr(CP_ACP, str);
#else
    return str;
#endif
}

std::wstring CStrUtils::TStrToWStr(const _tstring& str)
{
#ifdef _UNICODE
    return str;
#else
    return AStrToWStr(str);
#endif
}

std::string CStrUtils::TStrToU8Str(const _tstring& str)
{
#ifdef _UNICODE
    return WStrToU8Str(str);
#else
    return WStrToU8Str(AStrToWStr(str));
#endif
}

std::string CStrUtils::GetErrorMessageA(DWORD dwMessageId, DWORD dwLanguageId/* = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
{
    std::string strResult;

    LPSTR pBuffer = NULL;
    DWORD dwFlags =
        FORMAT_MESSAGE_ALLOCATE_BUFFER |    //函数分配一个足够大的缓冲区来保存格式化消息，并将指针放置在 pBuffer 指定的地址处分配的缓冲区
        FORMAT_MESSAGE_FROM_SYSTEM |        //函数应在系统消息表资源搜索请求的消息
        FORMAT_MESSAGE_IGNORE_INSERTS |     //将忽略消息定义中的插入序列（如 %1）
        FORMAT_MESSAGE_MAX_WIDTH_MASK;      //函数忽略消息定义文本中的常规

    ::FormatMessageA(
        dwFlags,            //格式设置选项以及如何解释 lpSource 参数
        NULL,               //消息定义的位置。 此参数的类型取决于 dwFlags 参数中的设置
        dwMessageId,        //请求的消息的消息标识符。 如果 dwFlags 包含 FORMAT_MESSAGE_FROM_STRING，则忽略此参数
        dwLanguageId,       //所请求消息 的语言标识符
        (LPSTR)&pBuffer,    //指向缓冲区的指针
        MAX_PATH,           //如果未设置 FORMAT_MESSAGE_ALLOCATE_BUFFER 标志，则此参数以 TCHAR 为单位指定输出缓冲区的大小。 如果设置了 FORMAT_MESSAGE_ALLOCATE_BUFFER ，则此参数指定要为输出缓冲区分配的最小 TCHAR 数
        NULL                //一个值数组，这些值用作格式化消息中的插入值。 格式字符串中的 %1 指示 Arguments 数组中的第一个值;%2 指示第二个参数;等等
    );

    if (NULL != pBuffer)
    {
        strResult = pBuffer;
        ::LocalFree(pBuffer);
    }

    return strResult;
}

std::wstring CStrUtils::GetErrorMessageW(DWORD dwMessageId, DWORD dwLanguageId/* = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
{
    std::wstring strResult;

    LPWSTR pBuffer = NULL;
    DWORD dwFlags =
        FORMAT_MESSAGE_ALLOCATE_BUFFER |    //函数分配一个足够大的缓冲区来保存格式化消息，并将指针放置在 pBuffer 指定的地址处分配的缓冲区
        FORMAT_MESSAGE_FROM_SYSTEM |        //函数应在系统消息表资源搜索请求的消息
        FORMAT_MESSAGE_IGNORE_INSERTS |     //将忽略消息定义中的插入序列（如 %1）
        FORMAT_MESSAGE_MAX_WIDTH_MASK;      //函数忽略消息定义文本中的常规

    ::FormatMessageW(
        dwFlags,            //格式设置选项以及如何解释 lpSource 参数
        NULL,               //消息定义的位置。 此参数的类型取决于 dwFlags 参数中的设置
        dwMessageId,        //请求的消息的消息标识符。 如果 dwFlags 包含 FORMAT_MESSAGE_FROM_STRING，则忽略此参数
        dwLanguageId,       //所请求消息 的语言标识符
        (LPWSTR)&pBuffer,   //指向缓冲区的指针
        MAX_PATH,           //如果未设置 FORMAT_MESSAGE_ALLOCATE_BUFFER 标志，则此参数以 TCHAR 为单位指定输出缓冲区的大小。 如果设置了 FORMAT_MESSAGE_ALLOCATE_BUFFER ，则此参数指定要为输出缓冲区分配的最小 TCHAR 数
        NULL                //一个值数组，这些值用作格式化消息中的插入值。 格式字符串中的 %1 指示 Arguments 数组中的第一个值;%2 指示第二个参数;等等
    );

    if (NULL != pBuffer)
    {
        strResult = pBuffer;
        ::LocalFree(pBuffer);
    }

    return strResult;
}

_tstring CStrUtils::GetErrorMessage(DWORD dwMessageId, DWORD dwLanguageId/* = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
{
#ifdef _UNICODE
    return GetErrorMessageW(dwMessageId, dwLanguageId);
#else
    return GetErrorMessageA(dwMessageId, dwLanguageId);
#endif
}

_tstring CStrUtils::FormatByteSize(
    uint64_t nBytesSize, 
    bool bSpace/* = true*/, 
    int nPrecision/* = 3*/, 
    bool bTruncate
)
{
    TCHAR szFormatBuf[MAX_PATH] = { 0 };
    TCHAR szResultBuf[MAX_PATH] = { 0 };
    _tstring strResult;
    uint64_t nCurUtilSize = 1;
    uint64_t nNextUtilSize = 1024;

    LPCTSTR strUtilName[] = {
        _T("Bytes"),
        _T("KB"),
        _T("MB"),
        _T("GB"),
        _T("TB"),
        _T("PB"),
        _T("EB"),
    };

    if (bTruncate)
    {
        ::_stprintf_s(szFormatBuf, _countof(szFormatBuf), _T("%%.%dlf"), 16);
    }
    else
    {
        ::_stprintf_s(szFormatBuf, _countof(szFormatBuf), _T("%%.%dlf"), nPrecision);
    }

    int nUtilIndex = 0;
    for (int i = 0; i < _countof(strUtilName); i++)
    {
        if ((nBytesSize >= nCurUtilSize && nBytesSize < nNextUtilSize) || 0 == nNextUtilSize || 0 == nBytesSize)
        {
            double lfResult = (double)nBytesSize / (double)nCurUtilSize;
            ::_stprintf_s(szResultBuf, _countof(szResultBuf), szFormatBuf, lfResult);
            strResult = szResultBuf;
            nUtilIndex = i;
            break;
        }

        nCurUtilSize *= 1024;
        nNextUtilSize *= 1024;
    }

    if (bTruncate)
    {
        size_t nDotPos = strResult.find(_T('.'));
        if (_tstring::npos != nDotPos)
        {
            strResult.resize(nDotPos + 1 + nPrecision);
        }
    }

    // 去除尾部的0
    size_t nLength = strResult.size();
    for (auto item = strResult.crbegin(); item != strResult.crend(); item++)
    {
        if (_T('.') == *item)
        {
            nLength--;
            break;
        }

        if (_T('0') != *item)
        {
            break;
        }

        nLength--;
    }

    strResult.resize(nLength);

    if (bSpace)
    {
        strResult += _T(" ");
    }

    strResult += strUtilName[nUtilIndex];
    
    return strResult;
}

void CStrUtils::ConsoleOutput(LPCTSTR pFormat, ...)
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