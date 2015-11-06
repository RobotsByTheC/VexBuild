#!/bin/bash

# Look for "python3"
if [ -z "$python" ]; then
	if [[ $(type -P "python3") ]]; then
		python=python3
	fi
fi

# If "python3" could not be found, check whether "python" can be found.
if [[ $(type -P "python") ]]; then
	# Check python version
	ret=$(python -c 'import sys; print("%i" % (sys.hexversion>0x03000000))')
	if [ $ret -eq 1 ] && [ $? -eq 0 ]; then
		python=python
	fi
fi

# If Python was found, execute it
if [ -n "$python" ]; then
	$python "$@"
else
	>&2 echo "Error: Could not run python, is you PATH configured properly?"
	exit 1
fi
