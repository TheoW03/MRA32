# QtMRA32

a ARM7TDMI-S 32-bit assembler, written in C++

coded by TheoW03

# instructions

```
MOV <Rd>, <const or Register>
ADD <Rd>, <Rn>, <Rm or const>
SUB <Rd>, <Rn>, <Rm or const>
MUL <Rd>, <Rn>, <Rm>
MLA <Rd>, <Rn>, <Rm>, <Rs>
B <BRANCH_NAME>
branch: 
```
also added comments with the "#" 

# Registers

Registers are 

``R0-R12`` general purpose registers 

``R13`` frame pointer (when I impl. it)

``R14`` stack pointer

``R15`` being the PC


can only do Integers and positive numbers.

floating point numbers will be coming soon.

(using my own floating point lib I used for MIPS)

as of right now uses big endian. will implement little endian eventually
