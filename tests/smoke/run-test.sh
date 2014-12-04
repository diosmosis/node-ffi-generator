SCRIPT_DIR=`dirname $0`

mkdir -p $SCRIPT_DIR/tests/smoke/out
$SCRIPT_DIR/../../build/Release/node-ffi-generator --file="$SCRIPT_DIR"/input.h --file="$SCRIPT_DIR/dir/other-header.h" --log-level=DEBUG --src-root="$SCRIPT_DIR" --dest="$SCRIPT_DIR"/tests/smoke/out --include="$HOME/Tools/llvm/Release/lib/clang/3.6.0/include/" --include="/usr/include" "-DMY_PREDEFINE=struct dependent"