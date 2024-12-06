import re

data = open('day6.txt', mode='rt')
lines = data.readlines()

def getChar(pos):
  if pos[0] < 0 or pos[1] < 0 or pos[1] >= len(lines) or pos[0] >= len(lines[pos[1]]):
    return '\0'
  return lines[pos[1]][pos[0]]

def move(pos, dir):
  return [pos[0] + dir[0], pos[1] + dir[1]]

def turn(dir):
  return [-dir[1], dir[0]]

guard = [0,0]
while guard[1] < len(lines):
  line = lines[guard[1]]
  if ('^' in line):
    while guard[0] < len(line):
      if line[guard[0]] == '^':
        # lines[guard[1]] = line[:guard[0]] + ['.'] + line[(guard[0] + 1):]
        break
      guard[0] += 1
    break
  guard[1] += 1
start = guard

def traceRoute(obstacle):
  seen = set()
  looper = set()
  guard = start
  face = [0,-1]
  ch = getChar(guard)
  while ch != '\0':
    seen.add(str(guard))
    key = str(guard + face)
    if key in looper:
      return True
    looper.add(key)
    next = move(guard, face)
    ch = getChar(next)
    if ch == '#' or next == obstacle:
      face = turn(face)
    else:
      guard = next
  return len(seen)

seen1 = traceRoute([-1,-1])

print("part 1: " + str(seen1))

count2 = 0
for y in range(0, len(lines)):
  print('row ' + str(y))
  for x in range(0, len(lines[0].strip())):    
    obst = [x, y]
    if obst != start:
      loops = traceRoute(obst)
      if loops == True:
        count2 += 1

print("part 2: " + str(count2))
