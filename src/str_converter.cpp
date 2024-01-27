#include "str_converter.h"

std::wstring string_to_wstring(const std::string *str)
{
    int length = MultiByteToWideChar(CP_UTF8, 0, str->c_str(), -1, nullptr, 0);

    if (length == 0)
    {
        return L"";
    }

    std::wstring result(length, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str->c_str(), -1, &result[0], length);

    return result;
}
