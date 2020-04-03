#!/bin/bash

((counter = $1))
while [ $counter -le $2 ]
do
  ./processRun.sh $counter
  ((counter++))
done
