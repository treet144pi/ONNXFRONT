#include "graph.h"
#include <cassert>
#include <iostream>

int main() {
    Graph g;
    int x1 = g.get_create_value("X");
    int x2 = g.get_create_value("X");
    assert(x1 == x2);

    int a = g.get_create_value("A");
    int y = g.get_create_value("Y");
    int n = g.add_node("MatMul", "MMM");

    assert(g.nodes[n].op_type == "MatMul");

    g.nodes[n].inputs.push_back(x1);
    g.nodes[n].inputs.push_back(a);
    g.nodes[n].outputs.push_back(y);

    assert(g.nodes[n].inputs.size() == 2);
    assert(g.nodes[n].outputs.size() == 1);

    g.values[x1].consumers.push_back(n);
    g.values[a].consumers.push_back(n);
    g.values[y].producer = n;

    assert(g.values[y].producer == n);
    assert(!g.values[x1].consumers.empty());

    std::cout << "==================test_graph PASSED==================\n";
    return 0;
}
