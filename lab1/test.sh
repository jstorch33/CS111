function should_fail() {
    result=$?;

    echo -n "==> $1 ";

    if [ $result -lt 1 ]; then
	echo "FAILURE";
	exit 1;
    else
	echo;
    fi
}

function should_succeed() {
    result=$?;

    echo -n "==> $1 ";

    if [ $result -gt 0 ]; then
	echo "FAILURE";
	exit 1;
    else
	echo;
    fi
}

tmp_file=/tmp/foo
tmp_file2=/tmp/foo2
> "$tmp_file"
> "$tmp_file2"


./simpsh \
    --wronly /tmp/foo \
    --verbose \
    --command 0 0 0 echo foo \
    | grep "command 0 0 0 echo foo" > /dev/null
should_succeed "command tracks all command options"


./simpsh --wronly /tmp/foo --command 0 0 0 echo "foo";
grep foo /tmp/foo > /dev/null;
should_succeed "command can write to write only file"


echo "foo" > "$tmp_file"
cat "$tmp_file" | wc -l | grep 1 > /dev/null
should_succeed "the temporary file should have one line"
# the cat of $tmp_file should be empty and not add another line to tmp_file
./simpsh --wronly "$tmp_file" --command 0 0 0 cat "$tmp_file"
cat "$tmp_file" | wc -l | grep 1 > /dev/null
should_succeed "shouldn't be able to write to read only file"


echo "foo" > "$tmp_file"
echo "bar" > "$tmp_file2"

cat "$tmp_file" | grep "foo" > /dev/null
should_succeed "the temporary file should have 'foo'"

cat "$tmp_file2" | grep "bar" > /dev/null
should_succeed "the temporary file 2 should have 'bar'"

# cat of /tmp/foo should end up in the /tmp/file2
./simpsh --rdonly "$tmp_file" --wronly "$tmp_file2" --command 0 1 0 cat "$tmp_file"
cat "$tmp_file2" | grep "foo" > /dev/null && cat "$tmp_file2" | wc -l | grep 1 > /dev/null
should_succeed "should be able to cat from one file to the other (replace bar with foo)"

echo "foo" > "$tmp_file"
cat "$tmp_file" | wc -l | grep 1 > /dev/null
should_succeed "the temporary file should have one line"
# the cat of $tmp_file should be empty and not add another line to tmp_file
./simpsh --wronly "$tmp_file" --command 0 0 0 cat "$tmp_file"
cat "$tmp_file" | wc -l | grep 1 > /dev/null
should_succeed "shouldn't be able to write to read only file"


echo "Success"
