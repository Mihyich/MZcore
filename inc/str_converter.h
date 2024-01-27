#ifndef STR_CONVERTER_H
#define STR_CONVERTER_H

#include <Windows.h>
#include <string>

/// @brief Конвертация строки string в wstring
std::wstring string_to_wstring(const std::string *str);

#endif //STR_CONVERTER_H
