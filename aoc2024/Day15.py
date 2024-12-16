import re

data = open('Day15.txt', mode='rt')
lines = data.readlines()


class Point:
  def __init__(self, x, y): 
    self.x = x 
    self.y = y


map = []
steps = None
start = Point(-1,-1)

def getAt(x, y):
  if inBounds(x, y):
    return map[y][x]
  return '\0'
def setAt(x, y, low):
  line = map[y]
  line = line[:x] + low + line[(x+1):]
  map[y] = line

y = 0
for line in lines:
  ls = line.strip()
  if steps != None:
    steps += ls
  elif ls == '':
    steps = []
  else:
    map.append(ls)
    if '@' in line:
      start.y = y
      start.x = line.index('@')
  y += 1

pos = Point(start.x, start.y)

map2 = []
for line in map:
  line2 = ''
  for ch in line:
    if ch == '#':
      line2 += '##'
    if ch == 'O':
      line2 += '[]'
    if ch == '.':
      line2 += '..'
    if ch == '@':
      line2 += '@.'
  map2.append(line2)


height = len(map)
width = len(map[0])

def inBounds(x, y):
  return x >= 0 and x < width and y >= 0 and y < height
def inBounds2(xy):
  return inBounds(xy[0], xy[1])

def pushBox(x, y, dx, dy):
  cur = getAt(x, y)
  x += dx
  y += dy
  ch = getAt(x, y)
  if ch == '#' or ch == '\0':
    return False
  if ch != '.':
    if not pushBox(x, y, dx, dy):
      return False
  setAt(x, y, cur)
  setAt(x - dx, y - dy, '.')
  return True

for s in steps:
  if s == '>':
    if pushBox(pos.x, pos.y, 1, 0):
      pos.x += 1
  elif s == '<':
    if pushBox(pos.x, pos.y, -1, 0):
      pos.x -= 1
  elif s == '^':
    if pushBox(pos.x, pos.y, 0, -1):
      pos.y -= 1
  elif s == 'v':
    if pushBox(pos.x, pos.y, 0, 1):
      pos.y += 1


sum = 0
for y in range(height):
  for x in range(width):
    ch = getAt(x, y)
    if ch == 'O':
      sum += y * 100 + x

print("part 1: " + str(sum))


map = map2
height = len(map)
width = len(map[0])
pos = Point(start.x, start.y)
pos.x = map[pos.y].index('@')

def canPush2(x, y, dx, dy):
  while x >= 0 and x < width and y >= 0 and y < width:
    x += dx
    y += dy
    ch = getAt(x, y)
    if ch == '.':
      return True
    if ch == '#' or ch == '\0':
      return False
    if dy != 0:
      if ch == '[' and not canPush2(x + 1, y, dx, dy):
        return False
      if ch == ']' and not canPush2(x - 1, y, dx, dy):
        return False
  return False

def pushBox2(x, y, dx, dy):
  if not canPush2(x, y, dx, dy):
    return False
  cur = getAt(x, y)
  x += dx
  y += dy
  ch = getAt(x, y)
  if dy != 0:
    if ch == '[':
      pushBox2(x + 1, y, dx, dy)
    elif ch == ']':
      pushBox2(x - 1, y, dx, dy)
  if ch != '.':
    pushBox2(x, y, dx, dy)
  setAt(x, y, cur)
  setAt(x - dx, y - dy, '.')
  return True

for s in steps:
  if s == '>':
    if pushBox2(pos.x, pos.y, 1, 0):
      pos.x += 1
  elif s == '<':
    if pushBox2(pos.x, pos.y, -1, 0):
      pos.x -= 1
  elif s == '^':
    if pushBox2(pos.x, pos.y, 0, -1):
      pos.y -= 1
  elif s == 'v':
    if pushBox2(pos.x, pos.y, 0, 1):
      pos.y += 1


sum = 0
for y in range(height):
  for x in range(width):
    ch = getAt(x, y)
    if ch == '[':
      sum += y * 100 + x

print("part 2: " + str(sum))
