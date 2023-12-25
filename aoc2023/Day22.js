function solve1() {
  var sum = 0;
  var bricks = [];
  var id = 0;
  for (var line of lines) {
    var b = line.match(/[0-9]+/g).map(n => parseInt(n));
    b[6] = id++;
    b[7] = -1;
    bricks.push(b);
  }
  bricks.sort((a,b) => a[5] - b[5]);

  for (var i = 0; i < bricks.length; i++) {
    var sup = fall(bricks, i);
    if (sup.length == 1) {
      bricks[sup[0]][7] = bricks[i][6];
      // solos[sup[0]] = true;
    }
    // trace(bricks[i].join(','));
  }

  bricks.sort((a,b) => a[5] - b[5]);

  for (var i = 0; i < bricks.length; i++) {
    var str = bricks[i].join(',');
    trace(str);
  }

  print(bricks.filter(b => b[7] < 0).length);
}

function fall(bricks, index) {
  var b = bricks[index];
  var max = 0;
  var supports = [];
  for (var i = index - 1; i >= 0; i--) {
    if (intersect(b, bricks[i])) {
      var h = bricks[i][2];
      if (h > max) {
        supports = [];
        max = h;
      }
      if (h == max) {
        supports.push(i);
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
  for (var line of lines) {
    // trace(power);
  }

  // print(sum);
}
