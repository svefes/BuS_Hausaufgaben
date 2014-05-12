#!/bin/bash
# Alle Dreiergruppen
sort teamnamen.txt | uniq -c |tr -s " "| grep "^ 3"|wc -l

# Alle Zweiergruppen
sort teamnamen.txt | uniq -c |tr -s " "| grep "^ 2"|wc -l

# Alle Einergruppen
sort teamnamen.txt | uniq -c |tr -s " "| grep "^ 1"|wc -l

#Keine Gruppe
sort teamnamen.txt | uniq -c | grep "^ *[0-9]* *$"