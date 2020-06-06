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

Dataset::Type parse_type(const char *line, const char **endptr)
{
    if (startsWith(line, "MEAS")) {
        if (endptr)
            *endptr = &line[4];
        return Dataset::Type::t_measured;
    } else if (startsWith(line, "SIMU")) {
        if (endptr)
            *endptr = &line[4];
        return Dataset::Type::t_simulated;
    } else if (startsWith(line, "BOTH")) {
        if (endptr)
            *endptr = &line[4];
        return Dataset::Type::t_both;
    } else if (startsWith(line, "COMMON")) {
        if (endptr)
            *endptr = &line[6];
        return Dataset::Type::t_common;
    } else {
        throw SyntaxError("Expected MEAS, SIMU, COMMON or BOTH");
    }
}

Datasize parse_datasize(const char *line)
{
    const char *readptr;
    assert(startsWith(line, "datasize "));
    Dataset::Type type = parse_type(&line[9], &readptr);
    assert(*readptr++ == ' ');
    int size = strtol(readptr, (char**)&readptr, BASE_10);
    assert(size > 0);
    assert(*readptr++ == ' ');
    int ports1 = strtol(readptr, (char**)&readptr, BASE_10);
    assert(ports1 > 0);
    assert(*readptr++ == ' ');
    int ports2 = strtol(readptr, (char**)&readptr, BASE_10);
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

unique_ptr<Dataset> Dataset::from_lines(std::function<int(char*)> get_line)
{
    char line[LINE_LENGTH];
    char *readptr;
    assert(get_line(line));
    auto [types, size, ports1, ports2] = parse_datasize(line);
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
    while (true)
    {
        assert(get_line(line));
        if (startsWith(line, "type "))
        {
            current_type = parse_type(&line[5], NULL);
            current_array = get_current_array(types, current_type);
            type_set = true;
        } else if(startsWith(line, "point ")) {
            int idx = strtol(&line[6], &readptr, BASE_10);
            assert(*readptr == ' ');
            int port1 = strtol(readptr, &readptr, BASE_10) - 1;
            assert(*readptr == ' ');
            int port2 = strtol(readptr, &readptr, BASE_10) - 1;
            assert(*readptr == ' ');
            double real = strtod(readptr, &readptr);
            assert(*readptr == ' ');
            double imag = strtod(readptr, &readptr);
            int base_location = 2 * (idx + size * (port2 + ports2 * port1));
            vector<double>& array = *data[current_array];
            array[base_location+0] = real;
            array[base_location+1] = imag;
        } else if(line[0] == '}') {
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
