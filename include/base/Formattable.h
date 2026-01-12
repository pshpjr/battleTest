// Formattable.h
#pragma once

#include <format>
#include <string>
#include <concepts>

// ToString() 메서드 체크용 concept
template<typename T>
concept HasToString = requires(const T& t) {
    { t.ToString() } -> std::convertible_to<std::string>;
};

#define DEFINE_FORMATTER(Type) \
static_assert(HasToString<Type>, \
#Type " must implement 'std::string ToString() const' method"); \
template<> \
struct std::formatter<Type> : std::formatter<std::string> { \
auto format(const Type& obj, auto& ctx) const { \
return std::formatter<std::string>::format(obj.ToString(), ctx); \
} \
}

