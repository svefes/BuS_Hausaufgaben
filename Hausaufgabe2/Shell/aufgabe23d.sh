#!/bin/bash
sort teamnamen.txt | uniq -c |tr -s " "| awk '{ print $1; }'|sort -g|uniq -c | grep "[1,2,3]$"