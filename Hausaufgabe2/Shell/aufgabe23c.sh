#!/bin/bash
sort teamnamen.txt | uniq -c | grep "^      3" | wc -l