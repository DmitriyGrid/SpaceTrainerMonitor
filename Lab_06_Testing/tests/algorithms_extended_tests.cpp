#include "Algorithms.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
using monitor::Reading;
static int checks=0;
static void require(bool ok, const char* what) { ++checks; if (!ok) throw std::runtime_error(what); }
int main() {
 try {
  const std::vector<Reading> r={{1,"temperature",45,0},{3,"voltage",26.8,2},{3,"voltage",24.1,0},{2,"temperature",39.5,1}};
  auto sorted=monitor::sortBySeverity(r);
  require(sorted.size()==4,"sort size");
  require(sorted[0].severity==2 && sorted[1].severity==1,"severity descending");
  require(sorted[2].componentId==1 && sorted[3].componentId==3,"stable ties");
  require(r[0].componentId==1,"input unchanged");
  require(monitor::sortBySeverity({}).empty(),"empty sort");
  require(monitor::findByComponent(r,3).size()==2,"search existing");
  require(monitor::findByComponent(r,99).empty(),"search missing");
  require(monitor::findByComponent({},3).empty(),"search empty");
  auto max=monitor::findMaximum(r,"voltage");
  require(max && max->value==26.8,"max voltage");
  require(!monitor::findMaximum(r,"pressure"),"max missing parameter");
  const double nan=std::numeric_limits<double>::quiet_NaN();
  auto finite=monitor::findMaximum({{1,"temperature",nan,0},{1,"temperature",35,0}},"temperature");
  require(finite && finite->value==35,"skip NaN");
  require(!monitor::findMaximum({{1,"temperature",nan,0}},"temperature"),"only NaN");
  require(monitor::selectCritical(r,2).size()==1,"critical threshold 2");
  require(monitor::selectCritical(r,1).size()==2,"critical threshold 1");
  require(monitor::selectCritical(r,3).empty(),"critical beyond max");
  require(monitor::selectCritical({},2).empty(),"critical empty");
  std::cout << "PASS: " << checks << " assertions on four algorithms\n";
  return 0;
 } catch(const std::exception& e) {std::cerr << "FAIL: " << e.what() << "\n"; return 1;}
}
