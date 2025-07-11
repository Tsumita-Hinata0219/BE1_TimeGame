#pragma once

#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


std::wstring ConvertString(const std::string& s);
void WriteUtf8(const std::string& s);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);