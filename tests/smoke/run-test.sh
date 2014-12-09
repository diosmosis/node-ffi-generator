SCRIPT_DIR=`dirname $0`

if ! $CC $SCRIPT_DIR/input.c -o $SCRIPT_DIR/libtest.so -shared "-DMY_PREDEFINE=struct dependent" -fPIC; then
    exit 1;
fi

echo "Built test lib!"

mkdir -p $SCRIPT_DIR/out
if ! $SCRIPT_DIR/../../build/Release/node-ffi-generator --file="$SCRIPT_DIR"/input.h --file="$SCRIPT_DIR/dir/other-header.h" --log-level=DEBUG --src-root="$SCRIPT_DIR" --dest="$SCRIPT_DIR"/out --include="$HOME/Tools/llvm/Release/lib/clang/3.6.0/include/" --include="/usr/include" "-DMY_PREDEFINE=struct dependent"; then
    exit 1;
fi

echo "JS generated!"

if ! node $SCRIPT_DIR/test-generated.js "$SCRIPT_DIR/libtest.so"; then
    exit 1;
fi

echo "JS works!"