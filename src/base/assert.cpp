#include "base/assert.h"

namespace base
{

// 전역 핸들러 변수
static AssertHandler g_assertHandler = nullptr;

void SetAssertHandler(AssertHandler handler)
{
    g_assertHandler = handler;
}

void Check(const char* expr, const char* msg, const char* file, const char* func, int line)
{
    // 핸들러가 설정되어 있으면 호출
    if (g_assertHandler)
    {
        g_assertHandler(expr, msg, file, func, line);
    }
}

void Verify(const char* expr, const char* msg, const char* file, const char* func, int line)
{
    // 핸들러가 설정되어 있으면 호출
    if (g_assertHandler)
    {
        g_assertHandler(expr, msg, file, func, line);
    }
}

} // namespace base
