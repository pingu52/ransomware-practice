#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <string.h>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <stdlib.h>
#include <strsafe.h>
#include <locale.h> // for setlocale | selocale func encoding UTF-8
#pragma comment(lib, "User32.lib")

const TCHAR* path[] = {
    TEXT("C:\\Users\\user\\Documents"),
    TEXT("C:\\Users\\user\\Desktop"),
    TEXT("C:\\Users\\user\\Downloads")
};
#define Documents path[0]
#define Desktop path[1]
#define Downloads path[2]
#define FILE_ATTSTR_TXT L".txt"
#define FILE_ATTSTR_DOCX L".docx"

WCHAR* CharToWchar(const char* pstrSrc)
{
    assert(pstrSrc);
    int nLen = strlen(pstrSrc) + 1;

    WCHAR* pwstr = (LPWSTR)malloc(sizeof(WCHAR) * nLen);
    mbstowcs(pwstr, pstrSrc, nLen);

    return pwstr;
}

char* WCharToChar(const wchar_t* pwstrSrc)
{
    assert(pwstrSrc);

#if !defined _DEBUG
    int len = 0;
    len = (wcslen(pwstrSrc) + 1) * 2;
    char* pstr = (char*)malloc(sizeof(char) * len);

    WideCharToMultiByte(949, 0, pwstrSrc, -1, pstr, len, NULL, NULL);
#else

    int nLen = wcslen(pwstrSrc);

    char* pstr = (char*)malloc(sizeof(char) * nLen + 1);
    wcstombs(pstr, pwstrSrc, nLen + 1);
#endif

    return pstr;
}

BOOL IsDirectory(const TCHAR* path)
{
    WIN32_FIND_DATA findFileData;
    /*
        typedef struct _WIN32_FIND_DATAW {
          DWORD    dwFileAttributes;
          FILETIME ftCreationTime;
          FILETIME ftLastAccessTime;
          FILETIME ftLastWriteTime;
          DWORD    nFileSizeHigh;
          DWORD    nFileSizeLow;
          DWORD    dwReserved0;
          DWORD    dwReserved1;
          WCHAR    cFileName[MAX_PATH];
          WCHAR    cAlternateFileName[14];
          DWORD    dwFileType; // Obsolete. Do not use.
          DWORD    dwCreatorType; // Obsolete. Do not use
          WORD     wFinderFlags; // Obsolete. Do not use
        } WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
    */
    HANDLE findFile = FindFirstFile(path, &findFileData);
    // typedef void *HANDLE
    FindClose(findFile);

    return (BOOL)(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL FindDirectory(const TCHAR* path)
{
    WIN32_FIND_DATA ffd;
    /*
        typedef struct _WIN32_FIND_DATAW {
          DWORD    dwFileAttributes;
          FILETIME ftCreationTime;
          FILETIME ftLastAccessTime;
          FILETIME ftLastWriteTime;
          DWORD    nFileSizeHigh;
          DWORD    nFileSizeLow;
          DWORD    dwReserved0;
          DWORD    dwReserved1;
          WCHAR    cFileName[MAX_PATH];
          WCHAR    cAlternateFileName[14];
          DWORD    dwFileType; // Obsolete. Do not use.
          DWORD    dwCreatorType; // Obsolete. Do not use
          WORD     wFinderFlags; // Obsolete. Do not use
        } WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
    */
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    StringCchLength(path, 260, &length_of_arg);

    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    _tprintf(TEXT("\nTarget directory is %s\n\n"), path);

    StringCchCopy(szDir, MAX_PATH, path);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    hFind = FindFirstFile(szDir, &ffd);

    do
    {

        //if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        //{
        //    _tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
        //}

        //else
        if (wcsstr(ffd.cFileName, FILE_ATTSTR_TXT)
            || wcsstr(ffd.cFileName, FILE_ATTSTR_DOCX))
        {
            //getExt((char*)ffd.cFileName);
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            //_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
            _tprintf(TEXT("  %s\n"), ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);
}

int _tmain(int argc, TCHAR* argv[])
{
    setlocale(LC_ALL, "ko-KR");

    if (IsDirectory(Documents))
        FindDirectory(Documents);
    if (IsDirectory(Desktop))
        FindDirectory(Desktop);
    if (IsDirectory(Downloads))
        FindDirectory(Downloads);
    return (0);
}