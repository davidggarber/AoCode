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

def allSolveTowel(towel):
  global parts
  if len(towel) == 0:
    return 1
  starts = [p for p in parts if towel[:len(p)] == p]
  if len(starts) == 0:
    return 0
  print(f"{len(starts)} ways to start {towel}")
  variants = 0
  for s in starts:
    variants += allSolveTowel(towel[len(s):])
  return variants


count = 0
for g in goals:
  if anySolveTowel(g):
    count += 1
print(f"part 1: {count}")

sum = 0
for g in goals:
  if allSolveTowel(g):
    sum += 1
print(f"part 2: {sum}")
