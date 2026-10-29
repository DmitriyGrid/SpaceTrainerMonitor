#pragma once
#include <string>
#include <vector>
#include <optional>
namespace monitor {
struct Reading {int componentId;std::string parameter;double value;int severity;};
std::vector<Reading> sortBySeverity(const std::vector<Reading> &readings);
std::vector<Reading> findByComponent(const std::vector<Reading> &readings,int componentId);
std::optional<Reading> findMaximum(const std::vector<Reading> &readings,const std::string &parameter);
std::vector<Reading> selectCritical(const std::vector<Reading> &readings,int minSeverity);
}
