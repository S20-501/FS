#!/bin/bash

tee test1.cin < input.txt | ./FS_main &> test1.cout
perl -i -p -e 's/FS> FS> /\n/; s/FS> //' ./test1.cout
