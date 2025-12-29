#include "battleTest/CombatSystem.hpp"
#include <algorithm>

namespace battleTest {

CombatResult CombatSystem::attack(CombatStatus &attacker, CombatStatus &defender)
{
  CombatResult result{ 0.0, 0.0, false };

  auto &attackerStats = attacker.getStats();
  auto &defenderStats = defender.getStats();

  // 공격력과 방어력 가져오기
  double attackPower = attackerStats.getStatOr(STAT_ATTACK, 0.0);
  double defensePower = defenderStats.getStatOr(STAT_DEFENSE, 0.0);

  // 데미지 계산 (최소 1)
  double damage = std::max(1.0, attackPower - defensePower);
  result.damageDealt = damage;

  // 현재 HP 가져오기
  double currentHP = defenderStats.getStatOr(STAT_HP, 0.0);

  // HP 감소
  double newHP = std::max(0.0, currentHP - damage);
  defenderStats.setStat(STAT_HP, newHP);

  result.remainingHP = newHP;
  result.targetDefeated = (newHP <= 0.0);

  return result;
}

bool CombatSystem::canCombat(const CombatStatus &entity1, const CombatStatus &entity2)
{
  const auto &stats1 = entity1.getStats();
  const auto &stats2 = entity2.getStats();
  return stats1.hasStat(STAT_HP) && stats2.hasStat(STAT_HP);
}

bool CombatSystem::isAlive(const CombatStatus &entity)
{
  const auto &stats = entity.getStats();
  auto hp = stats.getStat(STAT_HP);
  return hp.has_value() && hp.value() > 0.0;
}

}// namespace battleTest
