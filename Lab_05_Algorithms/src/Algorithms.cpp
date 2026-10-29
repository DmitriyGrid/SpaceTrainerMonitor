#include "Algorithms.h"
#include <algorithm>
#include <cmath>
namespace monitor {
std::vector<Reading> sortBySeverity(const std::vector<Reading>& r){auto out=r;std::stable_sort(out.begin(),out.end(),[](const Reading&a,const Reading&b){return a.severity>b.severity;});return out;}
std::vector<Reading> findByComponent(const std::vector<Reading>& r,int id){std::vector<Reading> out;std::copy_if(r.begin(),r.end(),std::back_inserter(out),[id](const Reading& x){return x.componentId==id;});return out;}
std::optional<Reading> findMaximum(const std::vector<Reading>& r,const std::string& name){std::optional<Reading> result;for(const auto& x:r)if(x.parameter==name && std::isfinite(x.value) && (!result || x.value>result->value))result=x;return result;}
std::vector<Reading> selectCritical(const std::vector<Reading>& r,int threshold){std::vector<Reading> out;std::copy_if(r.begin(),r.end(),std::back_inserter(out),[threshold](const Reading& x){return x.severity>=threshold;});return out;}
}
