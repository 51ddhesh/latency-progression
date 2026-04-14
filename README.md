## Latency Progression
This repository contains the code snippets and benchmarks for the blog: [Get the Cat out of the Heap](https://51ddhesh.github.io/blog/get-the-cat-out-of-the-heap/).

## Usage

```bash
mkdir build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Relase
cmake --build build -j
```

- Run the benchmark:
```bash
./build/latency_bench
```

### LICENSE
This project is licensed under the Boost Software License (BSL 1). Check the [LICENSE](./LICENSE) file or visit the official [Boost Software License](https://www.boost.org/LICENSE_1_0.txt) page.
