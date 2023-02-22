The program is written in C language using Visual Studio as IDE.
To compile in Visual Studio it is necessary to download the "C/C ++" extension (Microsoft) and have the compiler installed on the PC.

To affirm a reasonable correctness of the result, the "test.txt" input file is provided in which several test cases are meant to check the robustness in the event of input overflow, presence of spaces, empty inputs, pathological cases.

To be easily verifiable, the solution is provided using text files.
Reading and writing is performed using the functionality of the standard C library.
The variables used in the program are grouped in a struct to facilitate initialization (or reset) when necessary, with a single call to function of the standard C library:
void Memsetvoid*address, int value, size_t bytesNum).
The initialization function used, assigns each byte the value provided, starting from the memory address provided, for a number of bytes provided
In this way, it is avoided to initialize each variable individually helping to increase code reliability and readability.
The structure of the main is divided into 3 blocks: initialization of the variables, initial checks for the correct execution of the program, execution of the state machine.
The code is modular, each state is delimited by precise conditions and controls to enable transitions from one state to another.
The condition for writing the output is subject to reaching the end of the input line or the end of the input file.
To process the output, the input file is read one character at a time and for each one the state machine is activated.
This choice makes the mechanism more linear and less expensive in terms of memory allocation, and it is due to the fact that the input length and the input file size are unknown. 
Both also vary according to the user's changes.
However, this involves repeated calls to the file reading and writing functions, which in terms of time are longer.
On the contrary, allocating the entire content of the file in memory with a single call to the reading function involves minor times but greater expenditure in terms of space.