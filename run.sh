#!/bin/bash

for $f in `ls test/*.c`; do
	make test TEST=$f;
done

if [ -n $1 ] ; then
	[ ! -f $1 ] && exit 255;
else
	exit 254;
fi

fails=0

for i in $(seq 1 9000); do
	$1 >/dev/null
	wait $!
	[ $? != 0 ] && fails=$((fails + 1))
	echo "Pass $i"
done

echo "Failed $fails times"
