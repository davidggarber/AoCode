function solve1() {
  var sum = 0;
  var types = [
    [],[],[],[],[],[],[]
  ];

  for (var line of lines) {
    // Rename face cards so they sort
    line = sortable1(line);
    var ranks = {};
    var counts = [0,0,0,0,0,0];
    for (var c of line.substring(0,5).split('')) {
      if (!(c in ranks)) {
        ranks[c] = 1;
      }
      else {
        ranks[c]++;
      }
      counts[ranks[c]]++;
    }
    var type = counts[5] > 0 ? 6
      : counts[4] > 0 ? 5
      : counts[3] > 0 ? (counts[2] > 1 ? 4 : 3)
      : counts[2] > 0 ? (counts[2] * 1)
      : 0;
    types[type].push(line);
  }

  var rank = 1;
  for (var t of types) {
    t.sort();
    for (var l of t) {
      var bid = parseInt(l.split(' ')[1]);
      trace(original(l) + ': ' + rank + '*' + bid + ' = ' + (bid * rank));
      sum += bid * rank++;
    }
  }

  print(sum);
}

// Rename face cards so they sort
function sortable1(line) {
  return line.replace(/A/g,'E')
    .replace(/K/g,'D')
    .replace(/Q/g,'C')
    .replace(/J/g,'B')
    .replace(/T/g,'A');
}

function sortable2(line) {
  return line.replace(/A/g,'E')
    .replace(/K/g,'D')
    .replace(/Q/g,'C')
    .replace(/J/g,'1')  // below 2s
    .replace(/T/g,'A');
}

function original(line) {
  return line.replace(/A/g,'T')
    .replace(/B/g,'J')
    .replace(/1/g,'J')
    .replace(/C/g,'Q')
    .replace(/D/g,'K')
    .replace(/E/g,'A');
}

function solve2() {
  var sum = 0;
  var types = [
    [],[],[],[],[],[],[]
  ];

  for (var line of lines) {
    line = sortable2(line);
    var ranks = {};
    var counts = [0,0,0,0,0,0];
    for (var c of line.substring(0,5).split('')) {
      if (!(c in ranks)) {
        ranks[c] = 1;
      }
      else {
        ranks[c]++;
      }
      if (c != '1') {
        counts[ranks[c]]++;
      }
    }
    
    // Add jokers to the biggest pile
    for (var i = 5; i >= 0; i--) {
      if (i == 0 || counts[i] > 0) {
        for (var j = 1; j <= ranks['1']; j++) {
          counts[i + j] = 1;
        }
        break;
      }
    }

    var type = counts[5] > 0 ? 6
      : counts[4] > 0 ? 5
      : counts[3] > 0 ? (counts[2] > 1 ? 4 : 3)
      : counts[2] > 0 ? (counts[2] * 1)
      : 0;
    types[type].push(line);
  }

  var rank = 1;
  for (var t of types) {
    t.sort();
    for (var l of t) {
      var bid = parseInt(l.split(' ')[1]);
      trace(original(l) + ': ' + rank + '*' + bid + ' = ' + (bid * rank));
      sum += bid * rank++;
    }
  }

  print(sum);
}