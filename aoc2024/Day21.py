import re

numpad = ["789","456","123"," 0A"]
arrowpad = [" ^A","<v>"]
arrowDir = {'<':[-1,0], '>':[1,0], '^':[0,-1], 'v':[0,1]}
numbers = "0123456789A"
arrows = "<>^vA"
numToPos = {}
arrToPos = {}

for y in range(len(numpad)):
  for x in range(len(numpad[y])):
    n = numpad[y][x]
    numToPos[n] = [x,y]

for y in range(len(arrowpad)):
  for x in range(len(arrowpad[y])):
    n = arrowpad[y][x]
    arrToPos[n] = [x,y]

deltas = {}
sequences = {}
for n in numbers:
  for m in numbers:
    nm = n + m
    fromto = n + m
    mx = numToPos[m][0]
    nx = numToPos[n][0]
    dx = mx - nx
    my = numToPos[m][1]
    ny = numToPos[n][1]
    dy = my - ny
    seq = ''
    if nx == 0 and my == 3:  # from left to bottom
      seq = ('>' * dx) + ('v' * dy)  # right before down, to miss gap
    elif mx == 0 and ny == 3:  # from bottom to left
      seq = ('^' * -dy) + ('<' * -dx)  # up before left, to miss gap
    else:
      # do arrows farthest from A, before those nearer
      # lefts before verticals before rights
      if dx < 0:
        seq += '<' * -dx
      if dy > 0:
        seq += 'v' * dy
      else:
        seq += '^' * -dy
      if dx > 0:
        seq += '>' * dx
    deltas[nm] = [dx,dy]
    sequences[nm] = seq

for n in arrows:
  for m in arrows:
    nm = n + m
    fromto = n + m
    dx = arrToPos[m][0]-arrToPos[n][0]
    dy = arrToPos[m][1]-arrToPos[n][1]
    seq = ''
    if dy < 0 and arrToPos[n][0] == 0:
      seq = ('>' * dx) + ('^' * -dy)  # right before up, to miss gap
    elif dy > 0 and arrToPos[m][0] == 0:
      seq = ('v' * dy) + ('<' * -dx)  # down before left, to miss gap
    else:
      # do arrows farthest from A, before those nearer
      # lefts before verticals before rights
      if dx < 0:
        seq += '<' * -dx
      if dy > 0:
        seq += 'v' * dy
      else:
        seq += '^' * -dy
      if dx > 0:
        seq += '>' * dx
    deltas[nm] = [dx,dy]
    sequences[nm] = seq

    deltas[nm] = [arrToPos[m][0]-arrToPos[n][0],arrToPos[m][1]-arrToPos[n][1]]

data = open('Day21.txt', mode='rt')
lines = data.readlines()

pointers = ['A','A','A','A']
steps = []
# debug = []
memory = {}
def init(depth):
  global pointers, steps, debug
  pointers = []
  steps = []
  # debug = []
  for d in range(0, depth+1):
    pointers.append('A')
    steps.append(0)
    # debug.append('')

def goToNum(level, num):
  global pointers, steps, deltas, sequences, memory
  fromto = pointers[level] + num
  pointers[level] = num
  return sequences[fromto]

def inputCode(code, depth):
  global memory, steps, debug
  memcode = f"{depth},{code}"
  if memcode in memory:
    return memory[memcode]
  # debug[depth] += code + ' '
  depth -= 1
  innerLen = 0
  for ch in code:
    seq = goToNum(depth, ch) + 'A'
    # steps[depth] += len(seq)
    if depth > 0:
      innerLen += inputCode(seq, depth)
    else:
      innerLen += len(seq)
  memory[memcode] = innerLen
  return innerLen


part1 = 0
init(3)
for line in lines:
  code = line.strip()
  length = inputCode(code, 3)
  value = int(code[:-1])
  print(f"{code}: len {length} * {value}")
  part1 += length * value

print(f"part 1: {part1}")

part2 = 0
init(26)
for line in lines:
  code = line.strip()
  length = inputCode(code, 26)
  value = int(code[:-1])
  print(f"{code}: len {length} * {value}")
  part2 += length * value

print(f"part 2: {part2}")
