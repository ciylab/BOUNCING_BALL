import math

rayon = 6
print('int direction[10][2] = {')
for i in range(10):
    alpha = i * math.pi / 18
    print('    {', round(rayon * math.cos(alpha)), ',', 
        round(rayon * math.sin(alpha)), '}', end = '')
    if i < 9:
        print(', ')
    else:
        print('\n};')

