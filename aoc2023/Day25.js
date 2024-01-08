var links = [];
var comps = {};
var cComps;

function solve1() {
  for (var line of lines) {
    var parts = line.match(/[a-z]+/g);
    var left = parts.splice(0,1)[0];
    comps[left] = true;
    for (var p of parts) {
      links.push([left, p]);
      comps[p] = true;
    }
  }
  cComps = Object.keys(comps).length;

  var startRings = cComps < 30 ? 1 : 6;  // Sample data can succeed with 1 ring. Real data needs 6.
  var keys = Object.keys(comps);
  for (var rings = startRings; rings <= 10; rings++) {
    for (var start of keys) {
      reset(start, rings);
      if (shuffle(rings > 2) <= 3) {
        var group1 = Object.entries(comps).filter(e => e[1]).length;
        var group2 = cComps - group1;
        if (group1 > 0 && group2 > 0) {
          trace(group1 + ' / ' + group2 + ' split.');
          print(group1 * group2);
          return;
        }
      }
    }  
  }
}

// Pick one component at random, and move both it and all its neighbors to a second group
function reset(move, rings) {
  // trace('*****************************************');
  trace('Try starting with ' + move + ' and ' + rings + ' rings of neighbors...');
  for (var k of Object.keys(comps)) {
    comps[k] = k != move;
  }
  for (var r = 0; r < rings; r++) {
    moveFirstNeighbors();
  }
}

function shuffle(allowBothDir) {
  var crossing = countCrossings();
  var oneDir = true;
  while (crossing > 3) {
    move = null;
    var best = 0;
    var group1 = Object.entries(comps).filter(e => e[1]).length;
    var group2 = cComps - group1;
    for (var c of Object.keys(comps)) {
      if (!oneDir || comps[c]) {
        // Only consider components not yet moved
        var delta = shouldMove(c);
        // console.log('  ' + c + ': ' + delta);
        if (delta > best) {
          best = delta;
          move = c;
        }  
      }
    }
    if (move == null) {
    console.log(group1 + ' / ' + group2 + ' split with ' + crossing + ' crossings. No options.')
    if (oneDir && allowBothDir) {
        console.log('Allow both directions');
        oneDir = false;  
        continue;
      }
      else {
        // console.log('Done');
        break;
      }
    }
    comps[move] = !comps[move];
    crossing = countCrossings();  
    // console.log('Move ' + move + ' for a net improvement of ' + best + ' = ' + crossing + ' crossings');
  }

  return crossing;
}

function countCrossings() {
  var count = 0;
  // console.log('Group 2: ' + Object.entries(comps).filter(e => !e[1]).map(e => e[0]).join(', '));
  for (var l of links) {
    if (comps[l[0]] != comps[l[1]]) {
      count++;
      // console.log('  ' + l.join(' - '));
    }
  }
  // console.log('  == ' + count + ' crossings');
  return count;
}

function shouldMove(c) {
  var count = 0;
  var cross = 0;
  for (var l of links) {
    if (l[0] == c || l[1] == c) {
      count++;
      if (comps[l[0]] != comps[l[1]]) {
        cross++;
      }
    }
  }
  return cross - (count - cross);  // positive if more cross than not
}

function moveFirstNeighbors() {
  var move = [];
  for (var l of links) {
    if (comps[l[0]] != comps[l[1]]) {
      move.push(comps[l[0]] ? l[0] : l[1]);
    }
  }
  for (var m of move) {
    comps[m] = false;
  }
}

function solve2() {
  // no-op
}
