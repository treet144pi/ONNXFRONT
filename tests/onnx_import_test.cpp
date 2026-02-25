#include "graph.h"
#include "onnx_import.h"
#include "onnx.pb.h"

#include <cassert>
#include <iostream>
#include <string>

static int find_value_id(const Graph& g, const std::string& name) {
    auto it = g.value_by_name.find(name);
    if (it == g.value_by_name.end()) return -1;
    return it->second;
}

int main() {
    onnx::ModelProto model;
    model.set_ir_version(8);

    auto* opset = model.add_opset_import();
    opset->set_version(13);

    auto* gr = model.mutable_graph();
    gr->set_name("tiny");

    // graph input X
    {
        auto* x = gr->add_input();
        x->set_name("X");
        auto* tt = x->mutable_type()->mutable_tensor_type();
        tt->set_elem_type(onnx::TensorProto_DataType_FLOAT);
        auto* shape = tt->mutable_shape();
        shape->add_dim()->set_dim_value(1);
        shape->add_dim()->set_dim_value(2);
    }

    // graph output Y
    {
        auto* y = gr->add_output();
        y->set_name("Y");
        auto* tt = y->mutable_type()->mutable_tensor_type();
        tt->set_elem_type(onnx::TensorProto_DataType_FLOAT);
        auto* shape = tt->mutable_shape();
        shape->add_dim()->set_dim_value(1);
        shape->add_dim()->set_dim_value(2);
    }

    // initializer A
    {
        auto* a = gr->add_initializer();
        a->set_name("A");
        a->set_data_type(onnx::TensorProto_DataType_FLOAT);
        a->add_dims(2);
        a->add_dims(2);
        a->add_float_data(1.0f);
        a->add_float_data(0.0f);
        a->add_float_data(0.0f);
        a->add_float_data(1.0f);
    }

    // initializer C
    {
        auto* c = gr->add_initializer();
        c->set_name("C");
        c->set_data_type(onnx::TensorProto_DataType_FLOAT);
        c->add_dims(2);
        c->add_float_data(0.1f);
        c->add_float_data(0.2f);
    }

    //node  MatMul(X, A) -> AX
    {
        auto* n = gr->add_node();
        n->set_op_type("MatMul");
        n->set_name("matmul0");
        n->add_input("X");
        n->add_input("A");
        n->add_output("AX");
    }

    //node Add(AX, C) -> Y
    {
        auto* n = gr->add_node();
        n->set_op_type("Add");
        n->set_name("add0");
        n->add_input("AX");
        n->add_input("C");
        n->add_output("Y");
    }

    Graph g = onnxToGraph(model);

    assert(g.nodes.size() == 2);

    int idX  = find_value_id(g, "X");
    int idA  = find_value_id(g, "A");
    int idAX = find_value_id(g, "AX");
    int idC  = find_value_id(g, "C");
    int idY  = find_value_id(g, "Y");

    assert(idX  >= 0);
    assert(idA  >= 0);
    assert(idAX >= 0);
    assert(idC  >= 0);
    assert(idY  >= 0);

    assert(g.values[idX].is_graph_input);
    assert(g.values[idY].is_graph_output);
    assert(g.values[idA].is_initializer);
    assert(g.values[idC].is_initializer);

    assert(g.nodes[0].op_type == "MatMul");
    assert(g.nodes[1].op_type == "Add");

    assert(g.values[idAX].producer == g.nodes[0].id);
    assert(g.values[idY].producer  == g.nodes[1].id);


    bool ax_used_by_add = false;
    for (int vid : g.nodes[1].inputs) {
        if (vid == idAX) ax_used_by_add = true;
    }
    assert(ax_used_by_add);

    assert(g.values[idA].dtype == onnx::TensorProto_DataType_FLOAT);
    assert(g.values[idA].shape.size() == 2);
    assert(g.values[idA].shape[0] == 2);
    assert(g.values[idA].shape[1] == 2);

    std::cout << "==================test_import_onnx passed==================\n";
    return 0;
}
