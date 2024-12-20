import re
import heapq

data = open('Day19.txt', mode='rt')
lines = data.readlines()

parts = lines[0].strip().split(', ')
goals = [l.strip() for l in lines[2:]]

def anySolveTowel(towel):
  global parts
  if len(towel) == 0:
    return True
  starts = [p for p in parts if towel[:len(p)] == p]
  if len(starts) == 0:
    return False
  for s in starts:
    if anySolveTowel(towel[len(s):]):
      return True
  return False

known = {}
def allSolveTowel(towel):
  global parts
  if towel in known:
    return known[towel]
  variants = 0
  if len(towel) == 0:
    variants = 1
  starts = [p for p in parts if towel[:len(p)] == p]
  if len(starts) > 0:
    # print(f"{len(starts)} ways to start {towel}")
    for s in starts:
      variants += allSolveTowel(towel[len(s):])
  known[towel] = variants
  return variants


count = 0
for g in goals:
  if anySolveTowel(g):
    count += 1
print(f"part 1: {count}")

sum = 0
for g in goals:
  print(g)
  sum += allSolveTowel(g)
print(f"part 2: {sum}")
