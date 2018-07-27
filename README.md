Design for Testability - detection of faults in Programmable Logic Arrays
====
In this project, we generate a set of test patterns to detect all the single stuck-at faults and all the single crosspoint faults for PLA's using the method described by Smith [1]. A logical fault model is proposed for PLA's, which can be divided into four classes: growth faults (G), disappearance faults (D), shrinking faults (S), and appearance faults (A). A test set that detected all detectable G, D, S, and A faults also detects all the detectable stuck-at faults except in PLA's where either of the following conditions holds: 1. some AND gate(s) are redundant; i.e., they can be deleted from the array without affecting the functions realized; 2. if there exists an OR gate output which is normally 1 whenever any AND gate output in the array is 1.

- James E. Smith, "Detection of faults in programmable logic arrays," IEEE Transactions on Computers, vol. C-28, no. 11, pp.845-852 Nov. 1979.
