import re

data = open('day9.txt', mode='rt')
line = data.read().strip()

fat = []  # each item is a pair: ID,length, where ID==-1 when open space

def buildFat():
  nextId = 0
  i = 0
  while i < len(line):
    fat.append([nextId, int(line[i])])
    nextId += 1
    i += 1
    if i < len(line):
      fat.append([-1, int(line[i])])
      i += 1

def fill1():
  # fill spaces
  write = 0
  read = len(fat) - 1

  while write < read:
    if fat[write][0] >= 0:
      write += 1
    elif fat[read][0] < 0:
      read -= 1
    elif fat[write][1] <= fat[read][1]:
      # fill space from end
      fat[write][0] = fat[read][0]
      fat[read][1] -= fat[write][1]
    else:
      # fill part of space from end, leaving some space
      move = fat.pop(read)
      fat[write][1] -= move[1]
      fat.insert(write, move)

def checkSum():
  sum = 0
  pos = 0
  for seg in fat:
    if seg[0] > 0:
      sum += int((seg[0] * pos + seg[0] * (pos + seg[1] - 1)) * seg[1] / 2)
    pos += seg[1]
  return sum

buildFat()
fill1()
print("part 1: " + str(checkSum()))

def fill2():
  read = len(fat) - 1
  while read > 0:
    if fat[read][0] < 0:
      read -= 1
    else:
      # find a home to the right
      w = 0
      move = [fat[read][0],fat[read][1]]
      while w < read:
        if fat[w][0] < 0 and fat[w][1] >= fat[read][1]:
          fat[w][1] -= fat[read][1]
          fat[read][0] = -1
          if fat[w][1] == 0:  # exact fit
            fat[w] = move
          else:
            fat.insert(w, move)
          break
        else:
          w += 1
      if w >= read:
        # can't move
        read -= 1

fat = []  # reset
buildFat()
fill2()
print("part 2: " + str(checkSum()))
