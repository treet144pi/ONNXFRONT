#include "parse_attr.h"
#include "onnx_import.h"

Graph onnxToGraph(const onnx::ModelProto& model)
{
    const auto& g = model.graph();
    Graph graph;
//------------------------------------------------------------------------------
    auto num_inputs = g.input_size();
    for (int ii = 0; ii < num_inputs; ++ii)
    {
        const auto& input_val= g.input(ii);
        size_t idx = graph.get_create_value(input_val.name());

        graph.values[idx].is_graph_input = true;
    }
//------------------------------------------------------------------------------
    auto num_outputs = g.output_size();
    for (int oi = 0; oi < num_outputs; ++oi)
    {
        const auto& output_val= g.output(oi);
        size_t idx = graph.get_create_value(output_val.name());
        graph.values[idx].is_graph_output = true;
    }
//------------------------------------------------------------------------------s
    auto num_initializer = g.initializer_size();
    for (int i = 0; i < num_initializer; ++i)
    {
        const auto& init_val= g.initializer(i);
        size_t idx = graph.get_create_value(init_val.name());

        graph.values[idx].is_initializer = true;
        graph.values[idx].dtype          = init_val.data_type();


        auto dims_size            = init_val.dims_size();
        auto& vector_shape = graph.values[idx].shape;

        vector_shape.clear();
        vector_shape.reserve(dims_size);

        for (int di = 0; di < dims_size; ++di)
            vector_shape.push_back(init_val.dims(di));
    }
//------------------------------------------------------------------------------

    auto num_node = g.node_size();
    for (int ni= 0; ni <  num_node; ++ni)
    {

      const auto& node = g.node(ni);
      int id_node               = graph.add_node(node.op_type(),node.name());

      auto node_size = node.input_size();
      for (int i = 0; i < node_size; ++i)
      {
        auto node_input = node.input(i);
        int  idx_input      = graph.get_create_value(node_input);

        graph.values[idx_input].consumers.push_back(id_node);
        graph.nodes[id_node].inputs.push_back(idx_input);

      }

      node_size = node.output_size();
      for (int i = 0; i < node_size; ++i)
      {
        auto node_output = node.output(i);
        int  idx_output      = graph.get_create_value(node_output);

        graph.values[idx_output].producer = id_node;
        graph.nodes[id_node].outputs.push_back(idx_output);
      }


      for (const auto& attr: node.attribute())
      {
        graph.nodes[id_node].attrs[attr.name()] = parse_attr(attr);
      }
    }
//------------------------------------------------------------------------------
  return graph;
}
