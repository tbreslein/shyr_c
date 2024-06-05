build_clang_dir := "build_clang"
build_gcc_dir := "build"

config:
    CC=clang meson setup {{ build_clang_dir }} .
    CC=gcc meson setup {{ build_gcc_dir }} .
    ln -sf {{ build_clang_dir }}/compile_commands.json ./compile_commands.json

build:
    ninja -C {{ build_gcc_dir }}

run ex:
    {{ build_gcc_dir }}/{{ ex }}

clean:
    rm -fr {{ build_clang_dir }}
    rm -fr {{ build_gcc_dir }}
