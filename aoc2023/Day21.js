var start = [];

function solve1() {
  for (var y = 0; y < height(); y++) {
    var x = lines[y].indexOf('S');
    if (x >= 0) {
      start = [x,y];
      break;
    }
  }

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
  var radius = 26501365;
  var modulo = width() + height();
  var rMod = radius % modulo;
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
      trace('Step ' + step + ' reached ' + r + ': delta ' + d + ' dd ' + dd);
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
