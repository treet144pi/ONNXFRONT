#pragma once
#include "parse_attr.h"

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

struct Node {
  int              id = -1;
  std::string      op_type;
  std::string      name;
  std::vector<int> inputs;
  std::vector<int> outputs;
  AttrMap          attrs;        // атрибуты
};

struct Value {
  int id = -1;
  std::string name;
  int dtype = 0;
  std::vector<int64_t> shape;

  int producer = -1;                // node id, который создал этот value
  std::vector<int> consumers;       // node id, которые используют это value
  bool is_initializer = false;
  bool is_graph_input = false;
  bool is_graph_output = false;
};


struct Graph
{
    public:
        std::vector<Value>                   values;
        std::vector<Node>                    nodes;
        std::unordered_map<std::string, int> value_by_name;

    public: // functions
        int add_node(std::string op_type, std::string name = "");
        int get_create_value(std::string name);

};
