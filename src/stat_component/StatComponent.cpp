#include "battleTest/StatComponent.hpp"

namespace battleTest {

void StatComponent::setStat(const StatKey &statName, double value)
{
  auto oldValue = getStat(statName);
  stats_[statName] = value;
  notify(StatEvent::changed(statName, oldValue, value));
}

std::optional<double> StatComponent::getStat(const StatKey &statName) const
{
  auto it = stats_.find(statName);
  if (it != stats_.end()) { return it->second; }
  return std::nullopt;
}

double StatComponent::getStatOr(const StatKey &statName, double defaultValue) const
{
  auto it = stats_.find(statName);
  if (it != stats_.end()) { return it->second; }
  return defaultValue;
}

bool StatComponent::removeStat(const StatKey &statName)
{
  auto it = stats_.find(statName);
  if (it != stats_.end()) {
    double oldValue = it->second;
    stats_.erase(it);
    notify(StatEvent::removed(statName, oldValue));
    return true;
  }
  return false;
}

bool StatComponent::hasStat(const StatKey &statName) const { return stats_.find(statName) != stats_.end(); }

void StatComponent::addToStat(const StatKey &statName, double delta)
{
  auto it = stats_.find(statName);
  if (it != stats_.end()) {
    double oldValue = it->second;
    it->second += delta;
    notify(StatEvent::changed(statName, oldValue, it->second));
  } else {
    stats_[statName] = delta;
    notify(StatEvent::changed(statName, std::nullopt, delta));
  }
}

void StatComponent::clearStats()
{
  stats_.clear();
  notify(StatEvent::cleared());
}

}// namespace battleTest
