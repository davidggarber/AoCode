import re

data = open('day8.txt', mode='rt')
lines = data.readlines()

height = len(lines)
width = len(lines[0].strip())

def inBounds(x, y):
  return x >= 0 and x < width and y >= 0 and y < height
def inBounds2(xy):
  return inBounds(xy[0], xy[1])

def addMul(xy, dx, dy, mul):
  return [xy[0] + dx * mul, xy[1] + dy * mul]

antenae = {}

for y in range(0, height):
  line = lines[y]
  for x in range(0, width):
    ch = line[x]
    if ch != '.':
      if ch not in antenae:
        antenae[ch] = []
      antenae[ch].append([x,y])

antinodes = set()

for ch in antenae.keys():
  list = antenae[ch]
  for i in range(0, len(list) - 1):
    for j in range(i + 1, len(list)):
      dx = list[i][0] - list[j][0]
      dy = list[i][1] - list[j][1]
      an1 = addMul(list[i], dx, dy, 1)
      an2 = addMul(list[j], dx, dy, -1)
      if inBounds2(an1):
        antinodes.add(str(an1))
      if inBounds2(an2):
        antinodes.add(str(an2))

print("part 1: " + str(len(antinodes)))

antinodes = set()

for ch in antenae.keys():
  list = antenae[ch]
  for i in range(0, len(list) - 1):
    for j in range(i + 1, len(list)):
      dx = list[i][0] - list[j][0]
      dy = list[i][1] - list[j][1]
      mul = 0
      an = list[i]
      while inBounds2(an):
        antinodes.add(str(an))
        mul += 1
        an = addMul(list[i], dx, dy, mul)
      an = list[j]
      mul = 0
      while inBounds2(an):
        antinodes.add(str(an))
        mul -= 1
        an = addMul(list[j], dx, dy, mul)

print("part 2: " + str(len(antinodes)))
