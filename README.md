# SK_BFORCE2
brute force solver for sudokus
In the sudoku field, checking that a sudoku has one and only one solution is in many tasks a critical code.
This code is a stand alone version of he code in use in my programs.
The code is designed to be run in 64 bits mode with processors having the SSE4 set of instructions (All the recent intel and AMD processors have it).
The source is C++ with option to use SSE2/4 and intrinsic instructions. The code has been written and compiled using Microsoft Visual C++, but it has been verified that it compiles also using gcc (LINUX) compiler.
This code is very similar to previous version described here
http://forum.enjoysudoku.com/3-77us-solver-2-8g-cpu-testcase-17sodoku-t30470.html
The main change is in the guess sequence.
As far as I can see, this code leads to a significant improvement depending on the problem to solve compared to the previous version.
