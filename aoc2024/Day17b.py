import re

data = open('Day17.txt', mode='rt')
lines = data.readlines()

registers = []
output = []

def initReg():
  global registers
  global output
  registers = []
  registers.append(int(lines[0].strip().split(': ')[1]))
  registers.append(int(lines[1].split(': ')[1]))
  registers.append(int(lines[2].split(': ')[1]))
  output = []

initReg()
program = lines[4].strip().split(': ')[1].split(',')
program = [int(n) for n in program]
rOutput = 0

mirror = True

def write(n):
  global output
  global mirror
  output.append(n)
  mirror = str(output) == str(program[:len(output)])
  # print(output)

ip = 0

def combo(n):
  global registers
  if n <= 3:
    return n
  return registers[n - 4]

def adv(operand):
  global registers
  div = registers[0] / (2 ** combo(operand))
  registers[0] = int(div)
  return True

def rAdv(operand):
  global registers
  if operand <= 3:
    registers[0] = (registers[0] << operand) | (registers[0] & 0x7)
  else:
    registers[0] = (registers[0] << registers[operand - 4]) | (registers[0] & 0x7)
  return False

def rBdv(operand):
  global registers
  if operand <= 3:
    registers[0] = registers[1] << operand
  else:
    registers[0] = registers[1] << registers[operand - 4]
    return False

def rCdv(operand):
  global registers
  if operand <= 3:
    return registers[0] >> operand == registers[2]
  else:
    return registers[0] >> registers[operand - 4] == registers[2]

def bxl(operand):
  global registers
  registers[1] = registers[1] ^ operand
  return True

def rBxl(operand):
  bxl(operand)  # self-invert
  return False

def bst(operand):
  global registers
  registers[1] = combo(operand) & 0x7
  return True

def rBst(operand):
  global registers
  n = registers[1] & 0x7
  if operand <= 3:
    test = (operand & 0x7) == n
  else:
    registers[operand - 4] = (registers[operand - 4] & ~0x7) | n

def jnz(operand):
  global ip
  global registers
  if registers[0] == 0:
    return True
  ip = operand
  return False

def rJnz(operand):
  operand == operand  # no-op, other than verify A != 0, except first time
  return False

def bxc(operand):
  global registers
  registers[1] = registers[1] ^ registers[2]
  return True

def rBxc(operand):
  bxc(operand)  # self inverse
  return False

def out(operand):
  calc = combo(operand) % 8
  write(calc)
  return True

def rOut(operand):
  global registers
  global rOutput
  n = rOutput
  if operand == 4:
    registers[0] = ((registers[0] >> 3) << 3) | n
  elif operand == 5:
    registers[1] = ((registers[1] >> 3) << 3) | n
  elif operand == 6:
    registers[2] = ((registers[2] >> 3) << 3) | n
  return False

def bdv(operand):
  global registers
  div = registers[0] / (2 ** combo(operand))
  registers[1] = int(div)
  return True

def cdv(operand):
  global registers
  div = registers[0] / (2 ** combo(operand))
  registers[2] = int(div)
  return True

funcs = [adv, bxl, bst, jnz, bxc, out, bdv, cdv]
rFuncs = [rAdv, rBxl, rBst, rJnz, rBxc, rOut, rBdv, rCdv]

def testRunForward():
  global ip
  registers[1] = registers[2] = 0
  ip = 0
  while ip < len(program):
    op = program[ip]
    operand = program[ip + 1]
    if funcs[op](operand):
      ip += 2

possibleA = set()
firstBits = 10
moreBits = 3

for aa in range(0, 1<<firstBits):
  registers[0] = aa
  output = []
  testRunForward()
  # print(str(output))
  if str(output[:1]) == str(program[:1]):
    print(f"{aa} ({bin(aa)}) -> {output}")
    possibleA.add(aa)

print(f"Found {len(possibleA)} A's that yield {program[:1]}")

for steps in range(2, len(program) + 1):  
  possibleAA = set()

  for aa in possibleA:
    for a3 in range(0, 1<<moreBits):
      testA = aa | (a3 << (4 + 3 * steps))
      registers[0] = testA
      output = []
      testRunForward()
      # print(str(output))
      if str(output[:steps]) == str(program[:steps]):
        print(f"{testA} ({bin(testA)}) -> {output}")
        possibleAA.add(testA)

  print(f"Found {len(possibleAA)} A's that yield {program[:steps]}")
  possibleA = possibleAA

list = list(possibleA)
list.sort()
print(f"part 2: {list[0]}")


# testAt = 14
# registers[0] = 0
# for initC in range(0, 7):
#   err = False
#   registers[2] = initC
#   for i in range(len(program) - 1, 0, -1):
#     rOutput = program[i]
#     ip = len(program) - 2
#     while ip >= 0 and not err:
#       op = int(program[ip])
#       operand = int(program[ip + 1])
#       err = rFuncs[op](operand)
#       ip -= 2
#     if err:
#       break
#     if i == testAt:
#       testRunForward()
#   if i == 0 and registers[2] == 0:
#     break

# print("part 2: " + str(registers[0]))
