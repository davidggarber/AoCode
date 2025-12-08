data = open('day4.txt', mode='rt')
lines = data.readlines()

height = len(lines)
width = len(lines[0].strip())

def get(x, y):
  if y < 0 or y >= height:
    return '.'
  if x < 0 or x >= width:
    return '.'
  return lines[y][x % width]

def remove(x, y):
  lines[y] = lines[y][0:x] + '.' + lines[y][x+1:]

moveable = 0
for y in range(0, height):
  str = ''
  for x in range(0, width):
    c = get(x, y)
    if c == '@':
      neigh = 0
      for dy in range(-1, 2):
        for dx in range(-1, 2):
          nc = get(x + dx, y + dy)
          if nc == '@':
            neigh += 1
      if neigh <= 4:
        moveable += 1
        str += 'X'
      else:
        str += '@'
    else:
      str += '.'
  print(str)
  
print('part 1:', moveable)

moved = 0
moveable = -1
while moveable != 0:
  moveable = 0
  for y in range(0, height):
    str = ''
    for x in range(0, width):
      c = get(x, y)
      if c == '@':
        neigh = 0
        for dy in range(-1, 2):
          for dx in range(-1, 2):
            nc = get(x + dx, y + dy)
            if nc == '@':
              neigh += 1
        if neigh <= 4:
          moveable += 1
          remove(x, y)
          str += 'X'
        else:
          str += '@'
      else:
        str += '.'
    # print(str)
  moved += moveable

print('part 2:', moved)
