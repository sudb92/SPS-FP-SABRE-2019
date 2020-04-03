#!/bin/bash

# Shell script for a sensible execution chain of commands to process incoming data from runs
# Adapted from work developed over Fall2019-Spring2020 by B.Sudarsan, E.Good, K.Macon.
# For queries contact sbalak2@lsu.edu

# Check for existence of directory structure, make directories if needed
test -d output || mkdir output
test -d output/built_preprocessed_trees || mkdir output/built_preprocessed_trees
test -d output/histogram_pdfs || mkdir output/histogram_pdfs
test -d output/histogram_root || mkdir output/histogram_root

#Delete all processed segments in a particular run number
./cleanup.sh $1
#preprocess, eventbuild and generate plots in the chosen run number's folder at ./data/DAQ/run_X/UNFILTERED/compass_run_X*.root
./processRun $1 

ret=$? #Get return value of last executed command, which is the max_segments_compass, the maximum number of segments recorded by compass in that run.

# Prepare a suitable hadd command that merges all the generated histogram files together during processRun into one
runthis1="hadd -f -k output/histogram_root/combined_hist_run$1_seg0_to_$ret.root"

counter=0
while [[ $counter -le $ret ]]
do
  runthis1="${runthis1} output/histogram_root/histograms_run=$1,seg=$counter.root"
  ((counter++))
done

#Execute the hadd command
$runthis1

#Print the histogram ROOT file into a PDF, open it with the pdf-reader of choice
rootprint output/histogram_root/combined_hist_run$1_seg0_to_$ret.root -o output/histogram_pdfs/combined_hist_run$1_seg0_to_$ret.pdf
evince output/histogram_pdfs/combined_hist_run$1_seg0_to_$ret.pdf &
