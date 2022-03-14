#!/bin/bash

# script na testovanie#
# @author Timotej Ponek
# xxxxxxxxxxxxxxxxxxxxx


for FILE in testy/parser/simple{,[2-8]}; do ## {,[2-6]} regex for zero or one of character in range 2-6
    #echo $FILE
    ./main <$FILE 2>/dev/null
    ## $? gets the returncode of last run command
    returncode=$?
    if [ "$returncode" -eq "0" ]; then
        continue
    else
        echo "Test $FILE: Expected returncode 1, got $returncode" #returncode
    fi
done

##############################################################

for FILE in testy/tests/lex/characters/*; do
    ./main <$FILE 2>/dev/null
    returncode=$?
    if [ "$returncode" -eq "1" ]; then
        continue
    else
        echo "Test $FILE: Expected returncode 1, got $returncode"
    fi
done

for FILE in testy/tests/lex/comments/*; do
    ./main <$FILE 2>/dev/null
    returncode=$?
    if [ "$returncode" -eq "0" ]; then
        continue
    else
        echo "Test $FILE: Expected returncode 1, got $returncode"
    fi
done

for FILE in testy/tests/lex/numbers/*; do
    ./main <$FILE 2>/dev/null
    returncode=$?
    if [ "$returncode" -eq "0" ]; then
        continue
    else
        echo "Test $FILE: Expected returncode 1, got $returncode"
    fi
done

for FILE in testy/tests/lex/strings/*; do
    ./main <$FILE 2>/dev/null
    returncode=$?
    if [ "$returncode" -eq "0" ]; then
        continue
    else
        echo "Test $FILE: Expected returncode 1, got $returncode"
    fi
done

#######################################################################
for FILE in testy/tests/lex/lexErr/*; do
    ./main <$FILE 2>/dev/null
    returncode=$?
    if [ "$returncode" -eq "1" ]; then
        continue
    else
        echo "Test $FILE: Expected returncode 1, got $returncode"
    fi
done

######################################################################
# tieto testy su ok