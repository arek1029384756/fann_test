#ifndef FNN_MATH
#define FNN_MATH

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <data_format.hpp>

namespace math {

    std::pair<double, double> limits(const mw::DataVector& in, const std::set<int>& mask) {
        std::cout << "limits" << std::endl;
        auto elem = in.getElements();
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::lowest();

        std::for_each(elem.begin(), elem.end(), [&](const mw::DataElement& el) {
                auto d = el.getData();
                for(const auto& m : mask) {
                    auto val = d.at(m);
                    if(val < min) {
                        min = val;
                    }
                    if(val > max) {
                        max = val;
                    }
                }
                });

        return std::make_pair(min, max);
    }

    std::vector<double> mean(const mw::DataVector& in) {
        std::cout << "avg" << std::endl;
        auto elem = in.getElements();
        std::vector<double> zero(elem[0].dataSize(), 0.0);
        std::vector<double> sums = std::accumulate(elem.begin(), elem.end(), zero, [&](std::vector<double> sum, mw::DataElement v) {
            for(std::size_t i = 0; i < zero.size(); ++i) {
                auto& x = sum[i];
                x += v.getData().at(i);
            }
            return sum;
        });

        for(auto& x : sums) {
            std::cout << x << std::endl;
            x /= elem.size();
            std::cout << x << std::endl << std::endl;
        }

        return sums;
    }

    std::vector<double> sigma(const mw::DataVector& in, std::vector<double> mean) {
        std::cout << "sig" << std::endl;
        auto elem = in.getElements();
        std::vector<double> zero(elem[0].dataSize(), 0.0);
        std::vector<double> sums = std::accumulate(elem.begin(), elem.end(), zero, [&](std::vector<double> sum, mw::DataElement v) {
            for(std::size_t i = 0; i < zero.size(); ++i) {
                auto& x = sum[i];
                x += std::pow(v.getData().at(i) - mean[i], 2);
            }
            return sum;
        });

        for(auto& x : sums) {
            std::cout << x << std::endl;
            x /= (elem.size() - 1);
            x = std::sqrt(x);
            std::cout << x << std::endl << std::endl;
        }

        return sums;
    }

    //void normalize(const std::vector<mw::DataElement>& in, std::vector<mw::DataElement>& out) {
    //}
}

#endif
