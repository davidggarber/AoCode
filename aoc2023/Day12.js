function solve1() {
  var sum = 0;
  var progress = 0;
  for (var line of lines) {
    console.log(++progress);
    var springs = line.match(/\#+|\.+|\?+/g);
    var dams = line.match(/[0-9]+/g).map(d => parseInt(d));

    var bits = 0;
    for (var s of springs) {
      if (s[0] == '?') { bits += s.length; }
    }
    var row = 0;
    for (var i = 0; i < (0x1 << bits); i++) {
      var str = '';
      var bit = 1;
      for (var s of springs) {
        if (s[0] != '?') { str += s; }
        else {
          for (var j = 0; j < s.length; j++) {
            if ((i & bit) != 0) {
              str += '#';
            }
            else {
              str += '.';
            }
            bit <<= 1;
          }
        }
      }
      if (follows(str, dams)) {
        sum++;
        row++;
      }
    }
    trace(row + ' in ' + bits + ' bits');
  }

  print(sum);
}

function follows(line, dams) {
  var springs = line.match(/\#+/g);
  if (springs == null) { return dams.length == 0 || dams[0] == 0; }
  if (springs.length != dams.length) { return false; }
  var d = 0;
  for (var s of springs) {
    if (s.length != dams[d++]) {
      return false;
    }
  }
  return true;
}


function solve2() {
  var sum = 0;
  var progress = 0;
  for (var line of lines) {
    console.log(++progress);
    var pattern = line.match(/[\#|\.|\?]+/)[0];
    pattern = pattern + '?' + pattern + '?' + pattern + '?' + pattern + '?' + pattern + '.';
    var patLen = pattern.length;
    var dams = line.match(/[0-9]+/g).map(d => parseInt(d));
    dams = dams.concat(dams, dams, dams, dams);

    var starts = [];  // all valid starting offset
    var lengths = [];  // all valid lengths at each starting offset
    var lastMandatory = 0;
    for (var i = 0; i < pattern.length; i++) {
      if (pattern[i] == '#') {
        lastMandatory = i;
      }
      if ((i == 0 || pattern[i - 1] != '#') && pattern[i] != '.') {
        starts.push(i);
        var lens = [];
        for (var j = i + 1; j <= pattern.length - 1; j++) {
          if (pattern[j] == '.') {
            lens.push(j - i);
            break;
          }
          else if (pattern[j] == '?') {
            lens.push(j - i);
            // no break
          }
        }
        lengths.push(lens);
      }
    }
    starts.push(patLen + 1);

    // var ends = [];
    // for (var i = 1; i < pattern.length; i++) {
    //   if (pattern[i - 1] != '.' && pattern[i] != '#') {
    //     ends.push(i);
    //     for (var j = 0; j < starts.length && starts[j] < i; j++) {
    //       lengths[j].push(i - starts[j]);
    //     }
    //   }
    // }
    // ends.push(patLen);

    // initially justify left
    var places = [];
    var s = 0;
    for (var d of dams) {
      while (lengths[s].indexOf(d) < 0) {
        s++;
      }
      // while (ends.indexOf(starts[s] + d) < 0) {
      //   s++;
      // }
      places.push(s);
      var len = starts[s] + d;
      while (starts[s] <= len) {
        s++;
      }
    }
    var maxP = dams.length;
    places.push(starts.length - 1);

    var row = 0;
    var loops = 0;
    var tries = 0;
    // incrementally shift everything to the right
    var p = maxP - 1;
    while (p >= 0) {
      loops++;
      if (starts[places[maxP - 1]] + dams[maxP - 1] >= lastMandatory) {
        tries++;
        var test = makeFromPlaces(starts, places, dams, patLen);
        if (matches(pattern, test)) {
          row++;
          // if (0 == row % 10000) { console.log(row); }
        }  
      }

      p = maxP - 1;
      for ( ; p >= 0; p--) {
        // Could this move right?
        var shift = 1;
        var nextEnd = starts[places[p] + shift] + dams[p];
        while (nextEnd < starts[places[p + 1]]) {
          if (lengths[places[p] + shift].indexOf(dams[p]) >= 0) { break; }
          shift++;
          nextEnd = starts[places[p] + shift] + dams[p];
        }
        if (nextEnd < starts[places[p + 1]]) { break; }
      }
      if (p >= 0) {
        // group p could move right to the next starting point
        places[p] += shift;
        // reset every p after it to follow as tightly left as possible
        var s = places[p];
        for (var q = p; q < maxP; q++) {
          while (lengths[s].indexOf(dams[q]) < 0) {
            s++;
          }    
          places[q] = s;
          len = starts[s] + dams[q];
          while (starts[s] <= len) {
            s++;
          }
        }
      }
    }

    trace(row + ' in ' + tries + ' tries (' + loops + ' shifts)');
    sum += row;
  }

  print(sum);
}

var dots = new Array(50).join('.');
var pounds = new Array(50).join('#');

function makeFromPlaces(starts, places, dams, patlen) {
  var str = '';
  for (var i = 0; i < dams.length; i++) {
    var pos = starts[places[i]];
    str += dots.substring(0, pos - str.length);
    str += pounds.substring(0, dams[i]);
  }
  str += dots.substring(0, patlen - str.length);
  return str;
}

function matches(pattern, str) {
  for (var i = 0; i < pattern.length; i++) {
    if (pattern[i] != '?' && pattern[i] != str[i]) { return false; }
  }
  return true;
}