#include "Algorithms.h"
#include <iostream>
int main(){std::vector<monitor::Reading> data={{3,"voltage",24.1,0},{3,"voltage",26.8,2},{1,"temperature",48.2,0},{2,"temperature",39.5,0}};
for(const auto &x:monitor::sortBySeverity(data))std::cout<<x.componentId<<' '<<x.parameter<<' '<<x.value<<' '<<x.severity<<'\n';}
