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

data = open('Day18.txt', mode='rt')
lines = data.readlines()

corrupted = set()

wwidth = 70
hheight = 70
firststeps = 1024
# wwidth = 6
# hheight = 6
# firststeps = 12

for i in range(0,firststeps):
  corrupted.add(lines[i].strip())

def drawMaze(width, height):
  for y in range(height+1):
    line = ''
    for x in range(width+1):
      if f"{x},{y}" in corrupted:
        line += '#'
      else:
        line += '.'
    print(line)

drawMaze(wwidth,hheight)

def solveMaze(width, height):
  global corrupted
  start = PointStep(0,0,0)
  dest = PointStep(width,height,-1)
  hlist = [start]  # pos 0,0 at step 0
  heapq.heapify(hlist)
  seen = set()
  seen.add(start.key())

  while len(hlist) > 0:
    pos = heapq.heappop(hlist)
    # print(pos)
    if pos.atPos(dest):
      return pos.step
    u = PointStep(pos.x, pos.y-1, pos.step+1)
    d = PointStep(pos.x, pos.y+1, pos.step+1)
    l = PointStep(pos.x-1, pos.y, pos.step+1)
    r = PointStep(pos.x+1, pos.y, pos.step+1)
    if u.y >= 0 and u.key() not in seen and u.key() not in corrupted:
      seen.add(u.key())
      heapq.heappush(hlist, u)
    if d.y <= height and d.key() not in seen and d.key() not in corrupted:
      seen.add(d.key())
      heapq.heappush(hlist, d)
    if l.x >= 0 and l.key() not in seen and l.key() not in corrupted:
      seen.add(l.key())
      heapq.heappush(hlist, l)
    if r.x <= width and r.key() not in seen and r.key() not in corrupted:
      seen.add(r.key())
      heapq.heappush(hlist, r)
  return -1

part1 = solveMaze(wwidth,hheight)
print(f"part 1: {part1}")

def part2():
  global corrupted
  # TODO: do a binary search!
  for c in range(firststeps, len(lines)):
    corrupted.add(lines[c].strip())

    steps = solveMaze(wwidth,hheight)
    if steps < 0:
      return c
    print(f"{c} corrupted -> {steps}")
  return -len(lines)

part2 = part2()

drawMaze(wwidth,hheight)

print(f"part 2: {lines[part2]}")

