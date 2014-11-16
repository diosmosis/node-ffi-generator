SCRIPT_DIR=`dirname $0`

mkdir -p $SCRIPT_DIR/../../build/smoke-test-out
$SCRIPT_DIR/../../build/node-ffi-generator --log-level=DEBUG --file="$SCRIPT_DIR"/input.h --src-root="$SCRIPT_DIR" --dest="$SCRIPT_DIR"/../../build/smoke-test-out