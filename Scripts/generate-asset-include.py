#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser()

parser.add_argument(
    '--input',
    required=True,
    help='Name of the binary file to read.'
)

parser.add_argument(
    '--output',
    required=True,
    help='Name of the header file to write.'
)

parser.add_argument(
    '--variable',
    required=True,
    help='Variable name to store the data in.'
)

parser.add_argument(
    '--guard',
    required=True,
    help='Name to use in the #ifndef guards.'
)

args = parser.parse_args()

input = args.input
output = args.output
variable = args.variable
guard = args.guard

file = open(input, 'rb')
data = file.read()
file.close()

file = open(output, 'w')

file.write("#ifndef {}\n#define {}\n\n".format(guard, guard))

file.write('unsigned char {}[] = {{\n\t'.format(variable))

column = 0
for i, b in enumerate(data):
    file.write('0x{:02X}, '.format(b))

    column += 1
    if column == 16 and i != len(data) - 1:
        column = 0
        file.write('\n\t')

file.write('\n};\n\n')

file.write('#endif // {}\n'.format(guard))

file.close()