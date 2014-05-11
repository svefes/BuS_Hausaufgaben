#!/bin/bash

watch -n $1  "if (ps $2) 
then
echo "The Process with PID $2 is running" 
else
echo "The Process with PID $2 is not running" 
fi"
