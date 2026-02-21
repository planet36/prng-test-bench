# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: MPL-2.0

"""
Usage/Example:

# Takes about 3.3 mins
time python3 patterned_primes.py > /tmp/patterned_primes.txt

# Takes about 32 secs
time pypy3 patterned_primes.py > /tmp/patterned_primes.PYPY.txt

# about 556K
ls -hop /tmp/patterned_primes*.txt
md5sum /tmp/patterned_primes*.txt

"""

# pylint: disable=invalid-name

import numpy as np
import sympy

# pylint: disable=redefined-outer-name
def rotl(x: int, shift: int, width: int) -> int:
    """
    Rotate-left within `width` bits.

    Parameters
    ----------
    x : int
        Value to rotate.
    shift : int
        Rotation amount (can be any integer).
    width : int
        Bit-width to rotate within (e.g., 8, 16, 32, 64).

    Returns
    -------
    int
        Rotated value in the range [0, 2**width).
    """
    if width <= 0:
        raise ValueError("width must be positive")

    mask = (1 << width) - 1

    x &= mask

    # Normalize shift to be within [0, width)
    shift %= width

    if shift == 0:
        return x

    return ((x << shift) | (x >> (width - shift))) & mask


def has_forbidden_bit_sequence(s: str) -> bool:
    '''Determine if s has a forbidden bit sequence.'''

    #if s.startswith('00'):
    #    return True
    #if s.startswith('11'):
    #    return True
    #if s.endswith('11'):
    #    return True
    if '000' in s:
        return True
    if '111' in s:
        return True

    if '00100' in s:
        return True
    if '11011' in s:
        return True
    #if '001100' in s:
    #    return True
    #if '110011' in s:
    #    return True
    #if '1100' in s:
    #    return True
    #if '0011' in s:
    #    return True

    return False


l = []

for bits in (8, 16, 32):

    print(f'# {bits=}')
    bits_half = bits // 2

    #bits_mask = (2**bits) - 1
    #bits_half_mask = bits_mask >> bits_half

    #start = ((2**bits) >> 2) | 1 # odd
    start = (0b01 << (bits-2)) | 1 # odd

    #end = 2**bits - (start - 1)
    end = 0b11 << (bits-2)

    print(f'# start = {start}  {hex(start)}  0b{start:0{bits}b}')
    print(f'# end   = {end}  {hex(end)}  0b{end:0{bits}b} (exclusive)')

    num_bytes = bits // 8

    num_found_primes = 0

    #for i in range(0, 2**bits):
    #for i in range(2**(bits-1), 2**bits):
    #for i in range(2**(bits-1), 2**bits - (2**bits >> 2)):
    for i in range(start, end, 2):

        if i.bit_count() != bits_half:
            continue

        if not sympy.isprime(i):
            continue

        skip = False

        # Rotate the value to find forbidden bit patterns.
        for shift in range(0, 8):
            i2 = rotl(i, shift, bits)
            bit_count_per_byte = tuple(
                    b.bit_count() for b in i2.to_bytes(num_bytes, byteorder='big', signed=False))

            # Detect too few or too many set bits in each byte.
            if any(n < 4-1 or n > 4+1 for n in bit_count_per_byte):
                skip = True
                break

            if num_bytes > 2:
                # At least one of the bytes must have popcount = 4.
                if all(n != 4 for n in bit_count_per_byte):
                    skip = True
                    break

            s = f'{i2:0{bits}b}'

            if has_forbidden_bit_sequence(s):
                skip = True
                break

        if skip:
            continue

        s = f'{i:0{bits}b}'

        if bits == 32:
            l.append(i)

        print(f'{i=}  {hex(i)=}  0b{s}')
        num_found_primes += 1

    print(f'# {num_found_primes=}')
    print()


bits = 32
for num in (4, 8, 16, 32):

    print(f'# select {num} using numpy linspace')

    selection = np.array(l)[
            np.round(np.linspace(start=0, stop=len(l)-1, num=num, endpoint=True)).astype(int)]

    for i in selection:
        s = f'{i:0{bits}b}'
        i = int(i)
        print(f'{i=}  {hex(i)=}  0b{s}')
    print()
