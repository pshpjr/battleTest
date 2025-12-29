#include "battleTest/StatComponent.hpp"

namespace battleTest {

void StatComponent::setStat(const std::string &statName, double value) { stats_[statName] = value; }

std::optional<double> StatComponent::getStat(const std::string &statName) const
{
  auto it = stats_.find(statName);
  if (it != stats_.end()) { return it->second; }
  return std::nullopt;
}

double StatComponent::getStatOr(const std::string &statName, double defaultValue) const
{
  auto it = stats_.find(statName);
  if (it != stats_.end()) { return it->second; }
  return defaultValue;
}

bool StatComponent::removeStat(const std::string &statName) { return stats_.erase(statName) > 0; }

bool StatComponent::hasStat(const std::string &statName) const { return stats_.find(statName) != stats_.end(); }

void StatComponent::addToStat(const std::string &statName, double delta)
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
