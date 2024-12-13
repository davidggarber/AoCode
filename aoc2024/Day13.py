import re

data = open('Day13.txt', mode='rt')
lines = data.readlines()

machines = []

class Point:
  def __init__(self, x, y): 
    self.x = x 
    self.y = y

for i in range(0, len(lines), 4):
  btnA = re.search(r'.*\+(\d+).*\+(\d+)', lines[i])
  btnB = re.search(r'.*\+(\d+).*\+(\d+)', lines[i + 1])
  prize = re.search(r'.*\=(\d+).*\=(\d+)', lines[i + 2])
  aa = [int(a) for a in btnA.groups()]
  bb = [int(b) for b in btnB.groups()]
  pp = [int(p) for p in prize.groups()]
  machines.append([Point(aa[0],aa[1]), Point(bb[0],bb[1]), Point(pp[0],pp[1])])

def solveLinear(a, b, p):
  det = a.x * b.y - a.y * b.x
  if det == 0: 
    return 0 # Parallel - might need to check for a straight-shot solution

  # Cramer's rule
  C = (p.x * b.y - p.y * b.x) / det 
  D = (a.x * p.y - a.y * p.x) / det
  if C == int(C) and D == int(D):
    return C * 3 + D
  return 0

cost = 0
for m in machines:
  (a,b,p) = m
  cost += solveLinear(a, b, p)

print("part 1: " + str(int(cost)))

for m in machines:
  m[2].x += 10000000000000
  m[2].y += 10000000000000

cost = 0

for m in machines:
  (a,b,p) = m
  cost += solveLinear(a, b, p)

print("part 2: " + str(int(cost)))
