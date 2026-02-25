# ONNX FRONTEND

Проект для чтения моделей в формате .onnx, преобразования модели во внутренний граф и визуализации этого графа с помощью GraphViz.

---


##  Требования

- **CMake ≥ 3.16**
- **C++17** (g++ или clang++)
- **GraphViz** — для визуализации графа
- **protobuf-compiler**
- **libprotobuf-dev**
---



## Структура

```
.
ONNXFRONT/
├── CMakeLists.txt
├── README.md
├── proto/
│   └── onnx.proto
├── include/
│   ├── graph.h              # описания внутренних структур графа
│   ├── parse_attr.h
│   ├── onnx_import.h
│   └── exportdot.h
├── src/
│   ├── main.cpp
│   ├── graph.cpp             # реализация методов Graph'a
│   ├── parse_attr.cpp        # реализация парсинга атрибутов
│   ├── onnx_import.cpp       # реализация импорта модели ONNX в наш граф
│   └── exportdot.cpp         # реализация для экспорта нашего графа в GraphViz
└── tests/
    ├── graph_test.cpp        # тест на внутреннюю структуру графа
    ├── parse_attr_test.cpp   # тест парсинга атрибутов
    ├── onnx_import_test.cpp  # тест считывания графа из onnx
```
---

## Cборка проекта

```bash
rm -rf build
mkdir  build
cd build
cmake ..
cmake --build .
```

После сборки появятся бинарники:

| Бинарник                    | Назначение                                      |
|----------------------------|--------------------------------------------------|
| `./frontend`               | чтение ONNX-модели, построение графа и экспорт в .dot |
| `./test_graph`             | тест на внутреннюю структуру графа                    |
| `./test_parse_attr`        | тест парсинга атрибутов                               |
| `./test_import_onnx`       | тест считывания графа из onnx                         |
---


Для того, чтобы увидеть ваш граф,нужно:
```bash
./frontend ../models/mnist-8.onnx
```
После этого будет создан файл graph.dot.
Теперь же для визуализации надо:
```bash
dot -Tpng graph.dot -o graph.png
```
Или же так
```bash
dot -Tsvg graph.dot -o graph.svg
```
И теперь можно посмотреть на ваш граф

При клонировании репозитория будет уже две ONNX-модели в папке models(для примера)

Есть возможность добавления новой модели.
Для этого в папку models,например,добавьте вашу модель.
И запустить:
```bash
./frontend ../models/your_model.onnx
```

## Что считывается из ONNX

Из graph.input:

- имена входных тензоров графа
- флаг is_graph_input

Из graph.output:
- имена выходных тензоров графа
-флаг is_graph_output

Из graph.initializer:
- имя тензора
- тип данных
- shape, dims
- флаг is_initializer

Из graph.node (операции):

- тип операции
- name (имя ноды)
- входы
- выходы
- атрибуты

Связи в графе:

- producer  (какая нода создаёт Value)
- consumers (какие ноды используют Value)

Поддерживаются операции(Add, Mul, Conv, Relu, MatMul, Gemm ...)
и считываются их атрибуты
