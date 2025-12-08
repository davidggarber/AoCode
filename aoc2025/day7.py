from collections import defaultdict


data = open('day7.txt', mode='rt')
lines = data.readlines()

height = len(lines)
width = len(lines[0].strip())

def get(x, y):
  if y < 0 or y >= height:
    return '.'
  if x < 0 or x >= width:
    return '.'
  return lines[y][x % width]


xS = lines[0].index('S')
tachyons = {xS: 1}

count_splits = 0
for y in range(1, height):
  next_tachyons = defaultdict(int)
  for t,v in tachyons.items():
    if get(t,y) == '^':
      count_splits += 1
      next_tachyons[t-1] += v
      next_tachyons[t+1] += v
    else:
      next_tachyons[t] += v
  tachyons = next_tachyons

print('part 1:', count_splits)

time_lines = 0
for v in next_tachyons.values():
  time_lines += v
print('part 2:', time_lines)