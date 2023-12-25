var exit;
var pqueue = [];

function enqueue(p) {
  pqueue.push(p);
  var i = pqueue.length - 1;
  while (i > 0) {
    var j = Math.floor(i / 2);
    var jp = pqueue[j];
    if (p[2] + dist(p, exit) < jp[2] + dist(jp, exit)) {
      break;
    }
    pqueue[j] = p;
    pqueue[i] = jp;
    i = j;
  }
}

function dequeue() {
  var pop = pqueue.pop();
  if (pqueue.length == 0) {
    return pop;
  }
  var top = pqueue[0];
  pqueue[0] = pop;
  var i = 0;
  while ((i + 1) * 2 <= pqueue.length) {
    var j = (i + 1) * 2 - 1;
    var k = (i + 1) * 2;
    var jp = pqueue[j];
    if (k < pqueue.length) {
      var kp = pqueue[k];
      if (kp[2] + dist(kp, exit) > jp[2] + dist(jp, exit)) {
        j = k;
        jp = k;
      }
    }
    if (pop[2] + dist(pop, exit) > jp[2] + dist(jp, exit)) {
      break;
    }
    pqueue[j] = pop;
    pqueue[i] = jp;
    i = j;
  }
  return top;
}

// Manhattan distance
function dist(a, b) {
  return Math.abs(a[0] - b[0]) + Math.abs(a[1] - b[1]);
}

function isPath(p, from) {
  var ch = charAt(p[0], p[1]);
  if (ch == '.') {
    return true;
  };
  if (ch == '#') {
    return false;
  }
  if (ch == '>') {
    return p[0] > from[0];
  }
  // if (ch == '<') {
  //   return p[0] < from[0];
  // }
  if (ch == 'v') {
    return p[1] > from[1];
  }
  // if (ch == '^') {
  //   return p[1] < from[1];
  // }
}

function solve1() {
  exit = [lines[height() - 1].indexOf('.'), height()-1];
  var start = [lines[0].indexOf('.'), 0, 0, {}];  // x, y, distance travelled, prior step set
  pqueue.push(start);
  var branches = {};
  var path = null;
  var altPaths = [];
  while (pqueue.length > 0) {
    var p = dequeue();
    // console.log(p.join(','));
    if (dist(p, exit) == 0) {
      console.log('Path found: ' + p[2]);
      if (path == null || p[2] > path[2]) {
        path = p;
      }
      altPaths.push(p);
      continue;
    }
    var pos = [p[0], p[1]];
    var neighbors = [[p[0]+1,p[1]], [p[0]-1,p[1]], [p[0],p[1]+1], [p[0],p[1]-1]];
    neighbors = neighbors.filter(n => isPath(n, pos) && !(n in p[3]));
    if (neighbors.length > 1) {
      branches[pos] = true;
    }
    for (var n of neighbors) {
      n.push(p[2] + 1);  // step #
      n.push(structuredClone(p[3]));  // clone p's history
      n[3][pos] = p[2];  // append pos to history
      enqueue(n);
    }
  }

  print(path[2]);

  // demo
  var ap = -1;
  var hist = ap >= 0 ? altPaths[ap] : path;
  var hist3 = hist[3]
  hist3[[hist[0],hist[1]]] = hist[2];
  console.log('Demo of #' + demo + ' (of ' + altPaths.length + ') : ' + hist[2]);
  var demo = structuredClone(lines);
  for (var h of Object.keys(hist3)) {
    var p = h.split(',').map(v => parseInt(v));
    var line = demo[p[1]];
    if (line[p[0]] == '.') {
      line = line.substring(0, p[0]) + 'O' + line.substring(p[0] + 1);
      demo[p[1]] = line;
    }
  }
  for (var b of Object.keys(branches)) {
    var p = b.split(',').map(v => parseInt(v));
    var line = demo[p[1]];
    line = line.substring(0, p[0]) + '%' + line.substring(p[0] + 1);
    demo[p[1]] = line;  
  }

  for (var line of demo) {
    trace(line);
  }

  colorMap();
  colorMap('debug1');

}


function solve2() {
  var sum = 0;
  for (var line of lines) {
    // trace(power);
  }

  // print(sum);
}
