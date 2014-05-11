#!/bin/bash
ls -l | awk '{printf $9 " "; printf $6 " "; printf $7" "; print $8" "}'
