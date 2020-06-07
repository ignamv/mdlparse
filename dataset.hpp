#ifndef DATASET_HPP

#define DATASET_HPP

#include <functional>
#include <vector>
#include <memory>
#include <tuple>
#include <ostream>

using std::vector;
using std::unique_ptr;
using std::tie;
using std::ostream;
using std::istream;

class Dataset {
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
        unique_ptr<vector<double>> m_measured, m_simulated;
};

#endif
