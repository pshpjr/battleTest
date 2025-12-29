#pragma once

#include "battleTest/Observer.hpp"
#include "battleTest/StatEvents.hpp"
#include "battleTest/StatTypes.hpp"
#include <map>
#include <optional>

namespace battleTest {

class StatComponent : public Subject<StatEvent>
{
public:
  StatComponent() = default;
  ~StatComponent() = default;

  StatComponent(const StatComponent &) = default;
  StatComponent &operator=(const StatComponent &) = default;
  StatComponent(StatComponent &&) = default;
  StatComponent &operator=(StatComponent &&) = default;

  // 스탯 추가 또는 수정
  void setStat(const StatKey &statName, double value);

  // 스탯 값 가져오기 (없으면 std::nullopt 반환)
  [[nodiscard]] std::optional<double> getStat(const StatKey &statName) const;

  // 스탯 값 가져오기 (없으면 defaultValue 반환)
  [[nodiscard]] double getStatOr(const StatKey &statName, double defaultValue) const;

  // 스탯 제거
  bool removeStat(const StatKey &statName);

  // 스탯이 존재하는지 확인
  [[nodiscard]] bool hasStat(const StatKey &statName) const;

  // 스탯 값 증가
  void addToStat(const StatKey &statName, double delta);

  // 모든 스탯 제거
  void clearStats();

  // 모든 스탯 가져오기
  [[nodiscard]] const std::map<StatKey, double> &getAllStats() const noexcept { return stats_; }

private:
  std::map<StatKey, double> stats_;
};

}// namespace battleTest
