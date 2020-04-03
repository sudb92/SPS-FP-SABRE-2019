#!/bin/bash
# File to delete all folders

echo Deleting previously processed output segments in run# "$1" 

rm ./output/histogram_root/combined_hist_run"$1"*.root
rm ./output/histogram_root/histograms_builder_run="$1"*.root
rm ./output/histogram_root/histograms_run="$1"*.root
rm ./output/built_preprocessed_trees/builtFile_"$1"*.root
rm ./output/built_preprocessed_trees/preProcessedTree_"$1"*.root
rm ./output/histogram_pdfs/combined_hist_run"$1"*.pdf
rm ./output/built_preprocessed_trees/sabshiftedFile_temp.root

echo '..done!'
