var rules = {};

function solve1() {
  var sum = 0;
  var parts = null;
  for (var line of lines) {
    if (parts != null) {
      var vals = line.substring(1, line.length - 1).split(',');
      vals = vals.map(v => xmasVal(v));
      var part = vals.reduce((pv, cv) => Object.assign(pv, cv), {});
      parts.push(part);
    }
    else if (line.length == 0) {
      parts = [];
    }
    else {
      var name = line.split('{')[0];
      // var tests = line.match(/(?<=[\{|\,}])^[\}|\,]+/g);
      var tests = line.substring(name.length + 1, line.length - 1).split(',');
      tests = tests.map(t => '' + t);
      rules[name] = [];
      for (var t of tests) {
        var comp = (t.length > 1 && t[1] == '<' || t[1] == '>') ? t[1] : null;
        var field = comp ? t[0] : null;
        var val = comp ? parseInt(t.match(/[0-9]+/)) : 0;
        var dest = comp ? t.split(':')[1] : t;
        rules[name].push({field:field, lt:comp == '<', val:val, dest:dest});
      }
    }
  }

  for (var p of parts) {
    var rule = 'in';
    while (rule != 'A' && rule != 'R') {
      rule = ruleDest(rules[rule], p);
    }
    if (rule == 'A') {
      sum += Object.values(p).reduce((pv, cv) => pv + cv);
    }
  }

  print(sum);
}

function xmasVal(val) {
  var obj = {};
  obj[val[0]] = parseInt(val.substring(2));
  return obj;
}

function ruleDest(rule, part) {
  for (var r of rule) {
    if (!r.field) {
      return r.dest;
    }
    if (r.lt ? (part[r.field] < r.val) : (part[r.field] > r.val)) {
      return r.dest;
    }
  }
}

function solve2() {
  var range = {x:[1,4000],m:[1,4000],a:[1,4000],s:[1,4000]};
  var queue = [['in', range]];
  
  var sum = 0;
  while (queue.length > 0) {
    var group = queue.pop();
    range = group[1];
    var size = Object.values(range);
    size = size.reduce((pv, cv) => pv * Math.max(0, (cv[1] + 1 - cv[0])), 1);
    if (group[0] == 'R' || size == 0) {
      continue;
    }
    if (group[0] == 'A') {
      sum += size;
      continue;
    }
    var rule = rules[group[0]];
    for (var r of rule) {
      if (r.field) {
        if (r.lt) {
          if (range[r.field][0] < r.val) {
            var yesRange = structuredClone(range);
            yesRange[r.field][1] = r.val - 1;
            range[r.field][0] = r.val;  // fall through with the remainder
            queue.push([r.dest, yesRange]);
          }
        }
        else {
          if (range[r.field][1] > r.val) {
            var yesRange = structuredClone(range);
            yesRange[r.field][0] = r.val + 1;
            range[r.field][1] = r.val;  // fall through with the remainder
            queue.push([r.dest, yesRange]);
          }
        }
      }
      else {
        queue.push([r.dest, range]);
        break;
      }
    }
  }

  print(sum);
}
