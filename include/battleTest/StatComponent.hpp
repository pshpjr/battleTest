#pragma once

#include <map>
#include <string>
#include <optional>

namespace battleTest {

// 스탯 키 타입 - 향후 enum이나 다른 타입으로 변경 가능
using StatKey = std::string;

class StatComponent
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
