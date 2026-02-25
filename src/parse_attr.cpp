#include "parse_attr.h"
#include <stdexcept>

AttrValue parse_attr(const onnx::AttributeProto& a) {
    switch (a.type()) {
        case onnx::AttributeProto_AttributeType_INT:
            return static_cast<int64_t>(a.i());

        case onnx::AttributeProto_AttributeType_FLOAT:
            return a.f();

        case onnx::AttributeProto_AttributeType_STRING:
            return a.s();

        case onnx::AttributeProto_AttributeType_INTS: {
            std::vector<int64_t> v;
            v.reserve(a.ints_size());
            for (int i = 0; i < a.ints_size(); ++i) {
                v.push_back(a.ints(i));
            }
            return v;
        }

        case onnx::AttributeProto_AttributeType_FLOATS: {
            std::vector<float> v;
            v.reserve(a.floats_size());
            for (int i = 0; i < a.floats_size(); ++i) {
                v.push_back(a.floats(i));
            }
            return v;
        }

        case onnx::AttributeProto_AttributeType_STRINGS: {
            std::vector<std::string> v;
            v.reserve(a.strings_size());
            for (int i = 0; i < a.strings_size(); ++i) {
                v.push_back(a.strings(i));
            }
            return v;
        }

        default:
            throw std::runtime_error("unsupported onnx atrtribute type");
    }
}
