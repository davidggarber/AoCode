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

  var keys = Object.keys(comps);
  for (var start of keys) {
    reset(start);
    if (shuffle(true) <= 3) {
      var group1 = Object.entries(comps).filter(e => e[1]).length;
      var group2 = cComps - group1;
      if (group1 > 0 && group2 > 0) {
        trace(group1 + ' / ' + group2 + ' split has <= 3 crossings.');
        print(group1 * group2);
        return;
      }
    }
  }  
}

// Pick one component at random, and move both it and all its neighbors to a second group
function reset(move) {
  // trace('*****************************************');
  for (var k of Object.keys(comps)) {
    comps[k] = k != move;
  }
  var group2 = Object.entries(comps).filter(e => !e[1]).length;
  var rings = 0;
  while (group2 * 3 < cComps) {  // Need ~1/3 of components initially in 2nd group
    moveFirstNeighbors();
    rings++;
    group2 = Object.entries(comps).filter(e => !e[1]).length;
  }
  var group1 = cComps - group2;
  trace('Try starting with ' + move + ' and ' + rings + ' rings of neighbors for a ' + group1 + '/' + group2 + ' initial split...');
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
      if (oneDir && allowBothDir) {
        console.log(group1 + ' / ' + group2 + ' split with ' + crossing + ' crossings. Allow both directions');
        oneDir = false;  
        continue;
      }
      else {
        console.log(group1 + ' / ' + group2 + ' split with ' + crossing + ' crossings. No options.')
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
