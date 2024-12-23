import re

data = open('Day23.txt', mode='rt')
lines = data.readlines()

connections = {}

def addConnection(aa, bb):
  global connections
  if aa not in connections:
    connections[aa] = set()
  if bb not in connections:
    connections[bb] = set()
  connections[aa].add(bb)
  connections[bb].add(aa)


for line in lines:
  ends = line.strip().split('-')
  addConnection(ends[0], ends[1])

triples = set()
for comp in connections.keys():
  if comp[0] == 't':
    for aa in connections[comp]:
      for bb in connections[comp]:
        if aa != bb:
          if bb in connections[aa]:
            trip = [comp, aa, bb]
            trip.sort()
            triples.add(str(trip))

print(f"part 1: {len(triples)}")

def countBits(num):
  count = 0
  while num > 0:
    count += 1
    num &= num - 1
  return count

biggest = []
ties = []
for comp in connections.keys():
  if comp[0] == 't':  # T- prefix still assumed to be true
    cList = list(connections[comp])
    bits = {}
    nets = {}
    for i in range(len(cList)):
      bits[cList[i]] = 1 << i
      nets[cList[i]] = 1 << i  # each computer is in its own net
    for i in range(len(cList)):
      for j in range(i + 1, len(cList)):
        aa = cList[i]
        bb = cList[j]
        if aa != bb and bb in connections[aa]:
          nets[aa] |= bits[bb]
          nets[bb] |= bits[aa]
    most = 0
    for i in range(1 << len(cList)):
      if countBits(i) >= len(biggest):
        loop = [comp]
        for aa in cList:
          if (bits[aa] & i) and (nets[aa] & i) == i:
            loop.append(aa)
        if len(loop) > len(biggest):
          loop.sort()
          biggest = loop
          ties = []
        elif len(loop) == len(biggest):
          loop.sort()
          ties.append(','.join(loop))

def confirmNet(net):
  confirmed = True
  for i in range(len(net)):
    for j in range(i + 1, len(net)):
      aa = net[i]
      bb = net[j]
      if aa not in connections[bb] or bb not in connections[aa]:
        confirmed = False
  return confirmed
test = confirmNet(biggest)

print(f"Part2: {','.join(biggest)}")
for t in ties:
  print(f'Tied with {t}')

