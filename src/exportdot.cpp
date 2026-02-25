#include "exportdot.h"
#include "graph.h"
#include <fstream>


void export_dot(const Graph& g, const std::string& path) {
    std::ofstream out(path);
    if (!out) return;

    out << "digraph ONNX {\n";
    out << "  rankdir=TB;\n";
    // ноды прямоугольнии
    for (const auto& n : g.nodes)
    {
        out << "  op_" << n.id
            << " [shape=box,style=\"rounded,filled\", fillcolor=green, label=\"" << n.op_type;
        if (!n.name.empty()) out << "\\n" << n.name;

        out << "\"];\n";
    }

    // эллипсы это value
    for (const auto& v : g.values)
    {
        out << "  val_" << v.id
            << " [shape=ellipse,style=filled, fillcolor=white, label=\"" << v.name;

        if (v.is_graph_input)  out << "\\n[in]";
        if (v.is_graph_output) out << "\\n[out]";
        if (v.is_initializer)  out << "\\n[init]";

        out << "\"];\n";
    }


    for (const auto& n : g.nodes)
    {
        for (int vid : n.inputs)
            out << "  val_" << vid << " -> op_" << n.id << ";\n";

    }

    for (const auto& n : g.nodes)
    {
        for (int vid : n.outputs)
            out << "  op_" << n.id << " -> val_" << vid << ";\n";

    }

    out << "}\n";
}
