var start = [];

function findStart() {
  for (var y = 0; y < height(); y++) {
    var x = lines[y].indexOf('S');
    if (x >= 0) {
      return start = [x,y];
    }
  }
}

function solve1() {
  findStart();
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
  nextStep.push(start);
  firstReached[start] = [0,0,0,start[0],start[1]];  // step reached, tileX, tileY, relX, relY
  var tileSums = {};
  while (nextStep.length > 0 && step <= (modulo * 3 + rMod)) {
    var thisStep = nextStep;
    nextStep = [];
    step++;
    while (thisStep.length > 0) {
      var p = thisStep.pop();
      var neighbors = [[p[0]-1,p[1]], [p[0]+1,p[1]], [p[0],p[1]-1], [p[0],p[1]+1]];
      for (var n of neighbors) {
        if (wrapCharAt(n[0], n[1], step) == '.' && !(n in firstReached)) {
          var xTile = Math.floor(n[0] / width());
          var yTile = Math.floor(n[1] / height());
          firstReached[n] = [step, xTile, yTile,n[0] - xTile * width(),n[1] - yTile * height()];
          nextStep.push(n);
        }
      }  
    }
    if (step % modulo == rMod) {
      var oddness = step % 2;
      var r = Object.values(firstReached).filter(v => (v[0] % 2) == oddness).length;
      var d = r - modReached[modReached.length - 1];
      var dd = d - modDelta[modDelta.length - 1];
      modStep.push(step);
      modReached.push(r);
      modDelta.push(d);
      console.log('Step ' + step + ': dd=' + dd);
      trace('Step ' + step + ' reached ' + r + ': delta ' + d + ' dd ' + dd);
      
      // var dist = Math.ceil((step - rMod) / width());
      // if (dist == 2) {  // Draw out the pattern at step 2
      //   console.log('Trace ' + (dist * 2) + ' x ' + (dist * 2));
      //   for (var y = -dist; y <= dist; y++) {
      //     var line = '';
      //     for (var x = -dist; x <= dist; x++) {
      //       console.log(x + ',' + y);  // Filtering is slow
      //       var name = y == 0 ? '' : y > 0 ? Array(y+1).join('S') : Array(1-y).join('N');
      //       name += x == 0 ? '' : x > 0 ? Array(x+1).join('E') : Array(1-x).join('W');
      //       var tile = Object.values(firstReached).filter(e => e[0] % 2 == 0 && e[1] == x && e[2] == y).length;
      //       tileSums[name] = tile;
      //       line += '      '.substring(('' + tile).length) + tile;
      //     }
      //     trace(line);
      //   }
      // }
      
      if (dd == prevDD) {
        break;
      }
      prevDD = dd;
    }
  }

  // Turn this into a simplified quadratic: ax^2 + bx + c, where x is # of cycles
  //   ignore first cycle, in case not yet in the pattern
  // Three points:
  //    (1, i:modReached[2])
  //    (2, j:modReached[3])
  //    (3, k:modReached[4])
  //  Plug in explicit x's: 1,2,3 into quadratic
  //    i = a + b + c
  //    j = 4a + 2b + c
  //    k = 9a + 3b + c
  // subtract I from J and J from K
  //    j-i = 3a+b
  //    k-j = 5a+b
  // subtract those:
  //    k-j-(j-i) = k-2j+i = 2a
  var i = modReached[2];
  var j = modReached[3];
  var k = modReached[4];
  var a = (k - 2*j + i) / 2;
  var b = j - i - 3 * a;
  var c = i - a - b;
  // Extrapolate:
  trace('Extrapolate (' + a + ')x^2 + (' + b + ')x + ' + c);
  for (var s = 0; s < 10; s++) {
    trace('  ' + s + ': ' + (a*s*s + b*s + c));
  }

  var cycles = (radius - rMod) / modulo;
  trace(cycles + ': ' + (a*cycles*cycles + b*cycles + c));
  print(a*cycles*cycles + b*cycles + c);
}
