var bricks = [];
var brickIds = {};

function solve1() {
  var sum = 0;
  var id = 0;
  for (var line of lines) {
    var b = line.match(/[0-9]+/g).map(n => parseInt(n));
    brickIds[id] = b;
    b[6] = id++; // this brick's id
    b[7] = [];   // the ids of any bricks being supported only by this brick
    b[8] = [];   // the ids of all bricks supporting this brick
    b[9] = [];   // the ids of all bricks being supported by this brick (possibly with help)
    bricks.push(b);
  }
  bricks.sort((a,b) => a[5] - b[5]);
  // for (var b of bricks) {
  //   trace(b[2]+ '-' + b[5]);
  // }

  for (var i = 0; i < bricks.length; i++) {
    var sup = fall(bricks, i);
    bricks[i][8] = sup.map(s => bricks[s][6]);
    if (sup.length == 1) {
      bricks[sup[0]][7].push(bricks[i][6]);
      // solos[sup[0]] = true;
    }
    for (var j of sup) {
      bricks[j][9].push(bricks[i][6]);
    }
    // trace(bricks[i].join(','));
  }

  bricks.sort((a,b) => a[5] - b[5]);

  for (var i = 0; i < bricks.length; i++) {
    var str = '#' + bricks[i][6] 
      + ': ' + bricks[i].slice(0,3).join(',') + '~' + bricks[i].slice(3,6).join(',')
      + '  parents: ' + bricks[i][8].join(',');
    trace(str);
  }

  print(bricks.filter(b => b[7].length == 0).length);
}

function fall(bricks, index) {
  var b = bricks[index];
  var max = 0;
  var supports = [];
  for (var i = index - 1; i >= 0; i--) {
    if (intersect(b, bricks[i])) {
      var h = bricks[i][5];
      if (h > max) {
        supports = [];
        max = h;
      }
      if (h == max) {
        supports.push(i);  // index, not id
      }
    }
  }
  var drop = b[2] - (max + 1);
  if (drop > 0) {
    b[5] -= drop;
    b[2] -= drop;
  }
  return supports;
}

// Do two bricks intersect in x,y coordinates?
function intersect(a, b) {
  return a[0] <= b[3] && b[0] <= a[3]
    && a[1] <= b[4] && b[1] <= a[4];
}

function solve2() {
  var sum = 0;
  for (var b of bricks) {
    var wouldFall = {};
    var removed = {};
    removed[b[6]] = true;
    var children = b[9].slice(0);
    while (children.length > 0) {
      var grand = [];
      for (var c of children) {
        var child = brickIds[c];
        if (unsupported(child, removed)) {
          wouldFall[c] = true;
          removed[c] = true;
          for (var g of child[9]) {
            grand.push(g);
          }
        }
      }
      children = grand;
    }
    sum += Object.keys(wouldFall).length;
  }
  print(sum);
}

function unsupported(b, removed) {
  for (var p of b[8]) {
    if (!(p in removed)) {
      return false;
    }
  }
  return b[2] > 1;
}