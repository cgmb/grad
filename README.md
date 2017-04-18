## Ubuntu Dependencies

    sudo apt install libglfw3-dev pkg-config cmake build-essential ninja-build

## How to Build

    cmake -H. -B.build -G Ninja && cmake --build .build
