
KAIST EE209: Programming Structures for Electrical Engineering 
Assignment 2


=========================
Name: Melese Medhin
ID: 20210727
Assignment number no. :  2

=========================

=========================
Is it possible for StrCopy to call assert to verify that the destination memory area specified by the caller is large enough? Explain.

No, StrCopy cannot use assert to guarantee that the destination buffer is large enough. This is because the function receives only a pointer to the destination memory block, not its actual size. Without explicit size information, the function has no way of determining the available memory for the destination.
=======================
(Optionally) Please write the your implementation method in readme text file whether you use pointer notation for the extra credit. 

For the extra credit portion, I utilized pointer notation in my code.

=======================
 This assignment took me approximately four days to finish it.
