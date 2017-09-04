#ifndef FNN_MATH
#define FNN_MATH

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <data_format.hpp>

namespace math {

    template<class Oper, class... TArgs>
    auto compute(TArgs&... args) {
        try {
            return Oper::operation(args...);
        } catch(const std::out_of_range& e) {
            throw std::runtime_error(
                std::string("[1] Chosen dataset index exceeds range. ") +
                std::string("Maybe your input file lacks some columns? '") +
                    e.what() + std::string("'"));
        }
    }

    class Limits {
        public:
        static std::pair<double, double> operation(const mw::DataVector& in, const std::set<int>& mask) {
            std::cout << "limits" << std::endl;
            auto& elem = in.getElements();
            double min = std::numeric_limits<double>::max();
            double max = std::numeric_limits<double>::lowest();

            std::for_each(elem.begin(), elem.end(), [&](const mw::DataElement& el) {
                    for(const auto& m : mask) {
                        auto val = el.dataAt(m);
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
    };

    class Mean {
        public:
        static std::vector<double> operation(const mw::DataVector& in, const std::set<int>& mask) {
            std::cout << "mean" << std::endl;
            auto& elem = in.getElements();
            std::vector<double> zero(in.elementDataSize(), 0.0);
            std::vector<double> sums = std::accumulate(elem.begin(), elem.end(), zero, [&](std::vector<double>& sum, const mw::DataElement& v) {
                for(const auto& m : mask) {
                    auto& x = sum.at(m);
                    x += v.dataAt(m);
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
    };

    class Sigma {
        public:
        static std::vector<double> operation(const mw::DataVector& in, const std::vector<double>& mean, const std::set<int>& mask) {
            std::cout << "sigma" << std::endl;
            auto& elem = in.getElements();
            std::vector<double> zero(in.elementDataSize(), 0.0);
            std::vector<double> sums = std::accumulate(elem.begin(), elem.end(), zero, [&](std::vector<double>& sum, const mw::DataElement& v) {
                for(const auto& m : mask) {
                    auto& x = sum.at(m);
                    x += std::pow(v.dataAt(m) - mean.at(m), 2);
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
    };

    class GaussNorm {
        public:
        static int operation(const mw::DataVector& in, mw::DataVector& out, const std::set<int>& mask) {
            std::cout << "gauss" << std::endl;
            auto mean = compute<Mean>(in, mask);
            auto sigma = compute<Sigma>(in, mean, mask);
            auto& elem = in.getElements();
            for(const auto& x : elem) {
                mw::DataElement el;
                auto& v = x.getData();
                for(std::size_t i = 0; i < v.size(); ++i) {
                    el.addData((v.at(i) - mean.at(i)) / sigma.at(i));
                }
                out.addElement(el);
            }
            return 0;
        }
    };

}

#endif
