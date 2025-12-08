from math import sqrt

data = open('day8.txt', mode='rt')
lines = data.readlines()
lines = [line.strip().split(',') for line in lines]
boxes = []
for line in lines:
  boxes.append([int(x) for x in line])

def distance(a, b):
    dx = a[0] - b[0]
    dy = a[1] - b[1]
    dz = a[2] - b[2]
    return sqrt(dx * dx + dy * dy + dz * dz)
  
distances = {}
for i in range(1, len(boxes)):
  dist = {}
  for j in range(i):
    dist[j] = distance(boxes[i], boxes[j])  
  distances[i] = sorted(dist.items(), key=lambda x: x[1])

circuits = []
circuit_ids = {}

def joinNearest():
  global distances, boxes, circuits, circuit_ids
  closest = None
  for i in range(1, len(boxes)):
    dlist = distances[i]
    if len(dlist) > 0 and closest is None or dlist[0][1] < closest[1]:
      closest = (i, dlist[0][1], dlist[0][0])
  # connect closest[0] to closest[2]
  if closest[0] in circuit_ids and closest[2] in circuit_ids:
    circ = circuit_ids[closest[0]]
    move = circuit_ids[closest[2]]
    if circ == move:
      # they are already merged from a mutual link
      pass
    else:
      # merge id2 into id
      for m in move:
        circ.append(m)
        circuit_ids[m] = circ
      circuits.remove(move)
  elif closest[0] in circuit_ids:
    circuit_ids[closest[0]].append(closest[2])
    circuit_ids[closest[2]] = circuit_ids[closest[0]]
  elif closest[2] in circuit_ids:
    circuit_ids[closest[2]].append(closest[0])
    circuit_ids[closest[0]] = circuit_ids[closest[2]]
  else:
    new_circuit = [closest[0], closest[2]]
    circuits.append(new_circuit)
    circuit_ids[closest[0]] = new_circuit
    circuit_ids[closest[2]] = new_circuit
  
  # remove distance
  dlist = distances[closest[0]]
  del dlist[0]
  return closest

for i in range(10):
  latest = joinNearest()
  # print(f'Connect #{latest[0]} {boxes[latest[0]]} to #{latest[2]} {boxes[latest[2]]} at distance {latest[1]}')

# Sort biggest circuits
biggest = sorted(circuits, key=lambda x: len(x), reverse=True)
prod = len(biggest[0]) * len(biggest[1]) * len(biggest[2])
print(f'Part 1: {prod}')

latest = None
while (len(circuits) > 1 or len(circuit_ids) < len(boxes)):
  latest = joinNearest()
  # print(f'Connect #{latest[0]} {boxes[latest[0]]} to #{latest[2]} {boxes[latest[2]]} at distance {latest[1]}   -> {len(circuits)} separate circuits')
  print (f'{len(circuits)} separate circuits')
extension = boxes[latest[0]][0] * boxes[latest[2]][0]
print(f'Part 2: {extension}')