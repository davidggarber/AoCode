import re
import heapq

data = open('Day16.txt', mode='rt')
lines = data.readlines()


class Point:
  def __init__(self, x, y): 
    self.x = x 
    self.y = y
  def add(self, pt):
    return Point(self.x + pt.x, self.y + pt.y)

class PointDir:
  def __init__(self, x, y, dx, dy, pts, prev): 
    self.x = x 
    self.y = y
    self.dx = dx
    self.dy = dy
    self.pts = pts
    self.prev = [prev]
  def __lt__(self, other):
    return self.pts < other.pts
  def __eq__(self, other):
    return self.pts == other.pts
  def arrow(self):
    if self.dx < 0:
      return '<'
    if self.dx > 0:
      return '>'
    if self.dy < 0:
      return '^'
    if self.dy > 0:
      return 'v'
    return '#'
  def key(self):
    return f"{self.x},{self.y}" + self.arrow()
  def __repr__(self):
    return f"Point at ({self.x},{self.y}), facing {self.arrow()}, score {self.pts}"

  def move(self):
    return PointDir(self.x + self.dx, self.y + self.dy, self.dx, self.dy, self.pts + 1, self)
  def turn(self, cw):
    if cw:
      return PointDir(self.x, self.y, -self.dy, self.dx, self.pts + 1000, self)  # right turn
    return PointDir(self.x, self.y, self.dy, -self.dx, self.pts + 1000, self)  # left turn


map = []

def getAt(x, y):
  return map[y][x]


start = PointDir(-1,-1, 1, 0, 0, None)
end = Point(-1,-1)
y = 0
for line in lines:
  ls = line.strip()
  map.append(ls)
  if 'S' in line:
    start.y = y
    start.x = line.index('S')
  if 'E' in line:
    end.y = y
    end.x = line.index('E')
  y += 1

posList = [start]
heapq.heapify(posList)
seen = {start.key():start}
solved = None

def addToQueue(ptDir):
  global solved
  if solved is None and ptDir.x == end.x and ptDir.y == end.y:
    solved = ptDir
  if ptDir.key() not in seen:
    if getAt(ptDir.x, ptDir.y) != '#':
      seen[ptDir.key()] = ptDir
      heapq.heappush(posList, ptDir)
  elif seen[ptDir.key()].pts == ptDir.pts:
    seen[ptDir.key()].prev += ptDir.prev
    print(seen[ptDir.key()])

while len(posList) > 0:
  pos = heapq.heappop(posList)
  if solved is not None and pos.pts > solved.pts:
    break
  m = pos.move()
  tl = pos.turn(False)
  tr = pos.turn(True)
  addToQueue(m)
  addToQueue(tl)
  addToQueue(tr)

print("part 1: " + str(solved.pts))

path = set()
def followPath(ptDir):
  path.add(str([ptDir.x,ptDir.y]))
  for p in ptDir.prev:
    if p is not None:
      followPath(p)

followPath(solved)

print("part 2: " + str(len(path)))
