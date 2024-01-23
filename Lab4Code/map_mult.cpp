#include<unordered_map>

//first this->power_coeff_vec into a map (say ThisMap)
std::unordered_map<power,coeff> thisMap;
for(auto v: this->power_coeff_vec) {
    thisMap.emplace(v.first, v.second);
}
//then put other.power_coeff_vec into another map (say OtherMap)
std::unordered_map<power,coeff> otherMap;
for(auto u: other.power_coeff_vec) {
    thisMap.emplace(u.first, u.second);
}


//instantiate a productMap to record multiply result before the double for
std::unordered_map<power, coeff> productMap;

for(auto t: thisMap) {
    for(auto o: otherMap) {
        power currPower = t.first + o.first;
        //see if this currPower existed in productMap already
        if(productMap.count(currPower) == 0) {
            //no liked powered term
            productMap.emplace(currPower, t.second * o.second);
        }
        else
        {
            productMap.at(currPower) += t.second * o.second;
        }
    }
}
