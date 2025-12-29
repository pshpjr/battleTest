#include <catch2/catch_test_macros.hpp>


#include <battleTest/sample_library.hpp>
#include <battleTest/StatComponent.hpp>
#include <battleTest/CombatSystem.hpp>


TEST_CASE("Factorials are computed", "[factorial]")
{
  REQUIRE(factorial(0) == 1);
  REQUIRE(factorial(1) == 1);
  REQUIRE(factorial(2) == 2);
  REQUIRE(factorial(3) == 6);
  REQUIRE(factorial(10) == 3628800);
}

TEST_CASE("StatComponent basic operations", "[stat]")
{
  using namespace battleTest;

  StatComponent stats;

  SECTION("Setting and getting stats")
  {
    stats.setStat("HP", 100.0);
    stats.setStat("Attack", 50.0);

    auto hp = stats.getStat("HP");
    REQUIRE(hp.has_value());
    REQUIRE(hp.value() == 100.0);

    auto attack = stats.getStat("Attack");
    REQUIRE(attack.has_value());
    REQUIRE(attack.value() == 50.0);
  }

  SECTION("Getting non-existent stat")
  {
    auto defense = stats.getStat("Defense");
    REQUIRE_FALSE(defense.has_value());

    double defaultValue = stats.getStatOr("Defense", 25.0);
    REQUIRE(defaultValue == 25.0);
  }

  SECTION("Checking stat existence")
  {
    stats.setStat("HP", 100.0);
    REQUIRE(stats.hasStat("HP"));
    REQUIRE_FALSE(stats.hasStat("Attack"));
  }

  SECTION("Removing stats")
  {
    stats.setStat("HP", 100.0);
    REQUIRE(stats.hasStat("HP"));

    bool removed = stats.removeStat("HP");
    REQUIRE(removed);
    REQUIRE_FALSE(stats.hasStat("HP"));

    bool removedAgain = stats.removeStat("HP");
    REQUIRE_FALSE(removedAgain);
  }

  SECTION("Adding to stats")
  {
    stats.setStat("HP", 100.0);
    stats.addToStat("HP", 50.0);

    auto hp = stats.getStat("HP");
    REQUIRE(hp.value() == 150.0);

    // Adding to non-existent stat creates it
    stats.addToStat("Mana", 100.0);
    auto mana = stats.getStat("Mana");
    REQUIRE(mana.value() == 100.0);
  }

  SECTION("Clearing all stats")
  {
    stats.setStat("HP", 100.0);
    stats.setStat("Attack", 50.0);
    stats.setStat("Defense", 25.0);

    stats.clearStats();

    REQUIRE_FALSE(stats.hasStat("HP"));
    REQUIRE_FALSE(stats.hasStat("Attack"));
    REQUIRE_FALSE(stats.hasStat("Defense"));
  }
}

TEST_CASE("CombatSystem basic combat", "[combat]")
{
  using namespace battleTest;

  StatComponent attacker;
  StatComponent defender;

  SECTION("Basic attack without defense")
  {
    attacker.setStat(CombatSystem::STAT_ATTACK, 50.0);
    defender.setStat(CombatSystem::STAT_HP, 100.0);
    defender.setStat(CombatSystem::STAT_DEFENSE, 0.0);

    auto result = CombatSystem::attack(attacker, defender);

    REQUIRE(result.damageDealt == 50.0);
    REQUIRE(result.remainingHP == 50.0);
    REQUIRE_FALSE(result.targetDefeated);

    auto defenderHP = defender.getStat(CombatSystem::STAT_HP);
    REQUIRE(defenderHP.value() == 50.0);
  }

  SECTION("Attack with defense")
  {
    attacker.setStat(CombatSystem::STAT_ATTACK, 50.0);
    defender.setStat(CombatSystem::STAT_HP, 100.0);
    defender.setStat(CombatSystem::STAT_DEFENSE, 20.0);

    auto result = CombatSystem::attack(attacker, defender);

    REQUIRE(result.damageDealt == 30.0);  // 50 - 20
    REQUIRE(result.remainingHP == 70.0);
    REQUIRE_FALSE(result.targetDefeated);
  }

  SECTION("Minimum damage is 1")
  {
    attacker.setStat(CombatSystem::STAT_ATTACK, 10.0);
    defender.setStat(CombatSystem::STAT_HP, 100.0);
    defender.setStat(CombatSystem::STAT_DEFENSE, 50.0);

    auto result = CombatSystem::attack(attacker, defender);

    REQUIRE(result.damageDealt == 1.0);  // Minimum damage
    REQUIRE(result.remainingHP == 99.0);
  }

  SECTION("Target defeated")
  {
    attacker.setStat(CombatSystem::STAT_ATTACK, 150.0);
    defender.setStat(CombatSystem::STAT_HP, 100.0);
    defender.setStat(CombatSystem::STAT_DEFENSE, 0.0);

    auto result = CombatSystem::attack(attacker, defender);

    REQUIRE(result.damageDealt == 150.0);
    REQUIRE(result.remainingHP == 0.0);
    REQUIRE(result.targetDefeated);

    REQUIRE_FALSE(CombatSystem::isAlive(defender));
  }
}

TEST_CASE("CombatSystem utility functions", "[combat]")
{
  using namespace battleTest;

  StatComponent entity1;
  StatComponent entity2;

  SECTION("canCombat checks")
  {
    REQUIRE_FALSE(CombatSystem::canCombat(entity1, entity2));

    entity1.setStat(CombatSystem::STAT_HP, 100.0);
    REQUIRE_FALSE(CombatSystem::canCombat(entity1, entity2));

    entity2.setStat(CombatSystem::STAT_HP, 100.0);
    REQUIRE(CombatSystem::canCombat(entity1, entity2));
  }

  SECTION("isAlive checks")
  {
    REQUIRE_FALSE(CombatSystem::isAlive(entity1));

    entity1.setStat(CombatSystem::STAT_HP, 100.0);
    REQUIRE(CombatSystem::isAlive(entity1));

    entity1.setStat(CombatSystem::STAT_HP, 0.0);
    REQUIRE_FALSE(CombatSystem::isAlive(entity1));

    entity1.setStat(CombatSystem::STAT_HP, -10.0);
    REQUIRE_FALSE(CombatSystem::isAlive(entity1));
  }
}
