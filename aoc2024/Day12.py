import re

data = open('Day12.txt', mode='rt')
lines = data.readlines()

height = len(lines)
width = len(lines[0].strip())

def inBounds(x, y):
  return x >= 0 and x < width and y >= 0 and y < height
def inBounds2(xy):
  return inBounds(xy[0], xy[1])

def getAt(x, y):
  if inBounds(x, y):
    return lines[y][x]
  return '\0'
def setAt(x, y, low):
  line = lines[y]
  line = line[:x] + low + line[(x+1):]
  lines[y] = line

regions = []
region2 = []

def growRegion(x, y, ch, data):
  low = str(ch).lower()
  if getAt(x, y) == low:
    return True
  if getAt(x, y) == ch:
    setAt(x, y, low)
    data[0] += 1  # area
    pos = y * 1000 + x
    if not growRegion(x-1, y, ch, data):
      data[1] += 1  # perimeter
      data[2].add(pos)
    if not growRegion(x+1, y, ch, data):
      data[1] += 1  # perimeter
      data[3].add(pos)
    if not growRegion(x, y-1, ch, data):
      data[1] += 1  # perimeter
      data[4].add(pos)
    if not growRegion(x, y+1, ch, data):
      data[1] += 1  # perimeter
      data[5].add(pos)
    return True
  return False

def calcSides(data):
  sides = data[1]  # perim
  for l in data[2]:
    y = int(l / 1000)
    x = l % 1000
    if ((y+1)*1000 + x) in data[2]:
      sides -= 1
  for l in data[3]:
    y = int(l / 1000)
    x = l % 1000
    if ((y+1)*1000 + x) in data[3]:
      sides -= 1
  for t in data[4]:
    y = int(t / 1000)
    x = t % 1000
    if (y*1000 + (x+1)) in data[4]:
      sides -= 1
  for t in data[5]:
    y = int(t / 1000)
    x = t % 1000
    if (y*1000 + (x+1)) in data[5]:
      sides -= 1
  return sides

def findRegion(x, y):
  ch = getAt(x, y)
  if ch != ch.lower():
    data = [0,0,set(),set(),set(),set()]  # area, perim, lefts, rights, tops, bottoms
    growRegion(x, y, ch, data)
    regions.append(data)

    sides = calcSides(data)
    region2.append(data[0] * sides)

for y in range(height):
  for x in range(width):
    findRegion(x, y)

sum = 0
for r in regions:
  sum += r[0] * r[1]

print("part 1: " + str(sum))

sum = 0
for r in region2:
  sum += r
print("part 2: " + str(sum))
