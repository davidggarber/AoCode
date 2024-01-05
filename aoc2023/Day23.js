var exit;
var pqueue = [];

function tripletRanker(a, b) {
  return a[2] - b[2];
}

function exitDistRanker(a, b) {
  return a[2] + dist(a, exit) - (b[2] + dist(b, exit));
}

function enqueue(p, ranker) {
  pqueue.push(p);
  var i = pqueue.length - 1;
  while (i > 0) {
    var j = Math.floor(i / 2);
    var jp = pqueue[j];
    if (ranker(p, jp) < 0) {
      break;
    }
    pqueue[j] = p;
    pqueue[i] = jp;
    i = j;
  }
}

function dequeue(ranker) {
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
      if (ranker(kp, jp) > 0) {
        j = k;
        jp = kp;
      }
    }
    if (ranker(pop, jp) > 0) {
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

// Vector addition
function vadd(a, b) {
  return [a[0]+b[0], a[1]+b[1]];
}

var dirs = [[-1,0], [0,-1], [1,0], [0,1]];
var slides = '<^>v';

function canMoveSlippery(pos, dir) {
  var n = vadd(pos, dir);
  var ch = charAt(n[0], n[1]);
  if (ch == '.') { return true; }
  var i = slides.indexOf(ch);
  if (i < 0) {
    return false;  // '#'
  }
  return dir == dirs[i];
}

function canMove(pos, dir) {
  var n = vadd(pos, dir);
  var ch = charAt(n[0], n[1]);
  return ch != '#' && ch != '\0';
}

function buildBranchGraph(slippery) {
  exit = [lines[height() - 1].indexOf('.'), height()-1];
  var start = [lines[0].indexOf('.'), 0];  // x, y, distance travelled, prior step set

  var allBranches = {};
  allBranches[start] = start;
  allBranches[exit] = exit;
//  [start, exit];  // keys: node points, values: list of downstream next branch nodes
  for (var y = 1; y < height() - 1; y++) {
    for (var x = 1; x < width() - 1; x++) {
      if (charAt(x, y) != '#') {
        var pos = [x, y];
        var neighbors = dirs.map(d => vadd(pos, d)).filter(n => charAt(n[0],n[1]) != '#');
        if (neighbors.length > 2) {
          allBranches[pos] = pos;
        }
      }
    }
  }

  var cm = slippery ? canMoveSlippery : canMove;

  var graph = {};
  for (var branch of Object.values(allBranches)) {
    graph[branch] = {};  // downstream neighbors: distances
    var queue = [branch];
    var seen = {};
    seen[branch] = 0;
    while (queue.length > 0) {
      var pos = queue.pop();
      var dist = seen[pos];
      var neighbors = dirs.filter(d => cm(pos, d)).map(d => vadd(pos, d));
      for (var n of neighbors) {
        if (!(n in seen)) {
          if (n in allBranches) {
            graph[branch][n] = dist + 1;
          }
          else {
            seen[n] = dist + 1;
            queue.push(n);
          }
        }
      }
    }
  }

  return graph;
}

function solve1() {
  exit = [lines[height() - 1].indexOf('.'), height()-1];
  var start = [lines[0].indexOf('.'), 0];  // x, y, distance travelled, prior step set
  var graph = buildBranchGraph(true);

  var startPath = {};
  startPath[start] = true;
  var paths = [Object.keys(startPath)];
  var fullPaths = [];
  while (paths.length > 0) {
    var nextPaths = [];
    for (var p of paths) {
      var end = p[p.length - 1];
      var nexts = Object.keys(graph[end]);
      for (var n of nexts) {
        if (p.indexOf(n) < 0) {
          var q = structuredClone(p);
          q.push(n);
          if (n == exit) {
            fullPaths.push(q);
          }
          else {
            nextPaths.push(q);
          }
        }
      }
      paths = nextPaths;
    }
  }
  
  var max = 0;
  var best = null;
  for (var path of fullPaths) {
    var prev = null;
    var dist = 0;
    for (var node of path) {
      if (prev != null) {
        dist += graph[prev][node];
      }
      prev = node;
    }
    // trace(dist + ': ' + path.join(' -> '));
    if (dist > max) {
      max = dist;
      best = path;
    }
  }

  var draw = structuredClone(lines);
  var j = 0;
  for (var node of best) {
    var pos = node.split(',').map(p => parseInt(p));
    draw[pos[1]] = draw[pos[1]].substring(0, pos[0]) + (j++) + draw[pos[1]].substring(pos[0] + 1);
  }
  for (var d of draw) {
    trace(d);
  }
  colorMap('debug1');

  print(max);
}

function solve2() {
  exit = [lines[height() - 1].indexOf('.'), height()-1];
  var start = [lines[0].indexOf('.'), 0];  // x, y, distance travelled, prior step set
  var graph = buildBranchGraph(false);

  // Clean up one edge case: branch prior to exit should be treated as having only one down-stream
  var lastBranch = Object.keys(graph[exit])[0];
  graph[lastBranch] = {};
  graph[lastBranch][exit] = graph[exit][lastBranch];

  pqueue = [];
  enqueue([start, [], 0], tripletRanker);  // position, previous branches, distance

  var longest = 0;
  var longPath = null;

  var counter = 0;
  while (pqueue.length > 0) {
    var pos = dequeue(tripletRanker);
    if (++counter % 100000 == 0) {
      console.log('Processed ' + counter + '. Still tracking ' + pqueue.length + ' in queue');
    }
    var nexts = graph[pos[0]];
    for (var n of Object.keys(nexts)) {
      if (pos[1].indexOf(n) < 0) {
        var path = structuredClone(pos[1]);
        path.push(n);
        var dist = pos[2] + nexts[n];
        if (n == exit) {
          if (dist > longest) {
            console.log('Found a path of ' + dist + '. Still tracking ' + pqueue.length + ' in queue');
            longest = dist;
            longPath = path;
          }
        }
        else {
          enqueue([n, path, dist], tripletRanker);
        }
      }
    }
  }
  print(longest);
}

function solve2a() {
  exit = [lines[height() - 1].indexOf('.'), height()-1];
  var start = [lines[0].indexOf('.'), 0];  // x, y, distance travelled, prior step set
  var graph = buildBranchGraph(false);

  // Clean up one edge case: branch prior to exit should be treated as having only one down-stream
  var lastBranch = Object.keys(graph[exit])[0];
  graph[lastBranch] = {};
  graph[lastBranch][exit] = graph[exit][lastBranch];

  var startPath = {};
  startPath[start] = true;
  var paths = [Object.keys(startPath)];
  var fullPaths = [];
  while (paths.length > 0) {
    var nextPaths = [];
    for (var p of paths) {
      var end = p[p.length - 1];
      var nexts = Object.keys(graph[end]);
      for (var n of nexts) {
        if (p.indexOf(n) < 0) {
          var q = structuredClone(p);
          q.push(n);
          if (n == exit) {
            fullPaths.push(q);
          }
          else {
            nextPaths.push(q);
          }
        }
      }
      paths = nextPaths;
    }
  }
  
  var max = 0;
  for (var path of fullPaths) {
    var prev = null;
    var dist = 0;
    for (var node of path) {
      if (prev != null) {
        dist += graph[prev][node];
      }
      prev = node;
    }
    trace(dist + ': ' + path.join(' -> '));
    max = Math.max(max, dist);
  }

  print(max);
}

function solve1a() {
  exit = [lines[height() - 1].indexOf('.'), height()-1];
  var start = [lines[0].indexOf('.'), 0, 0, {}];  // x, y, distance travelled, prior step set

  pqueue.push(start);
  var branches = {};
  var path = null;
  var altPaths = [];
  while (pqueue.length > 0) {
    var p = dequeue(exitDistRanker);
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
      enqueue(n, exitDistRanker);
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
  colorMap('debug2');

}
