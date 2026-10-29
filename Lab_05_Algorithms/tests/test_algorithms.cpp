#include "Algorithms.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
int main(){using namespace monitor;
const std::vector<Reading> r={{1,"temperature",45.0,0},{3,"voltage",26.8,2},{3,"voltage",24.1,0},{2,"temperature",39.5,1}};
assert(sortBySeverity(r).front().componentId==3);
assert(sortBySeverity(r)[1].componentId==2);
assert(findByComponent(r,3).size()==2);
assert(findByComponent(r,999).empty());
assert(findMaximum(r,"voltage").has_value());
assert(findMaximum(r,"voltage")->value==26.8);
assert(!findMaximum(r,"pressure").has_value());
assert(selectCritical(r,2).size()==1);
assert(selectCritical(r,0).size()==r.size());
assert(sortBySeverity({}).empty());
assert(!findMaximum({{1, "test", std::numeric_limits<double>::quiet_NaN(), 0}},"test").has_value());
std::cout<<"All tests passed\n";
}
