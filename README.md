3dpuzzle
========

A simple solver for the very fun game called 3d logic 
http://mypuzzle.org/3d-logic

Right now it only supports 3x3 matrix (fast) or 5x5 matrix with Bruteforce (slow!)
The only algorithm right now is the brute force solver. 
Here's how the solver works

1. Read the puzzle from the file. A puzzle is made of 3 matrices (3x3 matrices for 3x3 puzzles and ...)
2. Read a graph.txt file that describes how to make the puzzle into a graph. It's the easiest way I found to do this without too much code.

The Solver
==========

Here's a few notes on how the solver works.
First, the graph is made without including the "walls". They are discarded at the graph reading stage actually.
When a graph of the puzzle is made, the following data structures are available :
- A vector of cells
- A graph of these cells

The graph of the cells is used by the Verification algorithm.
The verification algorithm is a recursive graph search algorithm done for each color. If each color checks out, then the cube is correct.
Maybe a better faster verification can be done! Work to be done here.

The only solver available right now is a brute force solver.
- A list of the "mutable" cells is made from the whole list of cells
- A short recursive function goes through this list and tries all the colors. Complexity for a 6 mutable cells with 4 color problem is 4^6
You can see how for big 5x5 or 6x6 cubes this approach will not work :)

That's all for now!
