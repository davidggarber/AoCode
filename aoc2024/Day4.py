import re

data = open('day4.txt', mode='rt')
lines = data.readlines()

def getChar(x, y):
  if x < 0 or y < 0 or y >= len(lines) or x >= len(lines[y]):
    return '\0'
  return lines[y][x]

def isXmas(x, y, dir):
  str = ''
  for i in range(0,4):
    str += getChar(x + i * dir[0], y + i * dir[1])
  return str == 'XMAS'

def isXmas2(x, y):
  str = ''
  for i in range(-1,2):
    str += getChar(x + i, y + i)
  if str != 'MAS' and str != 'SAM':
    return False
  str = ''
  for i in range(-1,2):
    str += getChar(x + i, y - i)
  return str == 'MAS' or str == 'SAM'

dirs = [[1,0], [-1,0], [0,1], [0,-1], [1,1], [1,-1], [-1,1], [-1,-1]]

count = 0
count2 = 0
for y in range(0, len(lines)):
  for x in range(0, len(lines[y])):
    for d in dirs:
      if isXmas(x, y, d):
        count += 1
    if isXmas2(x, y):
      count2 += 1

print("part 1: " + str(count))
print("part 2: " + str(count2))
