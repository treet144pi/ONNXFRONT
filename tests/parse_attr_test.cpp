#include "parse_attr.h"
#include "onnx.pb.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <variant>


int main() {
    // INt
    {
        onnx::AttributeProto a;
        a.set_name("group");
        a.set_type(onnx::AttributeProto_AttributeType_INT);
        a.set_i(1);

        AttrValue v = parse_attr(a);
        assert(std::holds_alternative<int64_t>(v));
        assert(std::get<int64_t>(v) == 1);
    }

    // FLOAT
    {
        onnx::AttributeProto a;
        a.set_name("alpha");
        a.set_type(onnx::AttributeProto_AttributeType_FLOAT);
        a.set_f(0.5f);

        AttrValue v = parse_attr(a);
        assert(std::holds_alternative<float>(v));
        assert(std::get<float>(v) == 0.5f);
    }

    // STRING
    {
        onnx::AttributeProto a;
        a.set_name("mode");
        a.set_type(onnx::AttributeProto_AttributeType_STRING);
        a.set_s("nearest");

        AttrValue v = parse_attr(a);
        assert(std::holds_alternative<std::string>(v));
        assert(std::get<std::string>(v) == "nearest");
    }

    // INTS
    {
        onnx::AttributeProto a;
        a.set_name("strides");
        a.set_type(onnx::AttributeProto_AttributeType_INTS);
        a.add_ints(1);
        a.add_ints(1);

        AttrValue v = parse_attr(a);
        assert(std::holds_alternative<std::vector<int64_t>>(v));
        const auto& vv = std::get<std::vector<int64_t>>(v);
        assert(vv.size() == 2);
        assert(vv[0] == 1 && vv[1] == 1);
    }

    std::cout << "==================test_parse_attr passed==================\n";
    return 0;
}
