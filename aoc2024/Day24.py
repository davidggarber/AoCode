import re

data = open('Day24.txt', mode='rt')
lines = data.readlines()

inputs = {}
flips = set()
gates = None
for line in lines:
  if len(line.strip()) == 0:
    gates = {}
    continue
  if gates is None:
    inp = line.strip().split(': ')
    inputs[inp[0]] = (int(inp[1]) == 1)
  else:
    gat = line.strip().split(' ')
    gates[gat[4]] = gat[0:3]

initialInputs = inputs.copy()

zzz = [g for g in gates.keys() if g[0] == 'z']
zzz.sort()
visited = set()

def resolve(z):
  global inputs, gates, flips
  if z in inputs:
    return inputs[z]
  gat = gates[z]
  aa = resolve(gat[0])
  bb = resolve(gat[2])
  g = None
  if gat[1] == 'AND':
    g = aa and bb
  elif gat[1] == 'OR':
    g = aa or bb
  else: # gat[1] == 'XOR':
    g = aa != bb
  if z in flips:
    g = not g
  inputs[z] = g
  return g

def ReadAsBits(gList):
  num = 0
  bit = 1
  for g in gList:
    r = resolve(g)
    if r:
      num |= bit
    bit <<= 1
  return num


part1 = ReadAsBits(zzz)

allInputs = list(inputs.keys())
allInputs.sort()

print(f"Part1: {part1}")

xxx = [g for g in inputs.keys() if g[0] == 'x']
yyy = [g for g in inputs.keys() if g[0] == 'y']
xxx.sort()
yyy.sort()

xStart = ReadAsBits(xxx)
yStart = ReadAsBits(yyy)
print(f"xInit: {bin(xStart)}")
print(f"yInit: {bin(yStart)}")
zSum = xStart + yStart
print(f"zSum : {bin(zSum)}")
print(f"part1: {bin(part1)}")

zWrong = zSum ^ part1
print(f"wrong: {bin(zWrong)}")
# the errors are at the boundaries between correct and incorrect bit sequences

# name the bits we want to flip
wrongGates = []
rightGates = []
bit = 1
for z in zzz:
  if bit & zWrong:
    wrongGates.append(z)
  else:
    rightGates.append(z)
  if ((bit & zWrong) == 0) != (((bit >> 1) & zWrong) == 0):
    print(f"Expect error at {z}")
  bit <<= 1

ands = [None] * len(xxx)
xors = [None] * len(xxx)
lefts = [None] * len(xxx)
rights = [None] * len(xxx)
middles = [None] * len(xxx)
overflows = [None] * len(xxx)

def findBasic(g):
  global gates, ands, xors
  gat = gates[g]
  if (gat[0][0] == 'x' and gat[2][0] == 'y') or (gat[0][0] == 'y' and gat[2][0] == 'x'):
    code = gat[0][1:]
    num = int(code)
    if gat[1] == 'AND':
      ands[num] = g
    elif gat[1] == 'XOR':
      xors[num] = g
    #else:
      # shouldn't happen

def findGate(aaa, bbb, op):
  global gates
  for g in gates.keys():
    gat = gates[g]
    if gat[1] == op:
      if (gat[0] == aaa and gat[2] == bbb) or (gat[0] == bbb and gat[2] == aaa):
        return g
  return None

def findHalfGate(aaa, op):
  global gates
  for g in gates.keys():
    gat = gates[g]
    if gat[1] == op and (gat[0] == aaa or gat[2] == aaa):
      return g
  return None


def findLeft(num):  
  lefts[num - 1] = findGate()


findBasic('z00')
for g in gates.keys():
  findBasic(g)

def nonOverlap(a, b, c, d):
  different = []
  if a != c and a != d:
    different.append(a)
  if b != c and b != d:
    different.append(b)
  if a != c and b != c:
    different.append(c)
  if a != d and b != d:
    different.append(d)
  return different

errors = []
fixes = []
rights[0] = xors[0]
overflows[0] = ands[0]
# lefts[1] = findGate(overflows[0], ands[0], 'OR')  # is redundant
lefts[1] = overflows[0]
rights[1] = findGate(lefts[1], xors[1], 'XOR')
overflows[1] = findGate(lefts[1], xors[1], 'AND')
lefts[2] = findGate(overflows[1], ands[1], 'OR')
rights[2] = findGate(lefts[2], xors[2], 'XOR')
overflows[2] = findGate(lefts[2], xors[2], 'AND')
for i in range(3, len(zzz)+1):
  lefts[i] = findGate(overflows[i-1], ands[i-1], 'OR')
  if lefts[i] == None:
    print(f"Can't find left-{str(i)}. Expected {overflows[i-1]} OR {ands[i-1]}.")
    if len(errors) > 0:
      z = findHalfGate(ands[i-1], 'OR')
      if z != None:
        fix = errors + [z]
        errors = []
        print(f"  Swapping {str(fix)}.")
        fixes += fix
        gT = gates[fix[0]]
        gates[fix[0]] = gates[fix[1]]
        gates[fix[1]] = gT
        overflows[i-1] = z
        lefts[i] = findGate(overflows[i-1], ands[i-1], 'OR')
      elif ands[i-1][0] == 'z':
        print(f"Found {ands[i-1]} where a non-Z gate should be")
        fix = errors + [ands[i-1]]
        errors = []
        print(f"  Swapping {str(fix)}.")
        fixes += fix
        gT = gates[fix[0]]
        gates[fix[0]] = gates[fix[1]]
        gates[fix[1]] = gT
        ands[i-1] = fix[0]
        lefts[i] = findGate(overflows[i-1], ands[i-1], 'OR')
      else:
        z = findHalfGate(overflows[i-1], 'OR')
        if z != None:
          fix = errors + [z]
          errors = []
          print(f"  Swapping {str(fix)}.")
          fixes += fix
          gT = gates[fix[0]]
          gates[fix[0]] = gates[fix[1]]
          gates[fix[1]] = gT
          ands[i-1] = z
          lefts[i] = findGate(overflows[i-1], ands[i-1], 'OR')

  rights[i] = findGate(lefts[i], xors[i], 'XOR')
  if rights[i] == None:
    zN = gates[zzz[i]]
    print(f"Can't find z{str(i)}. Expected {lefts[i]} XOR {xors[i]}.")
    print(f"  Actual z{str(i)} == {zN[0]} {zN[1]} {zN[2]}")
    print(f"  Check {lefts[i]} ==  {overflows[i-1]} OR {ands[i-1]}.")
    # Suspect overflows and lefts, before ands or xors
    fix = nonOverlap(lefts[i], xors[i], zN[0], zN[2])
    if (fix[0] in errors) or (fix[1] in errors):
      errors = []
    print(f"  Swapping {str(fix)}.")
    fixes += fix
    if lefts[i] == fix[0]:
      lefts[i] = fix[1]
    elif xors[i] == fix[0]:
      xors[i] = fix[1]
    gT = gates[fix[0]]
    gates[fix[0]] = gates[fix[1]]
    gates[fix[1]] = gT
  elif rights[i][0] != 'z':
    print(f"Found {rights[i]} where z{i} should be. {lefts[i]} XOR {xors[i]}.")
    errors.append(rights[i])
    rights[i] = zzz[i]

  overflows[i] = findGate(lefts[i], xors[i], 'AND')
  if overflows[i] == None:
    print(f"Can't find overflow for z{str(i)}. Should be {lefts[i]} AND {xors[i]}.")
    print(f"  What gate is and-ed with {xors[i]}? It will be an input to z{str(i+1)}.")
  elif overflows[i][0] == 'z':
    # don't overflow directly into a Z
    print(f"Found {overflows[i]} where a non-Z gate should be")
    g = overflows[i]
    zN = gates[g]
    print(f"  {g} == {zN[0]} {zN[1]} {zN[2]}")
    errors.append(g)
    if len(errors) == 2:
      print(f"  Swapping {str(errors)}.")
      fixes += errors
    rights[i] = errors[1]
    overflows[i] = errors[0]
    gT = gates[errors[0]]
    gates[errors[0]] = gates[errors[1]]
    gates[errors[1]] = gT
    errors = []
  
  if len(fixes) >= 8:
    break

fixes.sort()
print(f"Part 2: {','.join(fixes)}")