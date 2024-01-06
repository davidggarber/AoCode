var start = [];

function findStart() {
  for (var y = 0; y < height(); y++) {
    var x = lines[y].indexOf('S');
    if (x >= 0) {
      return start = [x,y];
    }
  }
}

function solve1a() {
  findStart();
  return;

  var firstReached = {};
  var nextStep = [];
  var step = 0;
  nextStep.push(start);
  firstReached[start] = 0;
  while (nextStep.length > 0 && step < 64) {
    var thisStep = nextStep;
    nextStep = [];
    step++;
    while (thisStep.length > 0) {
      var p = thisStep.pop();
      var neighbors = [[p[0]-1,p[1]], [p[0]+1,p[1]], [p[0],p[1]-1], [p[0],p[1]+1]];
      for (var n of neighbors) {
        if (charAt(n[0], n[1]) == '.' && !(n in firstReached)) {
          firstReached[n] = step;
          nextStep.push(n);
        }
      }  
    }
  }


  for (var y = 0; y < height(); y++) {
    var str = '';
    for (var x = 0; x < width(); x++) {
      var p = [x,y];
      if (p in firstReached) {
        str += (firstReached[p] % 2 == 0) ? 'O' : '.';
      }
      else {
        str += charAt(x, y);
      }
    }
    trace(str);
  }

  var sum = Object.values(firstReached).filter(v => (v % 2) == 0).length;

  print(sum);

  colorMap();
  colorMap('debug1');
}

var gridReached = {};

function wrapCharAt(x, y, step) {
  var g = [Math.floor(x / width()), Math.floor(y / width())];
  x -= g[0] * width();
  y -= g[1] * height();
  if (charAt(x, y) == '#') {
    return '#';
  }
  if (!(g in gridReached)) {
    gridReached[g] = [x, y, step];
  }
  return '.';
}

function solve2() {
  findStart();
  var radius = 26501365;
  // var radius = 5000;
  var modulo = width() + height();
  var rMod = radius % modulo;
  trace('Modulo is ' + modulo);
  trace('Step target ' + radius + ' % ' + modulo + ' == ' + rMod);
  var cycles = (radius - rMod) / modulo;
  trace('Outermost tiles are ' + cycles + ' away from the starting title');

  // after modulo/2, start tile is half filled with a diamond
  // after modulo, start tile is fully filled in, with even polarity
  //     and neighbors each have 1/4 filled in from one side
  // after modulo*3/2, side neighbors are 3/4 filled in, touching opposite sides
  //     and corner neighbors are 1/8 filled in at corners
  // after modulo*2, side neighbors are filled in with odd polarity
  //     and corner neighbors are 7/8 filled in, missing far corners

  // All modulo starting points
  var firstSide = (width() + 1) / 66;
  var secondSide = firstSide + width();
  var middle = Math.floor(width() / 2);
  var firstCorner = width() + 1;
  var secondCorner = width() + 1 + height();
  
  var evenFull = fillTile(start, 0, 2 * modulo + rMod);
  trace('Even full = ' + evenFull);
  // All evenFull tiles are the same, whether starting in the middle or a corner

  var oddFull = fillTile([0, middle], 0, 2 * modulo + rMod);
  trace('Odd full = ' + oddFull);
  // trace('S = ' + fillTile([middle, 0], firstSide, 2 * modulo + rMod));
  // trace('E = ' + fillTile([width() - 1, middle], firstSide, 2 * modulo + rMod));
  // trace('SEE = ' + fillTile([0, 0], secondCorner, 2 * modulo + rMod));
  // All oddFull tiles are the same, regardless of side or corner start

  // rMod will leave 4 points
  var sPoint = fillTile([middle, 0], secondSide, modulo + rMod);
  var nPoint = fillTile([middle, height() - 1], secondSide, modulo + rMod);
  var ePoint = fillTile([0, middle], secondSide, modulo + rMod);
  var wPoint = fillTile([width() - 1, middle], secondSide, modulo + rMod);
  trace('S point = ' + sPoint);
  trace('N point = ' + nPoint);
  trace('E point = ' + ePoint);
  trace('W point = ' + wPoint);

  // Then 4 diagonals, all of odd length:
  var seCorner = fillTile([0, 0], firstCorner, modulo + rMod);
  var neCorner = fillTile([0, height() - 1], firstCorner, modulo + rMod);
  var swCorner = fillTile([width() - 1, 0], firstCorner, modulo + rMod);
  var nwCorner = fillTile([width() - 1, height() - 1], firstCorner, modulo + rMod);
  trace('SE corner 7/8 = ' + seCorner);
  trace('NE corner 7/8 = ' + neCorner);
  trace('SW corner 7/8 = ' + swCorner);
  trace('NW corner 7/8 = ' + nwCorner);

  var sseCorner = fillTile([0, 0], secondCorner, modulo + rMod);
  var nneCorner = fillTile([0, height() - 1], secondCorner, modulo + rMod);
  var sswCorner = fillTile([width() - 1, 0], secondCorner, modulo + rMod);
  var nnwCorner = fillTile([width() - 1, height() - 1], secondCorner, modulo + rMod);
  trace('SSE corner 1/8 = ' + sseCorner);
  trace('NNE corner 1/8 = ' + nneCorner);
  trace('SSW corner 1/8 = ' + sswCorner);
  trace('NNW corner 1/8 = ' + nnwCorner);

  var testCycles = [0, 1, 2, 3, cycles];
  var prevSum = 0;
  var prevDelta = 0;
  for (var cyc of testCycles) {
    // Count by verticals.
    // 3 edge cases:
    var leftTip = wPoint + nnwCorner + sswCorner;
    var rightTip = ePoint + nneCorner + sseCorner;
    var middle = nPoint + sPoint;
    // All other edge angles are the same for every vertical
    var leftAngles = nnwCorner + nwCorner + sswCorner + swCorner;
    var rightAngles = nneCorner + neCorner + sseCorner + seCorner;
    // Get the tile dimensions
    var axisHeight = cyc * 2;  // # of full cells along a home axis
    var sideHeight = axisHeight - 1;  // # of full cells, not counting origin
    var edgeSum = middle + leftTip + rightTip + sideHeight * (leftAngles + rightAngles);
    // Calculate the number of filled tiles (odd and even variants)
    var evenCount = axisHeight + (sideHeight + 1) * sideHeight;
    var oddCount = sideHeight + (sideHeight - 1) * sideHeight;  // == evenCount - 1 - 2 * sideHeight
    var sum = edgeSum + evenCount * evenFull + oddCount * oddFull;
    var delta = sum - prevSum;
    trace(cyc + ' cycles + ' + rMod + ' = ' + (cyc * modulo + rMod) + ' steps reaches ' + sum + ' (delta=' + delta + ', dd=' + (delta-prevDelta) + ')');
    prevSum = sum;
    prevDelta = delta;
  }



  // // At radius, the points will be N tiles from start
  // // N will be even, because at rMod, N = 0, and each modulo adds N+=2
  // var tileDistance = Math.floor((radius - firstSide) / width());
  // trace('Full tiles between start and points = ' + tileDistance);
  // // Interior height will always be odd (because of reflection around 0)
  // // Because rMod < width(), there will always be one more odd that even
  // trace('Full interior height = ' + (tileDistance * 2 + 1));
  // var cOdds = Math.ceil((tileDistance * 2 + 1) / 2);
  // var cEvens = cOdds - 1;
  // var smCorners = cEvens;  // where odds meet evens, small (1/8) corners
  // var lgCorners = cOdds;  // where odds meet odds, or evens meet evens, large (7/8) corners
  // // trace('On axis, ' + cEvens + ' evens and ' + cOdds + ' odds');
  // cOdds += 2 * ((1 + cOdds - 2) * tileDistance / 2);
  // cEvens += 2 * ((cEvens - 2) * tileDistance / 2);
  // trace('Total evens = ' + cEvens + '; odds = ' + cOdds);
  
  // var sum = cEvens * evenFull + cOdds * oddFull;
  // sum += sPoint + nPoint + ePoint + wPoint;
  // sum += smCorners * sseCorner + lgCorners * seCorner;
  // sum += smCorners * nneCorner + lgCorners * swCorner;
  // sum += smCorners * sswCorner + lgCorners * neCorner;
  // sum += smCorners * nnwCorner + lgCorners * nwCorner;

  print(prevSum);
}

function fillTile(start, first, steps, doTrace) {
  doTrace |= false;
  var firstReached = {};
  var nextStep = [];
  nextStep.push(start);
  var odd = first % 2 == 1;
  firstReached[start] = odd;
  for (var step = first; step < steps; step++) {
    odd = !odd;
    var thisStep = nextStep;
    nextStep = [];
    while (thisStep.length > 0) {
      var p = thisStep.pop();
      var neighbors = [[p[0]-1,p[1]], [p[0]+1,p[1]], [p[0],p[1]-1], [p[0],p[1]+1]];
      for (var n of neighbors) {
        if ((charAt(n[0], n[1]) == '.' || charAt(n[0], n[1]) == 'S') && !(n in firstReached)) {
          firstReached[n] = odd;
          nextStep.push(n);
        }
      }  
    }
  }
  if (doTrace) {
    traceReached(firstReached, (r) => r == odd);
  }

  return Object.values(firstReached).filter(o => o == odd).length;
}

function traceReached(reached, lambda) {
  for (var y = 0; y < height(); y++) {
    var str = '';
    for (var x = 0; x < width(); x++) {
      var p = [x,y];
      if (p in reached) {
        str += lambda(reached[p]) ? 'O' : '.';
      }
      else {
        str += charAt(x, y);
      }
    }
    trace(str);
  }
  // colorMap('debug' + (curPart + 1));
}

function solve1() {
  findStart();
  var radius = 26501365;
  // var radius = 1000;

  var modulo = width() + height();
  var rMod = radius % modulo;

  trace('Modulo is ' + modulo);
  trace('Step target ' + radius + ' % ' + modulo + ' == ' + rMod);
  var modStep = [];
  var modReached = [0];
  var modDelta = [0];

  var firstReached = {};
  var nextStep = [];
  var step = 0;
  var prevDD = 0;
  var maxDD = 6;
  nextStep.push(start);
  firstReached[start] = 0;
  while (nextStep.length > 0 && step < modulo * 10) {
    var thisStep = nextStep;
    nextStep = [];
    step++;
    while (thisStep.length > 0) {
      var p = thisStep.pop();
      var neighbors = [[p[0]-1,p[1]], [p[0]+1,p[1]], [p[0],p[1]-1], [p[0],p[1]+1]];
      for (var n of neighbors) {
        if (wrapCharAt(n[0], n[1], step) == '.' && !(n in firstReached)) {
          firstReached[n] = step;
          nextStep.push(n);
        }
      }  
    }
    if (step % modulo == rMod) {
      var r = Object.values(firstReached).filter(v => (v % 2) == 0).length;
      var d = r - modReached[modReached.length - 1];
      var dd = d - modDelta[modDelta.length - 1];
      modStep.push(step);
      modReached.push(r);
      modDelta.push(d);
      console.log('Step ' + step + ': dd=' + dd);
      trace('Step ' + step + ' reached ' + r + ': delta ' + d + ' dd ' + dd);  // REVIEW: why doesn't 1st full modulo 
      if (dd == prevDD) {
        break;
      }
      prevDD = dd;
    }
  }

  var d = modDelta.pop();
  var dd = d - modDelta.pop();
  var r = modReached.pop();
  var s = modStep.pop();

  var reps = (radius - s) / modulo;
  var predict = r + reps * d + (reps * reps + reps) * dd / 2;

  // extrapolate
  // for (s += modulo; s <= radius; s += modulo) {
  //   d += dd;
  //   r += d;
  //   trace('... Step ' + s + ' reached ' + r + ': delta ' + d + ' dd ' + dd);
  // }

  print(predict);
}
