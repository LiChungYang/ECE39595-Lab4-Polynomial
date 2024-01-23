#include "poly.h"
#include<assert.h>
#include<algorithm>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <cmath>

polynomial :: polynomial()
{
    //Construct a new polynomial object that is the number 0x^0
    power_coef_vec.push_back(std::make_pair(0, 0));
}

polynomial :: polynomial(const polynomial &other)
{
    this->power_coef_vec = other.power_coef_vec;
}

void polynomial:: print() const
{
    for(auto iter = power_coef_vec.begin(); iter != power_coef_vec.end(); iter++)
    {
        std::cout << iter->second << "x^" << iter->first;
        if(next(iter, 1) != power_coef_vec.end()) 
        {
            std::cout<< " + ";
        }
    }
    std::cout<<"\n";
}

polynomial &polynomial::operator=(const polynomial &other)
{
    if(this == &other)
    {
        //assign to myself
        return *this;
    }
    else
    {
        this->power_coef_vec = other.power_coef_vec;
    }
    return *this;
}


polynomial polynomial::operator+(const polynomial &other) const {
    // Combine both polynomial vectors
    std::vector<std::pair<size_t, int>> combined_vec = this->power_coef_vec;
    combined_vec.insert(combined_vec.end(), other.power_coef_vec.begin(), other.power_coef_vec.end());

    //Sort the combined vector
    std::sort(combined_vec.begin(), combined_vec.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    //Prepare for multithreading
    const size_t num_threads = 8;
    std::vector<std::thread> threads(num_threads);
    std::vector<std::vector<std::pair<size_t, int>>> thread_results(num_threads);
    size_t chunk_size = combined_vec.size() / num_threads;

    //Lambda function for processing each chunk
    //adding the coefficient of the same power
    auto process_chunk = [&combined_vec](size_t start, size_t end, std::vector<std::pair<size_t, int>>& result) 
    {
        std::vector<std::pair<size_t, int>> local_result;
        for (size_t i = start; i < end; ++i) {
            if (!local_result.empty() && local_result.back().first == combined_vec[i].first) {
                local_result.back().second += combined_vec[i].second;
            } else {
                local_result.push_back(combined_vec[i]);
            }
        }
        result = std::move(local_result);
    };

    // Start threads
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? combined_vec.size() : start + chunk_size;
        threads[i] = std::thread(process_chunk, start, end, std::ref(thread_results[i]));
    }

    // Wait for threads to finish
    for (auto& t : threads) 
    {
        t.join();
    }

    // Merge thread results
    std::vector<std::pair<size_t, int>> final_result;
    for (auto& tr : thread_results) {
        final_result.insert(final_result.end(), tr.begin(), tr.end());
    }

    // Final sorting and combining of terms
    std::sort(final_result.begin(), final_result.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    polynomial out_polynomial;
    for (size_t i = 0; i < final_result.size(); ++i) {
        if (!out_polynomial.power_coef_vec.empty() && out_polynomial.power_coef_vec.back().first == final_result[i].first) {
            out_polynomial.power_coef_vec.back().second += final_result[i].second;
        } else {
            out_polynomial.power_coef_vec.push_back(final_result[i]);
        }
    }

    return out_polynomial;
}


polynomial polynomial::operator+(const int &other) const
{
    polynomial out_polynomial = *this;
    polynomial zero_polynomial;
    int checkifint = 0;

    if(out_polynomial.find_degree_of() == 0 && other == 0)
    {
        (zero_polynomial.power_coef_vec).push_back(std::make_pair(0, 0));
        return zero_polynomial;
    }

    for(auto iter1 = (out_polynomial.power_coef_vec).begin(); iter1 != (out_polynomial.power_coef_vec).end(); iter1++)
    {
        if(iter1->first == 0)
        {
            iter1->second = (iter1->second) + other;
            checkifint++;
            break;
        }
    }
    if(checkifint == 0)
    {
        (out_polynomial.power_coef_vec).push_back(std::make_pair(0, other));
    }

    return out_polynomial;
}

polynomial operator+(int otherint, polynomial const &other) //int + poly
{
    return other + otherint;
}


/*
polynomial polynomial::operator*(const polynomial &other) const {
    const size_t num_threads = 8;
    std::vector<std::thread> threads(num_threads);
    //std::vector<std::vector<std::pair<size_t, int>>> partial_results(num_threads);
    std::vector<polynomial> partial_results(num_threads);

    size_t chunk_size = this->power_coef_vec.size() / num_threads;

    // Thread function for multiplying a chunk
    //auto multiply_chunk = [this, &other](size_t start_index, size_t end_index, 
    //                                        std::vector<std::pair<size_t, int>>& result) {
    auto multiply_chunk = [this, &other](size_t start_index, size_t end_index, polynomial& result) 
    {
    //test
    polynomial tempoly;
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        for (size_t i = start_index; i < end_index; i++) 
        {
            for (const auto& iter2 : other.power_coef_vec) 
            {
                size_t temp_power = this->power_coef_vec[i].first + iter2.first;
                int temp_coeff = this->power_coef_vec[i].second * iter2.second;
                //result.push_back(std::make_pair(temp_power, temp_coeff));
                tempoly.power_coef_vec.push_back(std::make_pair(temp_power, temp_coeff));
            }
        }
        result = result + tempoly;
        //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        //auto chretime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        //std::cout << "Start1 " << start_index << " to1 " << end_index << " took " <<  chretime  << " milliseconds" << std::endl;

    };
    //test2


    // Starting the threads
    for (size_t i = 0; i < num_threads; ++i) 
    {
        size_t start_index = i * chunk_size;

        size_t end_index;

        if (i == num_threads - 1) 
        {
            end_index = this->power_coef_vec.size();
        } 

        else 
        {
            end_index = start_index + chunk_size;
        }

        threads[i] = std::thread(multiply_chunk, start_index, end_index, std::ref(partial_results[i]));
    }

    // Joining the threads
    for (auto& thread : threads) 
    {
        thread.join();
    }

    polynomial out_polynomial;

    //std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
    for (const auto& result : partial_results) {
        out_polynomial = out_polynomial + result;
    }
    //std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    //auto chretime = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - begin2).count();
    //std::cout << "Start2 to2 " << " took " <<  chretime  << " milliseconds" << std::endl;

    return out_polynomial;
    
}
*/

/*
polynomial polynomial::operator*(const polynomial &other) const {
    const size_t num_threads = 8;
    std::vector<std::thread> threads(num_threads);
    //std::vector<std::vector<std::pair<size_t, int>>> partial_results(num_threads);
    std::vector<polynomial> partial_results(num_threads);

    size_t chunk_size = this->power_coef_vec.size() / num_threads;


    //new
    //test
    std::unordered_map<power,coeff> thisMap;
    for(auto v: this->power_coef_vec) {
        thisMap.emplace(v.first, v.second);
    }

    //then put other.power_coeff_vec into another map (say OtherMap)
    std::unordered_map<power,coeff> otherMap;
    for(auto u: other.power_coef_vec) {
        thisMap.emplace(u.first, u.second);
    }

    // Thread function for multiplying a chunk
    //auto multiply_chunk = [this, &other](size_t start_index, size_t end_index, 
    //                                        std::vector<std::pair<size_t, int>>& result) {
    auto multiply_chunk = [this, &other](std::unordered_map<power,coeff> thisMap, std::unordered_map<power,coeff> otherMap, polynomial& result) 
    {
        std::unordered_map<power, coeff> productMap;
        //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
            result = polynomial(productMap.begin(), productMap.end());
        }
        //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        //auto chretime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        //std::cout << "Start1 " << start_index << " to1 " << end_index << " took " <<  chretime  << " milliseconds" << std::endl;

    };
    //test2


    // Starting the threads
    for (size_t i = 0; i < num_threads; ++i) 
    {
        size_t start_index = i * chunk_size;

        size_t end_index;

        if (i == num_threads - 1) 
        {
            end_index = this->power_coef_vec.size();
        } 

        else 
        {
            end_index = start_index + chunk_size;
        }

        threads[i] = std::thread(multiply_chunk, thisMap, otherMap, std::ref(partial_results[i]));
    }

    // Joining the threads
    for (auto& thread : threads) 
    {
        thread.join();
    }

    polynomial out_polynomial;

    //std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
    for (const auto& result : partial_results) {
        out_polynomial = out_polynomial + result;
    }
    //std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    //auto chretime = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - begin2).count();
    //std::cout << "Start2 to2 " << " took " <<  chretime  << " milliseconds" << std::endl;

    return out_polynomial;
    
}
*/

polynomial polynomial::operator*(const polynomial &other) const {
    const size_t num_threads = 8;
    std::vector<std::thread> threads(num_threads);
    std::vector<std::unordered_map<size_t, int>> partial_results(num_threads);

    // Determine the chunk size for each thread
    size_t chunk_size = this->power_coef_vec.size() / num_threads + (this->power_coef_vec.size() % num_threads != 0);

    // Thread function for multiplying a chunk
    auto multiply_chunk = [&](size_t start_index, size_t end_index, size_t thread_index) {
        for (size_t i = start_index; i < end_index && i < this->power_coef_vec.size(); ++i) {
            for (const auto& term : other.power_coef_vec) {
                size_t combined_power = this->power_coef_vec[i].first + term.first;
                int combined_coeff = this->power_coef_vec[i].second * term.second;
                partial_results[thread_index][combined_power] += combined_coeff;
            }
        }
    };

    // Start the threads
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start_index = i * chunk_size;
        size_t end_index = start_index + chunk_size;
        threads[i] = std::thread(multiply_chunk, start_index, end_index, i);
    }

    // Join the threads
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // Aggregate results
    std::unordered_map<size_t, int> final_result_map;
    for (const auto& result_map : partial_results) {
        for (const auto& term : result_map) {
            final_result_map[term.first] += term.second;
        }
    }

    // Convert final_result_map to polynomial
    polynomial out_polynomial;
    for (const auto& term : final_result_map) {
        out_polynomial.power_coef_vec.push_back(std::make_pair(term.first, term.second));
    }

    return out_polynomial;
}

polynomial polynomial::operator*(const int &other) const
{
    polynomial out_polynomial;
    //create an empty vector first
    int temp_coeff = 0;

    for(auto iter1 = (this -> power_coef_vec).begin(); iter1 != (this -> power_coef_vec).end(); iter1++)
    {
        //times the coefficient
        temp_coeff = iter1->second * other;

        out_polynomial.power_coef_vec.push_back(std::make_pair(iter1 -> first, temp_coeff));
    }

    (out_polynomial.power_coef_vec).erase((out_polynomial.power_coef_vec).begin());

    return out_polynomial;
}

polynomial operator*(int otherint, polynomial const &other) //int * poly
{
    return other * otherint;
}




polynomial polynomial::operator%(const polynomial &other) const
{
    polynomial out_polynomial = *this;
    polynomial othertemp_polynomial = other;
    polynomial temp_polynomial;
    polynomial zero_temppoly;
    size_t degree_vec1 = 0;
    size_t degree_vec2 = 0;
    size_t multi_pow = 0;

    int coe_vec1 = 0;
    int coe_vec2 = 0;
    int multi_coe = 0;
    int count = 1;

    if (this -> power_coef_vec == other.power_coef_vec)
    {
        return zero_temppoly;
    }
    
    
    //find the degree of the two polynomial and their correspoding coefficient
    std::sort(out_polynomial.power_coef_vec.begin(), out_polynomial.power_coef_vec.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    std::sort(othertemp_polynomial.power_coef_vec.begin(), othertemp_polynomial.power_coef_vec.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    degree_vec1 = out_polynomial.power_coef_vec.begin()->first;
    coe_vec1 = out_polynomial.power_coef_vec.begin()->second;


    degree_vec2 = othertemp_polynomial.power_coef_vec.begin()->first;
    coe_vec2 = othertemp_polynomial.power_coef_vec.begin()->second;

    
    // for(auto iter1 = (this->power_coef_vec).begin(); iter1 != (this->power_coef_vec).end(); iter1++)
    // {
    //     if(iter1->first >= degree_vec1)
    //     {
    //         degree_vec1 = iter1->first;
    //         coe_vec1 = iter1->second;
    //     }
    // }
    

    //degree_vec1 = this->find_degree_of();

    // for(auto iter2 = (other.power_coef_vec).begin(); iter2 != (other.power_coef_vec).end(); iter2++)
    // {
    //     if(iter2->first >= degree_vec2)
    //     {
    //         degree_vec2 = iter2->first;
    //         coe_vec2 = iter2->second;
    //     }
    // }
    


    //if divide by an integer
    if(degree_vec2 == 0)
    {
        return zero_temppoly;
    }

    //if the degree of the first polynomial is smaller, the remainder will be the same as the whole polinomial
    if(degree_vec1 < degree_vec2)
    {
        return *this;
    }

    if(degree_vec2 == 1 && coe_vec2 == 1)
    {
        int valint = -((othertemp_polynomial.power_coef_vec).at(count).second);
        int out = 0;
        for(auto iter1 = (this->power_coef_vec).begin(); iter1 != (this->power_coef_vec).end(); iter1++)
        {
            out += pow(valint, iter1->first) * iter1->second;
        }
        temp_polynomial.power_coef_vec.push_back((std::make_pair(0, out)));
        return temp_polynomial;
    }


    multi_pow = degree_vec1 - degree_vec2;
    multi_coe = coe_vec1 / coe_vec2;

    while(degree_vec1 >= degree_vec2)
    {
        if(count != 1)
        {
            temp_polynomial = zero_temppoly;
            multi_pow = degree_vec1 - degree_vec2;
            coe_vec1 = (out_polynomial.power_coef_vec).at(count - 1).second;
            
            // for(auto iter1 = (out_polynomial.power_coef_vec).begin(); iter1 != (out_polynomial.power_coef_vec).end(); iter1++)
            // {
            //     if(iter1->first == degree_vec1)
            //     {
            //         coe_vec1 = iter1->second;
            //     }
            // }
            
            multi_coe = coe_vec1 / coe_vec2;
        }

        // temp_polynomial.power_coef_vec.push_back(std::make_pair(multi_pow, -(multi_coe)));

        // temp_polynomial = temp_polynomial * othertemp_polynomial;

        
        for(auto iter2 = (other.power_coef_vec).begin(); iter2 != (other.power_coef_vec).end(); iter2++)
        {
            //copy the value times the advisor to the temp poly
            temp_polynomial.power_coef_vec.push_back(std::make_pair(multi_pow + iter2->first, -(multi_coe * iter2->second)));
            //temp_power_coef_vec.push_back(std::make_pair(multi_pow + iter2->first, multi_coe * iter2->second));            
        }
        

        out_polynomial = out_polynomial + (temp_polynomial);
        //degree_vec1 = out_polynomial.find_degree_of();
        degree_vec1 = (out_polynomial.power_coef_vec).at(count).first;
        count++;
    }
    
    return out_polynomial;

}


/*
polynomial polynomial::operator%(const polynomial &other) const
{
    polynomial out_polynomial = *this;
    polynomial temp_polynomial;
    polynomial zero_temppoly;
    size_t degree_vec1 = 0;
    size_t degree_vec2 = 0;
    size_t multi_pow = 0;

    int coe_vec1 = 0;
    int coe_vec2 = 0;
    int multi_coe = 0;
    int count = 1;

    if (this -> power_coef_vec == other.power_coef_vec)
    {
        return zero_temppoly;
    }
    
    //find the degree of the two polynomial and their correspoding coefficient
    for(auto iter1 = (this->power_coef_vec).begin(); iter1 != (this->power_coef_vec).end(); iter1++)
    {
        if(iter1->first >= degree_vec1)
        {
            degree_vec1 = iter1->first;
            coe_vec1 = iter1->second;
        }
    }
    

    //degree_vec1 = this->find_degree_of();

    for(auto iter2 = (other.power_coef_vec).begin(); iter2 != (other.power_coef_vec).end(); iter2++)
    {
        if(iter2->first >= degree_vec2)
        {
            degree_vec2 = iter2->first;
            coe_vec2 = iter2->second;
        }
    }

    //if divide by an integer
    if(degree_vec2 == 0)
    {
        return zero_temppoly;
    }

    //if the degree of the first polynomial is smaller, the remainder will be the same as the whole polinomial
    if(degree_vec1 < degree_vec2)
    {
        return *this;
    }

    multi_pow = degree_vec1 - degree_vec2;
    multi_coe = coe_vec1 / coe_vec2;

    while(degree_vec1 >= degree_vec2)
    {
        if(count != 1)
        {
            temp_polynomial = zero_temppoly;
            multi_pow = degree_vec1 - degree_vec2;
            for(auto iter1 = (out_polynomial.power_coef_vec).begin(); iter1 != (out_polynomial.power_coef_vec).end(); iter1++)
            {
                if(iter1->first == degree_vec1)
                {
                    coe_vec1 = iter1->second;
                }
            }
            multi_coe = coe_vec1 / coe_vec2;
        }

        for(auto iter2 = (other.power_coef_vec).begin(); iter2 != (other.power_coef_vec).end(); iter2++)
        {
            //copy the value times the advisor to the temp poly
            temp_polynomial.power_coef_vec.push_back(std::make_pair(multi_pow + iter2->first, -(multi_coe * iter2->second)));
            //temp_power_coef_vec.push_back(std::make_pair(multi_pow + iter2->first, multi_coe * iter2->second));            
        }

        out_polynomial = out_polynomial + (temp_polynomial);
        degree_vec1 = out_polynomial.find_degree_of();
        count++;
    }
    
    return out_polynomial;

}
*/


size_t polynomial::find_degree_of()
{
    size_t Max_first = 0;
    for (auto i : power_coef_vec) {

        if (i.first > Max_first && i.second != 0) {
            Max_first = i.first;
        }
    }

    if(Max_first == 0 && power_coef_vec.begin()->second == 0)
    {
        return 0;
    }

    return Max_first;
}

std::vector<std::pair<power, coeff>> polynomial :: canonical_form() const
{
    polynomial temp_polynomial = *this;
    std::vector<std::pair<power, coeff>> out_coef_vec;

    //size_t degree_vec1 = 0;
    //int coe_vec1 = 0;
    //auto itertemp = (this->power_coef_vec).begin();

    //sort the vec
    std::sort(temp_polynomial.power_coef_vec.begin(), temp_polynomial.power_coef_vec.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    //std::cout << temp_polynomial.power_coef_vec.begin()->first << "here\n";

    //if(temp_coef_vec.begin()->first == 0)
    if(temp_polynomial.find_degree_of() == 0)
    {
        out_coef_vec.push_back(std::make_pair(0, temp_polynomial.power_coef_vec.at(temp_polynomial.power_coef_vec.size() - 1).second));
        return out_coef_vec;
    }

    //find the degree of the polynomial and its correspoding coefficient
    
    for(auto iter1 = temp_polynomial.power_coef_vec.begin(); iter1 != temp_polynomial.power_coef_vec.end(); iter1++)
    {
        if(iter1->second != 0)
        {
            out_coef_vec.push_back(std::make_pair(iter1->first, iter1->second));
        }
    }

    return out_coef_vec;
}