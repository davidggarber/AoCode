var priQueue = [];
var seen = {};

function solve1() {
  var pos = [0, 0, 0, 0, 0, 0, width() + height() - 2];  // pos=0,0, prev_dir=0,0, 0 steps at that prev_dir, 0 total loss, distance to end
  enqueue(pos);
  while (true) {
    pos = dequeue();
    if (!pos) {
      break;
    }
    var s = pos.slice(0, 5);
    if (s in seen) {
      continue;
    }
    seen[s] = pos[5];  // pos6 is fixed for any x,y
    // console.log(pos.join(','));
    if (pos[6] == 0) {
      break;
    }
    var next = nextOptions(pos);
    for (var n of next) {
      enqueue(n);
    }
  }

  print(pos[5]);
}

function nextOptions(p) {
  var dirs = [
    [p[0] + 1, p[1], 1, 0, 0, 0, 0],  // right
    [p[0] - 1, p[1], -1, 0, 0, 0, 0],  // left
    [p[0], p[1] - 1, 0, -1, 0, 0, 0],  // up
    [p[0], p[1] + 1, 0, 1, 0, 0, 0]  // down
  ];
  dirs = dirs.filter(d => inBounds(d));
  dirs = dirs.map(d => extend(d, p));
  dirs = dirs.filter(d => !haveSeen(d));
  dirs = dirs.filter(d => d[4] >= 0 && d[4] < 3);
  return dirs;
}

function extend(d, p) {
  d[5] = p[5] + lossAt(d[0], d[1]);
  d[6] = width() - 1 - d[0] + height() - 1 - d[1];
  if (d[2] == p[2] && d[3] == p[3]) {
    d[4] = p[4] + 1;
  }
  else if (d[2] == -p[2] && d[3] == -p[3]) {
    d[4] = -1;
  }
  return d;
}

function enqueue(p) {
  var ins = 0;
  for (; ins < priQueue.length; ins++) {
    if (priQueue[ins][5] + priQueue[ins][6] < p[5] + p[6]) {   // lowest loss at end
      break; 
    }
  }
  priQueue.splice(ins, 0, p);
}

function dequeue() {
  return priQueue.pop();
}

function inBounds(p) {
  return p[0] >= 0 && p[1] >= 0
    && p[1] < height() && p[0] < width();
}

function haveSeen(p) {
  var d = p.slice(0, 5);
  return d in seen;
}

function lossAt(x, y) {
  return parseInt(lines[y][x]);
}


function solve2() {
  priQueue = [];
  seen = {};

  var pos = [0, 0, 0, 0, 11, 0, width() + height() - 2, 'S'];  // pos=0,0, prev_dir=0,0, 0 steps at that prev_dir, 0 total loss, distance to end
  enqueue(pos);
  while (true) {
    pos = dequeue();
    if (!pos) {
      break;
    }
    var s = pos.slice(0, 5);
    if (s in seen) {
      continue;
    }
    seen[s] = pos[5];  // pos6 is fixed for any x,y
    // console.log(pos.join(','));
    if (pos[6] == 0 && pos[4] >= 4) {
      break;
    }
    var next = nextOptions2(pos);
    for (var n of next) {
      enqueue(n);
    }
  }

  trace(pos[7]);
  print(pos[5]);
}

function nextOptions2(p) {
  var dirs = [
    [p[0] + 1, p[1], 1, 0, 1, 0, 0, p[7] + '>'],  // right
    [p[0] - 1, p[1], -1, 0, 1, 0, 0, p[7] + '<'],  // left
    [p[0], p[1] - 1, 0, -1, 1, 0, 0, p[7] + '^'],  // up
    [p[0], p[1] + 1, 0, 1, 1, 0, 0, p[7] + 'v']  // down
  ];
  dirs = dirs.filter(d => inBounds(d));
  dirs = dirs.map(d => extend2(d, p));
  dirs = dirs.filter(d => !haveSeen(d));
  dirs = dirs.filter(d => d[4] >= 0);
  return dirs;
}

function extend2(d, p) {
  d[5] = p[5] + lossAt(d[0], d[1]);
  d[6] = width() - 1 - d[0] + height() - 1 - d[1];
  if (d[2] == p[2] && d[3] == p[3]) {
    d[4] = p[4] + 1;
  }
  else if (d[2] == -p[2] && d[3] == -p[3]) {
    d[4] = -1;  // illegal - U turn
  }
  if (d[4] == 1 && p[4] < 4) {
    d[4] = -1;  // illegal - can't turn yet
  }
  else if (d[4] > 10) {
    d[4] = -1;  // illegal - must turn    
  }
  return d;
}
