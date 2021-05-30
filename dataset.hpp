#ifndef DATASET_HPP

#define DATASET_HPP

#include <functional>
#include <vector>
#include <memory>
#include <tuple>
#include <ostream>
#include "token.hpp"

using std::vector;
using std::unique_ptr;
using std::tie;
using std::ostream;
using std::istream;

class Dataset : public TokenData {
    public:
        enum Type { t_measured, t_simulated, t_common, t_both };
        Dataset(Type, unique_ptr<vector<double>>, unique_ptr<vector<double>>);
        int size() const;
        vector<double>& measured();
        vector<double>& simulated();
        vector<double>& common();
        static unique_ptr<Dataset> from_lines(istream&);
        bool operator==(const Dataset& rhs) const;
        friend ostream& operator<<(ostream& os, const Dataset& dataset);
    private:
        Type m_type;
        int m_size;
        unique_ptr<vector<double>> m_array1, m_array2;
        void to_stream(std::ostream&) const;
        bool equals(const TokenData&) const;
};

#endif
