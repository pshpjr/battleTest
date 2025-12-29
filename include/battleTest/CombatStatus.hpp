#pragma once

#include "battleTest/StatComponent.hpp"

namespace battleTest {

// 전투에 참여하는 엔티티의 상태를 나타내는 클래스
// StatComponent를 포함하며, 향후 버프/디버프, 스킬, 상태이상 등을 추가할 수 있음
class CombatStatus
{
public:
  explicit CombatStatus(StatComponent &stats);
  ~CombatStatus() = default;

  CombatStatus(const CombatStatus &) = delete;
  CombatStatus &operator=(const CombatStatus &) = delete;
  CombatStatus(CombatStatus &&) = default;
  CombatStatus &operator=(CombatStatus &&) = default;

  // 스탯 컴포넌트 접근
  [[nodiscard]] StatComponent &getStats() noexcept { return stats_; }
  [[nodiscard]] const StatComponent &getStats() const noexcept { return stats_; }

  // 향후 확장 가능한 기능들:
  // - 버프/디버프 관리
  // - 스킬 효과
  // - 상태이상 (기절, 독, 화상 등)
  // - 특수 능력 (회피율, 크리티컬 등)

private:
  StatComponent &stats_;
  // 향후 추가될 멤버들:
  // std::vector<Buff> buffs_;
  // std::vector<Debuff> debuffs_;
  // StatusEffects effects_;
};

}// namespace battleTest
