#pragma once
#include "graph.h"
#include "onnx.pb.h"


Graph onnxToGraph(const onnx::ModelProto& model);
