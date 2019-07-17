#!/bin/bash

for i in `seq 0 142`; do
    ./ft_malloc &
	wait $!
	if [ $? -ne 0 ]; then
		echo "Crashed at $i iteration";
		exit 1;
	fi
done

