# StereoMatching_SGM_MachineLearning
Augumenting SGM with a random forest

Bachelor Thesis

This project aim was to enrich and improve the SGM stereo algorithm by using machine learning techniques in order to weight the scanlines' contributions to each output point depending on their estimated confidence.

The "GENERATORE TRAIN 01" directory contains the code needed to create a training file.
The "GENERA FORESTA" directory contains the code needed to create a random forest and train it using the previusly generated file
The "PREVEDI01" directory contains the code to effectivly create the disparity maps by using my augmented SGM
