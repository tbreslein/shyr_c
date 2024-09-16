all: build/examples/all

build/examples/%: shyr/**/*
	meson setup build . -Dexamples=$*
	ln -sf build/compile_commands.json ./compile_commands.json
	ninja -C build

run_ex: build/examples/$(ex)
	build/examples/$(ex)
rex: run_ex

clean:
	rm -fr build

.PHONY: all run_ex rex clean
