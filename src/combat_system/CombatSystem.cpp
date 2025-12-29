#include "battleTest/CombatSystem.hpp"
#include <algorithm>

namespace battleTest {

CombatResult CombatSystem::attack(StatComponent &attacker, StatComponent &defender)
{
  CombatResult result{ 0.0, 0.0, false };

  // 공격력과 방어력 가져오기
  double attackPower = attacker.getStatOr(STAT_ATTACK, 0.0);
  double defensePower = defender.getStatOr(STAT_DEFENSE, 0.0);

  // 데미지 계산 (최소 1)
  double damage = std::max(1.0, attackPower - defensePower);
  result.damageDealt = damage;

  // 현재 HP 가져오기
  double currentHP = defender.getStatOr(STAT_HP, 0.0);

  // HP 감소
  double newHP = std::max(0.0, currentHP - damage);
  defender.setStat(STAT_HP, newHP);

  result.remainingHP = newHP;
  result.targetDefeated = (newHP <= 0.0);

  return result;
}

bool CombatSystem::canCombat(const StatComponent &entity1, const StatComponent &entity2)
{
  return entity1.hasStat(STAT_HP) && entity2.hasStat(STAT_HP);
}

bool CombatSystem::isAlive(const StatComponent &entity)
{
  auto hp = entity.getStat(STAT_HP);
  return hp.has_value() && hp.value() > 0.0;
}

}// namespace battleTest
