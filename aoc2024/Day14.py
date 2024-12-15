import re

data = open('Day14.txt', mode='rt')
lines = data.readlines()

class Point:
  def __init__(self, x, y): 
    self.x = x 
    self.y = y

# width = 11
# height = 7
width = 101
height = 103

class Robot:
  def __init__(self, pos, vel): 
    self.x = int(pos[0])
    self.y = int(pos[1])
    self.dx = (int(vel[0]) + width) % width
    self.dy = (int(vel[1]) + height) % height

robots = []

for line in lines:
  parts = line.strip().split(' ')
  pos = parts[0][2:].split(',')
  vel = parts[1][2:].split(',')
  robots.append(Robot(pos, vel))

midX = int((width - 1) / 2)
midY = int((height - 1) / 2)

def destQuadrant(r, count):
  x = (r.x + count * r.dx) % width
  y = (r.y + count * r.dy) % height
  if x < midX and y < midY:
    return 0
  if x > midX and y < midY:
    return 1
  if x < midX and y > midY:
    return 2
  if x > midX and y > midY:
    return 3
  return 4

quads = [0] * 5

for r in robots:
  q = destQuadrant(r, 100)
  quads[q] += 1

print("part 1: " + str(quads[0] * quads[1] * quads[2] * quads[3]))

# find a second when no robot in in the left or right top triangles?

def setAt(display, x, y, ch):
  line = display[y]
  line = line[:x] + ch + line[(x+1):]
  display[y] = line

def drawAll(sec, untree):
  line = '.' * width
  display = [line] * height
  for r in robots:
    x = (r.x + sec * r.dx) % width
    y = (r.y + sec * r.dy) % height
    setAt(display, x, y, '*')
  print("\nAt " + str(sec) + ' (' + str(int(untree * 100)) + '%)')
  for d in display:
    print(d)
  print('-' * width)

fudge = 10
def inCorner(x,y):
  if y + 2 * x < width - fudge:
    return True
  if y + 2 * (width - 1 - x) < width - fudge:
    return True
  return False

def countInCorner(s):
  cc = 0
  for r in robots:
    x = (r.x + sec * r.dx) % width
    y = (r.y + sec * r.dy) % height
    if inCorner(x,y):
      cc += 1
  return cc

sec = 0
while (True):
  untree = countInCorner(sec) / len(robots)
  if untree < 0.13:
    drawAll(sec, untree)
    i = input()
    if i != '':  # simple enter to see another one
      exit()
  sec += 1
  if sec % 1000000 == 0:
    print(sec)
