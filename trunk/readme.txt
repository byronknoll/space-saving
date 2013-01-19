space-saving

Made by Byron Knoll in 2013

http://code.google.com/p/space-saving/

This is a C++ implementation of the "space-saving" algorithm described in:

A. Metwally, D. Agrawal, and A. El Abbadi. Efficient Computation of Frequent and Top-k Elements in Data Streams. In Proceedings of the 10th ICDT International Conference on Database Theory, pages 398–412, 2005.

This project is released in the public domain - you can use the source code however you want.

The example program (runner.cpp) finds the most frequently occurring substrings of length N in a file.

To compile:
	make

Run without parameters to get help:
	./space-saving

Example execution:
	./space-saving file.txt 10 100000 100
