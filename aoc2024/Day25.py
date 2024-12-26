import re

data = open('Day25.txt', mode='rt')
lines = data.readlines()

keys = []
locks = []

def pinHeight(pic, col, ch):
  h = -1
  for row in pic:
    if row[col] == ch:
      return h
    h += 1
  return -1  # will error later, when shifted

def encodeLock(pic):
  bits = 0
  for c in range(5):
    col = pinHeight(pic, c, '.')
    b = ((1 << col) - 1)
    bits |= b << (5 * c)
  return bits

def encodeKey(pic):
  bits = 0
  for c in range(5):
    col = pinHeight(pic, c, '#')
    b = 0x1f ^ ((1 << col) - 1)
    bits |= b << (5 * c)
  return bits


for i in range(0, len(lines), 8):
  pic = lines[i:(i+7)]
  isLock = '#' in pic[0]
  if isLock:
    locks.append(encodeLock(pic))
  else:
    keys.append(encodeKey(pic))

part1 = 0

for key in keys:
  for lock in locks:
    if key & lock == 0:
      part1 += 1

print(f"Part1: {part1}")

