#include "graph.h"

int Graph::add_node(std::string op_type, std::string name)
{
    int id = nodes.size();
    nodes.push_back(Node{});
    nodes.back().id = id;
    nodes.back().op_type = op_type;
    nodes.back().name = name;
    return id;
}


int Graph::get_create_value(std::string name)
{
    auto it = value_by_name.find(name);
    if (it != value_by_name.end()) return it->second;

    int id = values.size();
    values.push_back(Value{});
    values.back().id = id;
    values.back().name = name;
    value_by_name[name] = id;

    return id;
}
