if [ ! -d ./build ]; then
    echo "ERROR: ./build directory not found"
    exit 2
fi

echo
echo "Test Suite"
echo

status=0
for test in ./build/bin/test_*; do
    if [ -x "$test" ]; then
        echo "Running Test: $test"
        $test
        last_status=$?
        if [ $last_status -ne 0 ]; then
            echo "Test $test failed with status $last_status"
        else
            echo "Ok"
        fi
        status=$(($status + $last_status))
    fi
done

echo
if [ $status -ne 0 ]; then
    echo "Test Suite Failed"
else
    echo "Test Suite Succeded"
fi

exit $status
