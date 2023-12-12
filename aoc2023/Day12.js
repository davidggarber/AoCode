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

var linePattern;
var lineRanges;
var lineDams;
function solve2() {
  var sum = 0;
  for (var line of lines) {
    var pattern = line.match(/[\#|\.|\?]+/)[0];
    linePattern = pattern + '?' + pattern + '?' + pattern + '?' + pattern + '?' + pattern + '.';
    var dams = line.match(/[0-9]+/g).map(d => parseInt(d));
    lineDams = dams.concat(dams, dams, dams, dams);

    // break into regions, and label with tightest and loosest possible packings
    lineRanges = linePattern.match(/[\#|\?]+/g);
    var row = calcCombos(0, 0);
    trace(row);
    sum += row;
  }
  print(sum);
}

// How many combinations of spans can fit in this range, recursing with the remainder to later ranges
function calcCombos(iRange, iDam) {
  if (iDam == lineDams.length)
  {
    // We're out of spans. Fast-forward through remaining ranges
    for (var r = iRange; r < lineRanges.length; r++) {
      if (lineRanges[r].indexOf('#') >= 0) {
        return 0;  // no spans left for required range
      }
    }
    return 1;
  }
  if (iRange >= lineRanges.length) {
    // We're out of ranges, and we have more spans (didn't get handled above)
    return 0;
  }
  // var combos = [];
  var sum = 0;
  for (var maxDam = iDam; maxDam <= lineDams.length; maxDam++) {
    var count = calcFit(iRange, iDam, maxDam);
    // combos.push([d, count]);
    sum += count;
  }
  // return combos;
  return sum;
}

// How many ways can the next set of damaged spans fit into the next range?
// Note: one way to fit is to use zero spans, and save them for later
function calcFit(iRange, nextDam, maxDam) {
  // if (damCount == 0) {
  //   return calcCombos(iRange + 1, nextDam);  // 1x combo of zero things, recursing
  // }
  var range = lineRanges[iRange];
  var count = fitInRange(range, nextDam, maxDam);
  if (count == 0) { 
    return 0; 
  }
  return count * calcCombos(iRange + 1, maxDam);
}

// Can we fit exactly N damaged spans in this range?
// Where N = maxDam - nextDam, which can be zero
function fitInRange(range, nextDam, maxDam) {
  if (nextDam == maxDam) {
    // Is the rest of this range optional? Then this works.
    // Else return zero to throw away any progress down this branch.
    return range.indexOf('#') < 0 ? 1 : 0;
  }
  if (range.length == 0) {
    return 0;  // Can't fit spans in zero range
  }
  var sum = 0;
  var dam = lineDams[nextDam++];
  for (var i = 0; i <= range.length - dam; i++) {
    if (matches(range, i, dam)) {
      sum += fitInRange(range.substring(i + dam + 1), nextDam, maxDam);  // REVIEW: +1 past end
    }
    if (range[i] == '#') {
      break;  // Can't skip #
    }
  }
  return sum;
}

// Can a span of size [dam] be placed in range at offset?
function matches(range, offset, dam) {
  if (range.substring(0, offset).indexOf('#') >= 0) { 
    return false;  // skipped required positions
  }
  if (range.length < offset + dam) {
    return false;  // not long enough
  }
  return range.length == (offset + dam)
    || range[offset + dam] != '#';  // does it end at a non-viable boundary?
}