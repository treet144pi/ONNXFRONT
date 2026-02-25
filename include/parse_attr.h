#pragma once
#include "onnx.pb.h"
#include <variant>

using AttrValue = std::variant<
  int64_t,
  float,
  std::string,
  std::vector<int64_t>,
  std::vector<float>,
  std::vector<std::string>
>;
using AttrMap = std::unordered_map<std::string, AttrValue>;

AttrValue parse_attr(const onnx::AttributeProto& a);
