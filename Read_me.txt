                                                                     **PROJECT**
                                                              CS204 - COMPUTER ARCHITECTURE
                                                                                                                                               TEAM MEMBERS:-
                                                                                                                                                             SUMIT TYAGI  (2022CSB1131)
                                                                                                                                                             MANISH ANWLA (2022CSB1090)
                                                                                                                                                             CHETAN PRAKASH DUDI (2022CSB1074)
    

         
SOME POINTS :-
	          1. OUR PROGRAM SUPPORT REGISTER NAME STRATING WITH X (EX: X0,X10,X31) NOT SUPPORT FORMATE OF SP,RA,RS 
		  2. LABEL NAME AND VARIABLE NAME ONLY HAVE CHARACTER [a-zA-Z0-9_.]
		  3. FORMATES SUPPORTED BY THE CODE IS MENTIONED IN my_Format.txt FILE


EXPLANATION:- 
		1. DATA MEMORY PART: 
                                     ->We have declared a function named as "getinver" which is returning 2's complement 
                                     ->We have declared a function named as "convert" which is returning hexadecimal representation of a 
                                       given string and taking string and the length upto which we want it's hexadecimal form.
                                     ->We have declared a function nmaed as "read_data" and this function has regression expressions for 
                                       comparing formats of word,byte,half word and asciiz.
                                     ->And then we are storing data part of byte where we first comparing the range for byte part and    
                                       then we are storing hexadecimal form in the memory.
                                     ->Similarly repeat the whole part for word,half word and asciiz.                   

                2. SYNTAX AND SEMANTICS PART:
					     ->The code processes each line of assembly-like code sequentially.
					     ->It tokenizes each line and matches the first token against predefined instruction formats.
					     ->Specific parsing logic is applied based on the matched instruction format.
					     ->Checks are performed within each instruction format block, including:
							1.Verifying the correct number of arguments.
							2.Validating register numbers.
							3.Ensuring immediate values are within a valid range.
					     ->If any syntax error is encountered, the program exits with an error message indicating the line number.
					     ->The code ensures that the assembly code follows the expected syntax.
					     ->Syntax errors encountered during parsing are flagged appropriately.

               3. MACHINE CODE CONVERTION PART: 
                                                ->The code defines several functions to convert assembly instructions into machine instructions.
						->There are functions for different types of instructions, such as R-type, I-type, S-type, SB-type, U-type, and UJ-type.
						->Each function takes an index i to access the specific instruction from a global array code.
						->Depending on the instruction format (Format[code[i].indexs].format), the appropriate conversion function is called.
						->For R-type instructions, the function R_machine_code constructs the machine code by concatenating opcode, function fields, and 							binary representations of register arguments.
						->Similar procedures are followed for other instruction types, with different bit manipulations and field assignments.
						->After constructing the machine code, the code converts it into hexadecimal format using the bin2hex function.
						->Finally, the hexadecimal machine code is stored in a file named "MCode.mc" along with the corresponding memory address.
						->The assembly_to_machine function iterates over all assembly instructions and converts each one to machine code accordingly.