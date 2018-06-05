#!/bin/bash
set -euo pipefail
stty -F /dev/ttyACM0 230400 raw -clocal -echo icrnl
cat /dev/ttyACM0 | ts "%.s" | { 
	stdbuf -oL head -n10000 | tee /tmp/1; 
	while head -n1 | tee >(sed -i '1d;$a'"$(cat)" /tmp/1); do :; done;
}

