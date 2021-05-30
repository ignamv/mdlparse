#include <functional>
#include <utility>
#include <exception>
#include <cassert>
#include "dataset.hpp"
#include "util.hpp"
#include "dataset_friend.hpp"
#include <string>

using std::string;

#define BASE_10 10

Dataset::Dataset(Dataset::Type type,
        unique_ptr<vector<double>> measured,
        unique_ptr<vector<double>> simulated) :
        m_type(type),
        m_measured(std::move(measured)),
        m_simulated(std::move(simulated))
{
    switch (m_type)
    {
        case Dataset::Type::t_measured:
        case Dataset::Type::t_both:
        case Dataset::Type::t_common:
            m_size = m_measured->size();
            break;
        case Dataset::Type::t_simulated:
            m_size = m_simulated->size();
    }
}

bool Dataset::operator==(const Dataset& rhs) const
{
    if (m_type != rhs.m_type)
        return false;
    if (*m_measured != *rhs.m_measured)
        return false;
    if (m_type == Dataset::Type::t_both && *m_simulated != *rhs.m_simulated)
        return false;
    return true;
}


int Dataset::size() const
{
    return m_size;
}

Dataset::Type parse_type(istream &is)
{
    char c;
    is.get(c);
    switch(c)
    {
        case 'M':
            is >> Consume("EAS");
            return Dataset::Type::t_measured;
        case 'S':
            is >> Consume("IMU");
            return Dataset::Type::t_simulated;
        case 'C':
            is >> Consume("OMMON");
            return Dataset::Type::t_common;
        case 'B':
            is >> Consume("OTH");
            return Dataset::Type::t_both;
        default:
            throw SyntaxError("Expected MEAS, SIMU, COMMON or BOTH");
    } 
}

Datasize parse_datasize(istream &is)
{
    int size, ports1, ports2;
    is >> Consume("datasize ");
    Dataset::Type type = parse_type(is);
    is >> size >> ports1 >> ports2 >> Consume("\n");
    assert(size > 0);
    assert(ports1 > 0);
    assert(ports2 > 0);
    return {type, size, ports1, ports2};
}

int get_current_array(Dataset::Type types, Dataset::Type current)
{
    assert(current != Dataset::Type::t_both);
    if (types != Dataset::Type::t_both) {
        if (current == types)
            return 0;
    } else {
        if (current == Dataset::Type::t_measured)
            return 0;
        else if (current == Dataset::Type::t_simulated)
            return 1;
    }
    throw SyntaxError("Invalid data type in dataset");
}

unique_ptr<Dataset> Dataset::from_lines(istream& is)
{
    char line[LINE_LENGTH];
    char *readptr;
    auto [types, size, ports1, ports2] = parse_datasize(is);
    int total_size = 2 * size * ports1 * ports2;
    vector<double> *data[2] = {
        new vector<double>(total_size),
        types == Dataset::Type::t_both ? 
                new vector<double>(total_size) : 
                NULL,
    };
    //data[0] = unique_ptr<vector<double>>(new vector<double>(total_size));
    //if (types == Dataset::Type::t_both)
    //{
        //data[1] = new vector<double>(total_size);
    //}
    Dataset::Type current_type;
    int current_array;
    bool type_set = false;
    char c;
    Consume newline("\n"), space(" ");
    while (true)
    {
        is.get(c);
        if (c == 't')
        {
            is >> Consume("ype ");
            current_type = parse_type(is);
            is >> newline;
            current_array = get_current_array(types, current_type);
            type_set = true;
        } else if(c == 'p') {
            int idx, port1, port2;
            double real, imag;
            is >> Consume("oint ") >> idx >> port1 >> port2 >> real >> imag >> newline;
            int base_location = 2 * (idx + size * (port2 - 1 + ports2 * (port1 - 1)));
            vector<double>& array = *data[current_array];
            array[base_location+0] = real;
            array[base_location+1] = imag;
        } else if(c == '}') {
            is >> newline;
            break;
        } else {
            throw SyntaxError("Invalid line in dataset");
        }
    }
    unique_ptr<Dataset> ret = unique_ptr<Dataset>(new Dataset(
                types, 
                unique_ptr<vector<double>>(data[0]),
                unique_ptr<vector<double>>(data[1]))
            );
    return ret;
}

string type_to_string(Dataset::Type type)
{
    switch(type)
    {
        case Dataset::Type::t_measured:
            return "MEAS";
        case Dataset::Type::t_simulated:
            return "SIMU";
        case Dataset::Type::t_common:
            return "COMMON";
        case Dataset::Type::t_both:
            return "BOTH";
        default:
            throw;
    }
}

void doubles_to_stream(ostream& os, const vector<double>& data)
{
    os << "[";
    bool first = true;
    for (const double& x : data)
    {
        if (!first)
            os << ", ";
        first = false;
        os << x;
    }
    os << "]";
}

ostream& operator<<(ostream& os, const Dataset& dataset)
{
    
    os << "Dataset(" << type_to_string(dataset.m_type) << ", ";
    doubles_to_stream(os, *dataset.m_measured);
    if (dataset.m_type == Dataset::Type::t_both)
    {
        os << ", ";
        doubles_to_stream(os, *dataset.m_simulated);
    }
    os << ")";
    return os;
}