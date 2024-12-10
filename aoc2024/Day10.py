import re

data = open('day10.txt', mode='rt')
lines = data.readlines()

height = len(lines)
width = len(lines[0].strip())

def inBounds(x, y):
  return x >= 0 and x < width and y >= 0 and y < height
def inBounds2(xy):
  return inBounds(xy[0], xy[1])

def findNines(map):
  nines = {}
  for y in range(0, height):
    for x in range(0, width):
      ch = map[y][x]
      if ch == '9':
        pos = str([x,y])
        s = set()
        s.add(pos)
        nines[pos] = s
  return nines

list = findNines(lines)

def appendSet(prev, x, y, combined):
  pos = str([x,y])
  if pos in prev:
    combined.update(prev[pos])

def findSteps(map, prev, num):
  step = {}
  for y in range(0, height):
    for x in range(0, width):
      ch = map[y][x]
      if ch == num:
        s = set()
        appendSet(prev, x-1, y, s)
        appendSet(prev, x+1, y, s)
        appendSet(prev, x, y-1, s)
        appendSet(prev, x, y+1, s)
        if len(s) > 0:
          pos = str([x,y])
          step[pos] = s
  return step

num = 9
while num > 0:
  num -= 1
  list = findSteps(lines, list, str(num))

sum = 0
for zero in list:
  sum += len(list[zero])

print("part 1: " + str(sum))


def countRating(prev, x, y):
  pos = str([x,y])
  if pos in prev:
    return prev[pos]
  return 0

def findRating(map, prev, num):
  step = {}
  for y in range(0, height):
    for x in range(0, width):
      ch = map[y][x]
      if ch == num:
        rat = countRating(prev, x-1,y)
        rat += countRating(prev, x+1,y)
        rat += countRating(prev, x,y-1)
        rat += countRating(prev, x,y+1)
        if rat > 0:
          pos = str([x,y])
          step[pos] = rat
  return step

list2 = findNines(lines)
for n in list2:
  list2[n] = 1
num = 9
while num > 0:
  num -= 1
  list2 = findRating(lines, list2, str(num))

sum = 0
for zero in list2:
  sum += list2[zero]

print("part 2: " + str(sum))
