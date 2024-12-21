import re
import heapq

class PointStep:
  def __init__(self, x, y, s): 
    self.x = x 
    self.y = y
    self.step = s
  def __lt__(self, other):
    return self.step < other.step
  def __eq__(self, other):
    return self.step == other.step
  def key(self):
    return f"{self.x},{self.y}"
  def __repr__(self):
    return f"Point at ({self.x},{self.y}) at step {self.step}"
  def atPos(self, other):
    return self.x == other.x and self.y == other.y  # ignore step

data = open('Day20.txt', mode='rt')
lines = data.readlines()

start = None
end = None
for y in range(len(lines)):
  line = lines[y]
  if 'S' in line:
    start = PointStep(line.index('S'), y, 0)
  if 'E' in line:
    end = PointStep(line.index('E'), y, -1)

def inBounds(pos):
  global lines
  return pos.y >= 0 and pos.y < len(lines) and pos.x >= 0 and pos.x < len(lines[0].strip())
def getAt(pos):
  return lines[pos.y][pos.x]

track = {}
def walkMaze():
  global start, end, track
  path = [start]
  visited = set()
  heapq.heapify(path)
  while len(path) > 0:
    pos = path.pop()
    track[pos.key()] = pos
    if getAt(pos) == 'E':
      end = pos
    visited.add(pos.key())
    u = PointStep(pos.x, pos.y-1, pos.step+1)
    d = PointStep(pos.x, pos.y+1, pos.step+1)
    l = PointStep(pos.x-1, pos.y, pos.step+1)
    r = PointStep(pos.x+1, pos.y, pos.step+1)
    if u.key() not in visited and getAt(u) != '#':
      heapq.heappush(path, u)
    if d.key() not in visited and getAt(d) != '#':
      heapq.heappush(path, d)
    if l.key() not in visited and getAt(l) != '#':
      heapq.heappush(path, l)
    if r.key() not in visited and getAt(r) != '#':
      heapq.heappush(path, r)

def shortcut(p):
  if inBounds(p) and getAt(p) != '#' and p.key() in track:
    t = track[p.key()]
    return t.step - p.step
  return -1  # not a shortcut

walkMaze()

minImprove = 100
count = 0
shorts = []
for pos in track.values():
  u = PointStep(pos.x, pos.y-2, pos.step+2)
  d = PointStep(pos.x, pos.y+2, pos.step+2)
  l = PointStep(pos.x-2, pos.y, pos.step+2)
  r = PointStep(pos.x+2, pos.y, pos.step+2)
  sc = [shortcut(u), shortcut(d), shortcut(l), shortcut(r)]
  for s in sc:
    if s >= minImprove:
      shorts.append(s)
      count += 1

shorts.sort()
print(shorts)
print(f"part 1: {count}")


count = 0
for pos in track.values():
  for dy in range(-20, 21):
    for dx in range(-20 + abs(dy), 21 - abs(dy)):
      p = PointStep(pos.x + dx, pos.y + dy, pos.step + abs(dx) + abs(dy))
      if inBounds(p) and getAt(p) != '#':
        t = track[p.key()]
        short = t.step - p.step
        if short >= 100:
          count += 1
print(f"part 2: {count}")
