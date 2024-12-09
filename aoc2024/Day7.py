import re

data = open('day7.txt', mode='rt')
lines = data.readlines()

def calc1(a, b, op):
  if (op % 2) == 0:
    return a + b
  return a * b

def test1(line):
  colon = line.split(': ')
  result = int(colon[0])
  args = [int(g) for g in colon[1].split(' ')]

  for combo in range(0, 1 << len(args)):
    total = calc1(args[0], args[1], combo)
    for c in range(2, len(args)):
      combo = combo >> 1
      total = calc1(total, args[c], combo)  
    if total == result:
      # print(line)
      return total
  return 0

sum = 0
for line in lines:
  val = test1(line)
  sum += val

print("part 1: " + str(sum))


def calc2(a, b, op):
  if (op % 3) == 0:
    return a + b
  if (op % 3) == 1:
    return a * b
  return int(str(a) + str(b))

def test2(line):
  colon = line.split(': ')
  result = int(colon[0])
  args = [int(g) for g in colon[1].split(' ')]

  for combo in range(0, 3 ** len(args)):
    total = calc2(args[0], args[1], combo)
    for c in range(2, len(args)):
      combo = int(combo / 3)
      total = calc2(total, args[c], combo)  
    if total == result:
      # print(line)
      return total
  return 0

sum = 0
for line in lines:
  val = test1(line)
  if val == 0:
    val = test2(line)
  sum += val

print("part 2: " + str(sum))
