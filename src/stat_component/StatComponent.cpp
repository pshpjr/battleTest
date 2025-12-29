#include "battleTest/StatComponent.hpp"

namespace battleTest {

void StatComponent::setStat(const StatKey &statName, double value) { stats_[statName] = value; }

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

bool StatComponent::removeStat(const StatKey &statName) { return stats_.erase(statName) > 0; }

bool StatComponent::hasStat(const StatKey &statName) const { return stats_.find(statName) != stats_.end(); }

void StatComponent::addToStat(const StatKey &statName, double delta)
{
  auto it = stats_.find(statName);
  if (it != stats_.end()) {
    it->second += delta;
  } else {
    stats_[statName] = delta;
  }
}

void StatComponent::clearStats() { stats_.clear(); }

}// namespace battleTest
