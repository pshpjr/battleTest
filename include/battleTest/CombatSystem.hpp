#pragma once

#include "battleTest/StatComponent.hpp"

namespace battleTest {

struct CombatResult
{
  double damageDealt;
  double remainingHP;
  bool targetDefeated;
};

class CombatSystem
{
public:
  // 공격자가 방어자를 공격
  // 데미지 = 공격자의 Attack - 방어자의 Defense (최소 1)
  // 방어자의 HP에서 데미지만큼 감소
  static CombatResult attack(StatComponent &attacker, StatComponent &defender);

  // 두 객체가 전투 가능한지 확인 (둘 다 HP를 가지고 있는지)
  static bool canCombat(const StatComponent &entity1, const StatComponent &entity2);

  // 엔티티가 살아있는지 확인 (HP > 0)
  static bool isAlive(const StatComponent &entity);

  // 기본 스탯 키
  inline static const StatKey STAT_HP = "HP";
  inline static const StatKey STAT_ATTACK = "Attack";
  inline static const StatKey STAT_DEFENSE = "Defense";
};

}// namespace battleTest
