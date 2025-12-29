#pragma once

#include "battleTest/StatTypes.hpp"
#include <optional>

namespace battleTest {

// 스탯 변경 이벤트 타입
enum class StatEventType
{
  Changed,// 스탯 값 변경
  Removed,// 스탯 제거
  Cleared // 모든 스탯 제거
};

// 스탯 이벤트 데이터
struct StatEvent
{
  StatEventType type;
  StatKey statName;
  std::optional<double> oldValue;// 이전 값 (없었으면 nullopt)
  std::optional<double> newValue;// 새 값 (제거되었으면 nullopt)

  // 스탯 변경 이벤트 생성
  static StatEvent changed(const StatKey &name, std::optional<double> oldVal, double newVal)
  {
    return StatEvent{ StatEventType::Changed, name, oldVal, newVal };
  }

  // 스탯 제거 이벤트 생성
  static StatEvent removed(const StatKey &name, double oldVal)
  {
    return StatEvent{ StatEventType::Removed, name, oldVal, std::nullopt };
  }

  // 모든 스탯 제거 이벤트 생성
  static StatEvent cleared() { return StatEvent{ StatEventType::Cleared, "", std::nullopt, std::nullopt }; }
};

}// namespace battleTest
