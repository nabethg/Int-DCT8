import math

def to_q15(x):
    # Precondition: x is in [-1, 1)
    return int(round(x * 32768))

N = 8
indices = [3, 4, 6, 7]

for i in indices:
    angle = i * math.pi / (2 * N)
    c = math.cos(angle)
    s = math.sin(angle)
    print(f"c{i} = cos({i}π/16) = 16'sd{to_q15(c)}")
    print(f"s{i} = sin({i}π/16) = 16'sd{to_q15(s)}\n")
