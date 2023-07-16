#!/bin/bash

for (( i=10; i<=1000000; i*=10))
do
    ./src $i
done
