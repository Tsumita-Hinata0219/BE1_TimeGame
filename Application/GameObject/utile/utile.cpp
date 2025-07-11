#include "utile.h"

#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#endif

std::wstring ConvertString(const std::string& s) {
#ifdef _WIN32
    int len = MultiByteToWideChar(
        CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
    std::wstring ws(len, 0);
    MultiByteToWideChar(
        CP_UTF8, 0, s.data(), (int)s.size(), &ws[0], len);
    return ws;
#else
    // Windows以外ならとりあえずそのまま返す（必要に応じて修正）
    return std::wstring(s.begin(), s.end());
#endif
}

void WriteUtf8(const std::string& s) {
#ifdef _WIN32
    std::wstring ws = ConvertString(s);
    DWORD _;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE),
        ws.c_str(), (DWORD)ws.size(), &_, nullptr);
#else
    std::cout << s;
#endif
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}