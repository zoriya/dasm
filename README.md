# 8086 Disassembler

## How to compile

Simply run `make` and you will get a `dasm` executable if you have gcc and gnumake.
To run the program, use `dasm -d <file>` where file should be a 8086 assembled file.

## Explanations

The binary file is read on the main.c file, after that, the `dasm` method
will read the header to get the start/end of the code. It will then iterate on all
instructions and print it. To identify the instructions, we use the current opcode
and find the corresponding instruction in the table contained in `instructions.c`.

Using this information, we can know the name of the instruction, how the arguments
are read and what is the size of the instruction (how many operand there is). This
allow us the pretty print the instruction and continue to the next one by advancing
the pc (program counter) by the size of the instruction.

The instruction's arguments can then be printed in the `print_instruction` method
where it will print every arguments using their addressing mode described in the
instruction table.

## References

Some op-code tables

https://csiflabs.cs.ucdavis.edu/~ssdavis/50/8086%20Opcodes.pdf
http://www.mlsite.net/8086/#oper_b
