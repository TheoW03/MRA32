# QtMRA32

ARMv7 32 bit emulator


# Instructions

```
CMP <RN>, <const or register>
MOV <Rd>, <const or Register>
ADD <Rd>, <Rn>, <Rm or const>
SUB <Rd>, <Rn>, <Rm or const>
MUL <Rd>, <Rn>, <Rm>

MLA <Rd>, <Rn>, <Rm>, <Rs> # MULTIPLY accumalte
B <BRANCH_NAME> # this is a comment
branch: 
ORR <Rd>, <Rn>, <Rm or const> # bitwise ORR not logical OR
AND <Rd>, <Rn>, <Rm or const> # bitwise AND not logical AND

```

also added comments with the "#" they follow the MIPS and numbers are like this


# Conditionally Executed Instructions


this will follow the CPSR flags which set by adding a "S" prefix or CMP

```
EQ <Rn>, <Rm or const> (Z_FLAG = 1)
NE <Rn>, <Rm or const> (Z_FLAG = 0)
LE <Rn>, <Rm or const> (N_FLAG =  1 OR Z_FLAG = 1)
GE <Rn>, <Rm or const> (N_FLAG = 0 OR Z_FLAG = 1)
LT <Rn>, <Rm or const> (N_FLAG = 1)
GT <Rn>, <Rm or const> (N_FLAG = 0)

```

you can edit the flags

# Registers

``R0-R12`` general purpose registers 

``R7`` sys call register

``R13`` stack pointer (when i impl it)

``R14`` Link  register (when I impl. it)

``R15`` being the PC

# Other Technical Details

can only do Integers and positive numbers. and numbers up to 2^12 or 4096 

if a number is not a power of 2, and is greater than 4096. then do "ORR" to the nearest power of 2

example: 

```
MOV R0, 5000
ORR R0, R0, 4096
```


if this results in a "wierd" number, its one of the few numbers the ROR works with, if it does. you dont need the
ORR

floating point numbers will be coming soon.

(using my own floating point lib I used for MIPS)

as of right now uses big endian. will implement little endian eventually

(not a full 1:1 emulation of an ARM CPU)
