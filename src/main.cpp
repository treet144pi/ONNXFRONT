#include "graph.h"
#include "onnx.pb.h"
#include "exportdot.h"
#include "onnx_import.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>


int main(int argc, char** argv)
{
    if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " model.onnx\n";
      return 1;
    }

    std::ifstream in(argv[1], std::ios::binary);
    if (!in) {
      std::cerr << "Cannot open: " << argv[1] << "\n";
      return 2;
    }

    std::string data((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());

    onnx::ModelProto model;
    if (!model.ParseFromString(data)) {
      std::cerr << "Failed to parse ONNX\n";
      return 3;
    }


    Graph graph = onnxToGraph(model);

    export_dot(graph, "graph.dot");
    std::cout << "saved our graph"<<'\n';

    return 0;
}
