MOV R0, 255
MOV R1, 254

MUL R5, R1, R0
CMP R1, R0
BGT L1

main:
    MOV R4, 1
L1:
    MOV R3, 2