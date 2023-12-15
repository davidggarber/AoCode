function solve1() {
  var line = lines[0].trim();
  var sequence = line.split(',');
  var sum = sequence.map(s => hashCode(s)).reduce((accum, next) => accum + next);

  print(sum);
}

function hashCode(str) {
  var hash = 0;
  for (var i = 0; i < str.length; i++) {
    var cp = str.charCodeAt(i);
    hash = ((hash + cp) * 17) % 256;
  }
  return hash;
}

function solve2() {
  var boxes = [];
  for (var b = 0; b < 256; b++) {
    boxes.push([[],{}]);
  }
  var sequence = lines[0].split(',');
  for (var s of sequence) {
    if (s[s.length - 1] == '-') {  // name-
      var name = s.substring(0, s.length - 1);
      var hash = hashCode(name);
      var i = boxes[hash][0].indexOf(name);
      if (i >= 0) {
        boxes[hash][0].splice(i, 1);
        boxes[hash][1][name] = 0;
      }
    }
    else {  // name=#
      var name = s.substring(0, s.length - 2);
      var hash = hashCode(name);
      var lens = parseInt(s.substring(s.length - 1));
      var i = boxes[hash][0].indexOf(name);
      if (i < 0) {
        boxes[hash][0].push(name);
      }
      boxes[hash][1][name] = lens;
    }
  }

  var sum = 0;
  var b2 = boxes.map(b => b[0].map(n => b[1][n]));
  var b3 = b2.map(b => b?.reduce((pv, cv, i) => pv + cv * (i+1), 0));
  var sum = b3.reduce((pv, cv, i) => pv + cv * (i+1), 0);

  print(sum);
}
