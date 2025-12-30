import time
import numpy as np

data = open('day10.txt', mode='rt')
lines = data.readlines()

target_lights = []
button_wirings = []
joltage_vectors = []
joltage_requirements = []

def parse_lights(s):
  s = s[1:]  # Remove leading '['
  bit = 1
  n = 0
  for ch in s:
    if ch == '#':
      n |= bit
    bit <<= 1
  return n

def parse_button(s):
  bits = s.split(',')
  n = 0
  for bit in bits:
    n |= 1 << int(bit)
  return n

def parse_buttons(s):
  s = s.strip().split(' ')
  buttons = [parse_button(b[1:-1]) for b in s]
  return buttons

def parse_vector(s, size):
  v = [0] * size
  rows = s.split(',')
  for r in rows:
    v[int(r)] += 1
  return v

def parse_jolt_vectors(s, size):
  s = s.strip().split(' ')
  vectors = [parse_vector(b[1:-1], size) for b in s]
  return vectors

def parse_joltage(s):
  s = s[:-1]  # remove trailing }
  jolts = [int(x) for x in s.split(',')]
  return jolts

def print_options(options):
  s = ''
  for (o,k) in options.items():
    s += f'{str(bin(o))[2:]}:{k} '
  return s

def solve_diagram(target, buttons):
  # print(f'Target: {str(bin(target))[2:]}')
  options = {}
  for b in buttons:
    options[b] = 1  # could include duplicates
  while True:
    if target in options:
      return options[target]
    # print(f'Options: {print_options(options)}')

    # What new options could we reach?
    cur_options = list(options.items())
    for (o,k) in cur_options:
      for b in buttons:
        new_o = o ^ b
        new_count = k + 1
        if new_o != 0 and (new_o not in options or new_count < options[new_o]):
          options[new_o] = new_count
    if len(options) == len(cur_options):
      raise Exception('No progress made')


for line in lines:
  parts = line.strip().split(']')
  size = len(parts[0]) - 1  # exclude leading '['
  target_lights.append(parse_lights(parts[0]))
  parts = parts[1].split('{')
  button_wirings.append(parse_buttons(parts[0]))
  joltage_vectors.append(parse_jolt_vectors(parts[0], size))
  joltage_requirements.append(parse_joltage(parts[1]))

# Part 1:
total_buttons = 0
for i in range(len(target_lights)):
  presses = solve_diagram(target_lights[i], button_wirings[i])
  # print(f'Diagram {i}: {presses} button presses')
  total_buttons += presses
print(f'Part 1: {total_buttons}')

sub_vectors = {}

def IsSubVector(sub, super):
  return np.all(super >= sub)

secondary_trick = True

def PullSecondaryVectors(vectors):
  global secondary_trick
  primary_vectors = { tuple(v):set() for v in vectors }
  secondaries = set()
  if secondary_trick:
    for i in range(len(vectors)):
      v = vectors[i]
      for j in range(i + 1, len(vectors)):
        u = vectors[j]
        if IsSubVector(v, u):
          secondaries.add(tuple(u))
          u2 = u - v
          primary_vectors[tuple(v)].add(tuple(u2))
        elif IsSubVector(u, v):
          secondaries.add(tuple(v))
          v2 = v - u
          primary_vectors[tuple(u)].add(tuple(v2))
    for s in secondaries:
      del primary_vectors[s]
  return (primary_vectors, secondaries)





def PlotAllCombos(vectors, steps, vec_maxes):
  # Calculate every possible combination of steps vectors
  prev_combos = [{'remaining': steps}]
  for iv in range(len(vectors) - 1, -1, -1):
    v = vectors[iv]
    combos = []
    for c in prev_combos:
      r = int(c['remaining'])
      max = vec_maxes[tuple(v)]
      if max < 0 or max > r:
        max = r
      if iv == 0:
        # Last vector, must use all remaining steps
        if max < r:
          continue  # Not enough bandwidth to take all necessary steps
        branch = c.copy()
        branch[tuple(v)] = max
        branch['remaining'] = 0
        combos.append(branch)
        continue
      for i in range(max + 1):
        branch = c.copy()
        branch['remaining'] = r - i
        branch[tuple(v)] = i
        combos.append(branch)
    prev_combos = combos
  return prev_combos

def JoltByAxis(target, vectors, parent_progress):
  if len(vectors) == 0:
    return None  # No vectors to use

  rank = len(target)
  axis_affecting_vectors = [[] for _ in range(len(target))]
  for vt in vectors.keys():
    v = np.array(vt)
    for i in range(len(target)):
      if v[i] != 0:
        axis_affecting_vectors[i].append(v)

  # Pick the smallest group from axis_affecting_vectors
  smallest_axis = -1
  min_size = len(vectors) * max(target) + 1 # larger than any possible group
  for i in range(len(axis_affecting_vectors)):
    if target[i] == 0 or len(axis_affecting_vectors[i]) == 0:
      continue
    if len(axis_affecting_vectors[i]) * target[i] < min_size:
      min_size = len(axis_affecting_vectors[i]) * target[i]
      smallest_axis = i
  if smallest_axis < 0:
    return None  # No axis can be affected

  best_recurse = None
  axis_vectors = axis_affecting_vectors[smallest_axis]
  other_vectors = vectors.copy()
  for v in axis_vectors:
    if tuple(v) in other_vectors:
      del other_vectors[tuple(v)]

  primary_vectors, second_vectors = PullSecondaryVectors(axis_vectors)
  vector_maxes = vectors.copy()
  for v,secs in primary_vectors.items():
    s_max = sum(vectors[tuple(np.array(v)+np.array(s))] for s in secs)
    if s_max > 0:
      if v in vector_maxes:
        vector_maxes[v] += s_max
      else:
        vector_maxes[v] = s_max
  axis_vectors = [np.array(v) for v in primary_vectors.keys()]

  axis_steps = target[smallest_axis]
  best_score = axis_steps
  plot_all_combos = PlotAllCombos(axis_vectors, axis_steps, vector_maxes)
  for ic in range(len(plot_all_combos)):
    pct = (ic * 100) / len(plot_all_combos)
    prog = f'{parent_progress}  {smallest_axis}:{pct:.2f}%'
    print(f'\r{prog}                                ', end='', flush=True)
    
    combo = plot_all_combos[ic]

    combined = np.zeros(rank)
    steps = [(np.array(key), val) for key,val in combo.items() if key != 'remaining']
    for s in steps:
      combined += s[0] * s[1]
  
    if np.any(combined > target):
      # Overshot the target. Discard
      continue
    if np.all(combined == target):
      return [(s[0],s[1],f'#{smallest_axis}',str((target-combined).astype(int).tolist())) for s in steps if s[1] > 0]
    # Recurse from this progress
    t2 = target - combined
    v2 = other_vectors.copy()
    for s in steps:
      if s[1] > 0:
        for sec in primary_vectors[tuple(s[0])]:
          if sec in v2:
            if v2[sec] >= 0:
              v2[sec] += s[1]
          else:
            v2[sec] = s[1]
          if sec in vectors:
            # Vector available 2 ways: directly and as combo
            v2[sec] += vectors[sec]
    recurse = JoltByAxis(t2, v2, prog)
    if recurse == None:
      continue
    print(f'\r{prog}                                ', end='', flush=True)  # pop the progress stack
    
    # Un-count secondaries
    for s in steps:
      s1 = s[1]
      for tsec in primary_vectors[tuple(s[0])]:
        if s1 > 0:
          sec = np.array(tsec)
          r_count = sum(r[1] for r in recurse if np.all(r[0] == sec))
          if r_count > 0:
            dupe = min(r_count, s1)
            if dupe == 0:
              continue
            sup = np.array(s[0]) + sec
            # Record a negative count, to balance out the duplicate
            recurse = [(sec, -dupe, '??', str(t2.astype(int).tolist()), sup)] + recurse
            s1 -= dupe

    score = axis_steps + sum(r[1] for r in recurse)
    if best_recurse == None or score < best_score:
      # if best_recurse != None:
      #   print(f'Improve {best_score} -> {score}')
      best_recurse = [(s[0],s[1],f'#{smallest_axis}',str(t2.astype(int).tolist())) for s in steps if s[1] > 0] + recurse
      best_score = score

  return best_recurse
  

slowest_rows = [18, 19, 31, 48, 63]
slowest_rows += [20, 43, 61]  # these are bad, but not horrible

part2 = 0
for i in range(len(joltage_requirements)):
  vectors = {tuple(np.array(v)):float('inf') for v in joltage_vectors[i]}
  target = np.array(joltage_requirements[i])
  print(f'\nLine #{i}: {str(target)}  ------------')

  if i in slowest_rows:
    print('  Skipping super-slow')
    continue


  secondary_trick = False
  start = time.perf_counter()
  steps = JoltByAxis(target, vectors, f'Line #{i}:')
  duration = time.perf_counter() - start
  # print('\n' + '\n'.join(str(s) for s in steps) + '\n')

  score = sum(s[1] for s in steps)
  merged = sum(s[1] for s in steps if s[1] < 0)
  print(f'\rLine #{i}: {score}  ({merged} merged) : {duration:.2f} sec                                                       ')


  # secondary_trick = True
  # start = time.perf_counter()
  # steps2 = JoltByAxis(target, vectors, f'Line #{i}:')
  # duration = time.perf_counter() - start
  # print('\n' + '\n'.join(str(s) for s in steps2) + '\n')

  # score = sum(s[1] for s in steps2)
  # merged = sum(s[1] for s in steps2 if s[1] < 0)
  # print(f'\rLine #{i}: {score}  ({merged} merged) : {duration:.2f} sec                                                       ')
  part2 += score

print(f'---------------\nPart 2: {part2}')
