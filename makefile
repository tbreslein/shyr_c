alias b := build
alias r := run

build: shyr/**/*
	meson setup build .
	ln -sf build/compile_commands.json ./compile_commands.json
	ninja -C build

run: build
	build/$(ex)

clean:
	rm -fr build

.PHONY: run clean
