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
zzz.reverse()

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
  for p in gList:
    num <<= 1
    r = resolve(p)
    if r:
      num |= 1
  return num


part1 = ReadAsBits(zzz)

allInputs = list(inputs.keys())
allInputs.sort()

print(f"Part1: {part1}")

xxx = [g for g in inputs.keys() if g[0] == 'x']
yyy = [g for g in inputs.keys() if g[0] == 'y']
xxx.sort()
xxx.reverse()
yyy.sort()
yyy.reverse()


xStart = ReadAsBits(xxx)
yStart = ReadAsBits(yyy)
print(f"xInit: {bin(xStart)}")
print(f"yInit: {bin(yStart)}")
zSum = xStart + yStart
print(f"zSum : {bin(zSum)}")
print(f"part1: {bin(part1)}")

zWrong = zSum ^ part1
print(f"wrong: {bin(zWrong)}")

switchers = set()

def findSwitchers(g):
  global gates, inputs, switchers
  if g not in gates:
    return
  switchers.add(g)
  gat = gates[g]
  aa = inputs[gat[0]]
  bb = inputs[gat[2]]
  gg = inputs[g]
  if g[1] == 'AND':
    if gg:
      findSwitchers(gat[0])
      findSwitchers(gat[2])
    else:
      if not aa:
        findSwitchers(gat[0])
      if not bb:
        findSwitchers(gat[2])
  elif g[1] == 'OR':
    if gg:
      if aa:
        findSwitchers(gat[0])
      if bb:
        findSwitchers(gat[2])
    else:
      if not aa:
        findSwitchers(gat[0])
      if not bb:
        findSwitchers(gat[2])
  else:
    findSwitchers(gat[0])
    findSwitchers(gat[2])

zBit = 1
for z in zzz:
  if zWrong & zBit:
    findSwitchers(z)
  zBit <<= 1

t2f = []
f2t = []
for s in list(switchers):
  if inputs[s]:
    t2f.append(s)
  else:
    f2t.append(s)

def rerun():
  global inputs, initialInputs
  inputs = initialInputs.copy()
  bits = ReadAsBits()
  good = zWrong & bits
  oops = (~zWrong) & bits
  return [good, oops]

flips.add(t2f[0])
flips.add(f2t[0])

test = rerun()
print(test)
