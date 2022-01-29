# parser-generator
Produces C++ LL(1) parser of some user grammar.

To generate parser you need to build `gen` target and run it with:

`./gen "path/to/grammar.txt" "path/to/output_dir"`

 Also check out `run-*` and `test-*` targets in `CMakeList.txt`.
