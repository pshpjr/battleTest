#pragma once
#include <functional>

namespace base
{

// Assert 핸들러 타입 정의
using AssertHandler = std::function<void(const char* expr, const char* msg, const char* file, const char* func, int line)>;

// Assert 핸들러 설정
void SetAssertHandler(AssertHandler handler);

// Check: Debug 빌드에서만 동작
void Check(const char* expr, const char* msg, const char* file, const char* func, int line);

// Verify: Release 빌드에서도 동작
void Verify(const char* expr, const char* msg, const char* file, const char* func, int line);

} // namespace base

// CHECK 매크로: Debug 빌드에서만 동작
#ifndef NDEBUG
#define CHECK(expr, msg) \
    do { \
        if (!(expr)) { \
            ::base::Check(#expr, msg, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#else
#define CHECK(expr, msg) ((void)0)
#endif

// VERIFY 매크로: 항상 동작
#define VERIFY(expr, msg) \
    do { \
        if (!(expr)) { \
            ::base::Verify(#expr, msg, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
