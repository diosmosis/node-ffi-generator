SCRIPT_DIR=`dirname $0`

mkdir -p $SCRIPT_DIR/../../build/smoke-test-out
$SCRIPT_DIR/../../build/Release/node-ffi-generator --log-level=DEBUG --file="$SCRIPT_DIR"/input.h --src-root="$SCRIPT_DIR" --dest="$SCRIPT_DIR"/../../build/smoke-test-out --include="/usr/lib/gcc/x86_64-linux-gnu/4.8/include/" --include="/usr/include"