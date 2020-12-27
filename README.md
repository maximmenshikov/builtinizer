# builtinizer
Clang builtin definition converter

This tool is a 15 minute tool created to convert Clang's Builtins.def to some
different form. Use at your own risk, PRs welcome if (when) you find issues.


## Build

Put Builtins.def from Clang to src folder. Then build with ``cmake`` as always:

	mkdir -p build
	cd build
	cmake ..
	cmake --build .

## Run

It was created for "--constexpr" mode (getting functions which may be considered
constexpr). Another parameter is "--list" (simple comma-separated list of
matching functions). And the last parameter is "--ignore-half", which makes tool
ignore signatures with half types. So:

	./builtinizer [--constexpr] [--ignore-half] [--list]
