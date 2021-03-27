# Normalized Crosscorrelation

This algorithm calculates the normalized cross correlation between two signal files. The cross correlation is a measure of the degree to which two different signals are similar. A higher cross correlation means a higher degree of similarity and vice versa. Its main difference from correlation is that there is a time lag involved as compared to a correlation in which two signals are considered to be in the same time. 

## Project Specifications
1. This algorithm takes in two different files, one for x, and one for y, and determines the starting index for the crosscorrelation. When no starting index is given, assume that the starting index is 0. 
2. After obtaining the raw signal files, this algorithm subtracts the average of each signal file to get a signal consisting of subtracted averages. 
3. Calculate the cross-correlation based on the starting index and the end indexes of the signal files. 
4. Output the results to a separate text file.

## Inputs 
There are ten different inputs in this project:
1. x1.txt
2. x2.txt
3. x3.txt
4. x4.txt
5. y1.txt
6. y2.txt
7. y3.txt
8. y4.txt
9. output1.txt
10. output2.txt

## Outputs 
The ten inputs result into one output per x and y input combination:
1. output1.txt (output of x1.txt and y1.txt)
2. output2.txt (output of x2.txt and y2.txt)
3. output3.txt (output of x3.txt and y3.txt)
4. output4.txt (output of x4.txt and y4.txt)
5. output5.txt (output of output1.txt and output2.txt)

Output5.txt is shown to emphasize that even the outputs of this algorithm can be used as inputs for the succeeding iterations of the normalized cross correlation.

Each function used has been given documented within the .cpp file. Enjoy!

