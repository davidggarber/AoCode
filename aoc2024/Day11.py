import re

data = open('day11.txt', mode='rt')
line = data.read().strip()
startStones = line.split(' ')

expanded = {}
counts = [{} for _ in range(76)]

def expand1(n):
  if n == '0':
    return ['1']
  if len(n) % 2 == 0:
    return [str(int(n[:(len(n) >> 1)])), str(int(n[(len(n) >> 1):]))]
  return [str(int(n) * 2024)]

def tryExpand1(n):
  if n in expanded:
    return expanded[n]
  x = expand1(n)
  expanded[n] = x
  counts[1][n] = len(x)
  return x

def countExpandX(n, times):
  if times == 0:
    return 1
  if times == 1:
    tryExpand1(n)
    return counts[1][n]
  list = tryExpand1(n)
  times -= 1
  sum = 0
  for l in list:
    if l in counts[times]:
      sum += counts[times][l]
    else:
      c = countExpandX(l, times)
      counts[times][l] = c
      sum += c
  return sum

for times in range(0, 26):
  fullSum = 0
  for s in startStones:
    fullSum += countExpandX(s, times)
  print("part 1: " + str(times) + ": " + str(fullSum))

fullSum = 0
for s in startStones:
  fullSum += countExpandX(s, 75)
print("part 2: " + str(fullSum))
