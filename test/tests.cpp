#include <catch2/catch_test_macros.hpp>


#include <battleTest/sample_library.hpp>
#include <battleTest/StatComponent.hpp>
#include <battleTest/CombatStatus.hpp>
#include <battleTest/CombatSystem.hpp>
#include <battleTest/Observer.hpp>
#include <battleTest/StatEvents.hpp>
#include <vector>


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

  StatComponent attackerStats;
  StatComponent defenderStats;

  SECTION("Basic attack without defense")
  {
    attackerStats.setStat(CombatSystem::STAT_ATTACK, 50.0);
    defenderStats.setStat(CombatSystem::STAT_HP, 100.0);
    defenderStats.setStat(CombatSystem::STAT_DEFENSE, 0.0);

    CombatStatus attacker(attackerStats);
    CombatStatus defender(defenderStats);

    auto result = CombatSystem::attack(attacker, defender);

    REQUIRE(result.damageDealt == 50.0);
    REQUIRE(result.remainingHP == 50.0);
    REQUIRE_FALSE(result.targetDefeated);

    auto defenderHP = defenderStats.getStat(CombatSystem::STAT_HP);
    REQUIRE(defenderHP.value() == 50.0);
  }

  SECTION("Attack with defense")
  {
    attackerStats.setStat(CombatSystem::STAT_ATTACK, 50.0);
    defenderStats.setStat(CombatSystem::STAT_HP, 100.0);
    defenderStats.setStat(CombatSystem::STAT_DEFENSE, 20.0);

    CombatStatus attacker(attackerStats);
    CombatStatus defender(defenderStats);

    auto result = CombatSystem::attack(attacker, defender);

    REQUIRE(result.damageDealt == 30.0);  // 50 - 20
    REQUIRE(result.remainingHP == 70.0);
    REQUIRE_FALSE(result.targetDefeated);
  }

  SECTION("Minimum damage is 1")
  {
    attackerStats.setStat(CombatSystem::STAT_ATTACK, 10.0);
    defenderStats.setStat(CombatSystem::STAT_HP, 100.0);
    defenderStats.setStat(CombatSystem::STAT_DEFENSE, 50.0);

    CombatStatus attacker(attackerStats);
    CombatStatus defender(defenderStats);

    auto result = CombatSystem::attack(attacker, defender);

    REQUIRE(result.damageDealt == 1.0);  // Minimum damage
    REQUIRE(result.remainingHP == 99.0);
  }

  SECTION("Target defeated")
  {
    attackerStats.setStat(CombatSystem::STAT_ATTACK, 150.0);
    defenderStats.setStat(CombatSystem::STAT_HP, 100.0);
    defenderStats.setStat(CombatSystem::STAT_DEFENSE, 0.0);

    CombatStatus attacker(attackerStats);
    CombatStatus defender(defenderStats);

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

  StatComponent stats1;
  StatComponent stats2;

  SECTION("canCombat checks")
  {
    CombatStatus entity1(stats1);
    CombatStatus entity2(stats2);

    REQUIRE_FALSE(CombatSystem::canCombat(entity1, entity2));

    stats1.setStat(CombatSystem::STAT_HP, 100.0);
    REQUIRE_FALSE(CombatSystem::canCombat(entity1, entity2));

    stats2.setStat(CombatSystem::STAT_HP, 100.0);
    REQUIRE(CombatSystem::canCombat(entity1, entity2));
  }

  SECTION("isAlive checks")
  {
    CombatStatus entity1(stats1);

    REQUIRE_FALSE(CombatSystem::isAlive(entity1));

    stats1.setStat(CombatSystem::STAT_HP, 100.0);
    REQUIRE(CombatSystem::isAlive(entity1));

    stats1.setStat(CombatSystem::STAT_HP, 0.0);
    REQUIRE_FALSE(CombatSystem::isAlive(entity1));

    stats1.setStat(CombatSystem::STAT_HP, -10.0);
    REQUIRE_FALSE(CombatSystem::isAlive(entity1));
  }
}
}

// 테스트용 옵저버 클래스
class TestStatObserver : public battleTest::Observer<battleTest::StatEvent>
{
public:
  std::vector<battleTest::StatEvent> events;

  void onNotify(const battleTest::StatEvent &event) override { events.push_back(event); }

  void clear() { events.clear(); }
};

TEST_CASE("Observer pattern for StatComponent", "[observer]")
{
  using namespace battleTest;

  StatComponent stats;
  TestStatObserver observer;

  SECTION("Observer receives stat changed events")
  {
    stats.addObserver(&observer);

    stats.setStat("HP", 100.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].type == StatEventType::Changed);
    REQUIRE(observer.events[0].statName == "HP");
    REQUIRE_FALSE(observer.events[0].oldValue.has_value());
    REQUIRE(observer.events[0].newValue.value() == 100.0);
  }

  SECTION("Observer receives multiple events")
  {
    stats.addObserver(&observer);

    stats.setStat("HP", 100.0);
    stats.setStat("Attack", 50.0);
    stats.setStat("HP", 150.0);

    REQUIRE(observer.events.size() == 3);

    // First event: HP set to 100
    REQUIRE(observer.events[0].statName == "HP");
    REQUIRE(observer.events[0].newValue.value() == 100.0);

    // Second event: Attack set to 50
    REQUIRE(observer.events[1].statName == "Attack");
    REQUIRE(observer.events[1].newValue.value() == 50.0);

    // Third event: HP changed from 100 to 150
    REQUIRE(observer.events[2].statName == "HP");
    REQUIRE(observer.events[2].oldValue.value() == 100.0);
    REQUIRE(observer.events[2].newValue.value() == 150.0);
  }

  SECTION("Observer receives addToStat events")
  {
    stats.addObserver(&observer);

    stats.setStat("HP", 100.0);
    observer.clear();

    stats.addToStat("HP", 50.0);

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].type == StatEventType::Changed);
    REQUIRE(observer.events[0].oldValue.value() == 100.0);
    REQUIRE(observer.events[0].newValue.value() == 150.0);
  }

  SECTION("Observer receives stat removed events")
  {
    stats.addObserver(&observer);

    stats.setStat("HP", 100.0);
    observer.clear();

    stats.removeStat("HP");

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].type == StatEventType::Removed);
    REQUIRE(observer.events[0].statName == "HP");
    REQUIRE(observer.events[0].oldValue.value() == 100.0);
    REQUIRE_FALSE(observer.events[0].newValue.has_value());
  }

  SECTION("Observer receives clear all stats event")
  {
    stats.addObserver(&observer);

    stats.setStat("HP", 100.0);
    stats.setStat("Attack", 50.0);
    observer.clear();

    stats.clearStats();

    REQUIRE(observer.events.size() == 1);
    REQUIRE(observer.events[0].type == StatEventType::Cleared);
  }

  SECTION("Multiple observers receive events")
  {
    TestStatObserver observer1;
    TestStatObserver observer2;

    stats.addObserver(&observer1);
    stats.addObserver(&observer2);

    stats.setStat("HP", 100.0);

    REQUIRE(observer1.events.size() == 1);
    REQUIRE(observer2.events.size() == 1);
    REQUIRE(observer1.events[0].newValue.value() == 100.0);
    REQUIRE(observer2.events[0].newValue.value() == 100.0);
  }

  SECTION("Observer can be removed")
  {
    stats.addObserver(&observer);

    stats.setStat("HP", 100.0);
    REQUIRE(observer.events.size() == 1);

    stats.removeObserver(&observer);
    stats.setStat("Attack", 50.0);

    // No new events after removal
    REQUIRE(observer.events.size() == 1);
  }

  SECTION("Duplicate observer registration is ignored")
  {
    stats.addObserver(&observer);
    stats.addObserver(&observer);// Duplicate

    stats.setStat("HP", 100.0);

    // Should only receive one event
    REQUIRE(observer.events.size() == 1);
  }
}
