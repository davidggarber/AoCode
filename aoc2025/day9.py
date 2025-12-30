data = open('day9.txt', mode='rt')
lines = data.readlines()

reds = [line.strip().split(',') for line in lines]
reds = [(int(x), int(y)) for x, y in reds]

tl = sorted(reds, key=lambda x: x[0]*x[1])
tr = sorted(reds, key=lambda x: (-1-x[0])*x[1])

i = 0
biggest = 0
for i in range(len(tl) // 4):
  for j in range(len(tl)-1, i, -1):
    area = (1+abs(tl[i][0] - tl[j][0])) * (1+abs(tl[j][1] - tl[i][1]))
    if area > biggest:
      biggest = area
      print(f'{tl[i]} - {tl[j]} => {area}')

for i in range(len(tr) // 4):
  for j in range(len(tr)-1, i, -1):
    area = (1+abs(tr[i][0] - tr[j][0])) * (1+abs(tr[j][1] - tr[i][1]))
    if area > biggest:
      biggest = area
      print(f'{tl[i]} - {tl[j]} => {area}')

print(f'Part 1: {biggest}')

def isInside(p):
  global reds
  count_below = 0
  for i in range(len(reds)):
    p1 = reds[i]
    p2 = reds[(i+1) % len(reds)]
    if p1[1] == p2[1]:
      # Hoizontal edge
      if p[0] >= min(p1[0],p2[0]) and p[0] < max(p1[0],p2[0]):
        if p[1] == p1[1]:
          return True
        if p[1] < p1[1]:
          count_below += 1
    else:
      # Vertical edge
      if p[0] == p1[0] and p[1] >= min(p1[1],p2[1]) and p[1] <= max(p1[1],p2[1]):
        return True
  return count_below % 2 == 1

def crossesHorz(x,y1,y2):
  global reds
  k = len(reds)
  for i in range(len(reds)):
    p1 = reds[i]
    p2 = reds[(i+1) % k]
    if p1[1] == p2[1]:
      # Horizontal edge
      y = p1[1]
      if x > min(p1[0],p2[0]) and x < max(p1[0],p2[0]):
        # Crosses our x
        if y > min(y1,y2) and y < max(y1,y2):
          # Within (y1,y2)
          return True
      if x == p1[0]:
        # p1 is immediately in line with our x
        p0 = reds[(i + k - 2) % k]
        if (p2[0]-p1[0]) * (p1[0]-p0[0]) > 0:
          y0 = p0[1]
          if min(y,y0) > min(y1,y2) and max(y,y0) < max(y1,y2):
            # y and y0 are both within (y1,y2), so it might as well be a crossing line
            return True
      if x == p2[0]:
        # p2 is immediately in line with our x
        p3 = reds[(i + 3) % k]
        if (p3[0]-p2[0]) * (p2[0]-p1[0]) > 0:
          y3 = p3[1]
          if min(y,y3) > min(y1,y2) and max(y,y3) < max(y1,y2):
            # y and y3 are both within (y1,y2), so it might as well be a crossing line
            return True
  return False

def crossesVert(y, x1,x2):
  global reds
  k = len(reds)
  for i in range(len(reds)):
    p1 = reds[i]
    p2 = reds[(i+1) % k]
    if p1[0] == p2[0]:
      # Vertical edge
      x = p1[0]
      if y > min(p1[1],p2[1]) and y < max(p1[1],p2[1]):
        # Crosses our y
        if x > min(x1,x2) and x < max(x1,x2):
          # Within (x1,x2)
          return True
      if y == p1[1]:
        # p1 is immediately in line with our y
        p0 = reds[(i + k - 2) % k]
        if (p2[1]-p1[1]) * (p1[1]-p0[1]) > 0:
          x0 = p0[0]
          if min(x,x0) > min(x1,x2) and max(x,x0) < max(x1,x2):
            # x and x0 are both within (x1,x2), so it might as well be a crossing line
            return True
      if y == p2[1]:
        # p2 is immediately in line with our x
        p3 = reds[(i + 3) % k]
        if (p3[1]-p2[1]) * (p2[1]-p1[1]) > 0:
          x3 = p3[0]
          if min(x,x3) > min(x1,x2) and max(x,x3) < max(x1,x2):
            # x and x3 are both within (x1,x2), so it might as well be a crossing line
            return True
  return False

def isInterior(p1, p2):
  # Check opposite corners are inside
  if not isInside((p1[0], p2[1])) or not isInside((p2[0], p1[1])):
    return False
  if crossesHorz(p1[0], p1[1], p2[1]):
    return False
  if crossesHorz(p2[0], p1[1], p2[1]):
    return False
  if crossesVert(p1[1], p1[0], p2[0]):
    return False
  if crossesVert(p2[1], p1[0], p2[0]):
    return False
  return True

i = 0
biggest = 0
for i in range(len(reds)):
  print(i)
  for j in range(len(reds)-1, i, -1):
    area = (1+abs(reds[i][0] - reds[j][0])) * (1+abs(reds[j][1] - reds[i][1]))
    if area > biggest and isInterior(reds[i], reds[j]):
      biggest = area
      print(f'{reds[i]} - {reds[j]} => {area}')

print(f'Part 2: {biggest}')
