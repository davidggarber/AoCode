var modules = {};

function init1() {
  var mods = {};
  for (var line of lines) {
    var splt = line.split(' -> ');
    var name = splt[0].match(/[a-z]+/)[0];
    var mod = { name:name, type:splt[0][0], dest: splt[1].split(', '), state:false, inputs:{} };
    if (!(name in renames)) {
      renames[name] = name;
    }
    mods[name] = mod;
  }
  // Set all & mods' inputs to off
  for (var m of Object.keys(mods)) {
    for (var d of mods[m].dest) {
      if (d in mods && mods[d].type == '&') {
        mods[d].inputs[m] = false;
      }
    }
  }
  for (var n of Object.keys(renames)) {
    oldNames[renames[n]] = n;
  }
  return mods;
}

var oldNames = {};
var renames = {
  "kn": "kn.1",
  "zk": "kn.2",
  "vz": "kn.3",
  "hc": "kn.4",
  "xc": "kn.5",
  "rk": "kn.6",
  "dc": "kn.7",
  "lr": "kn.8",
  "vj": "kn.9",
  "sp": "kn.a",
  "hz": "kn.b",
  "hx": "kn.c",
  "bl": "kn.&",
  "ks": "kn.~",
  "fb": "fb.1",
  "zd": "fb.2",
  "gv": "fb.3",
  "lx": "fb.4",
  "rs": "fb.5",
  "hp": "fb.6",
  "bj": "fb.7",
  "rd": "fb.8",
  "tp": "fb.9",
  "sm": "fb.a",
  "bv": "fb.b",
  "qb": "fb.c",
  "vv": "fb.&",
  "jt": "fb.~",
  "ln": "ln.1",
  "dh": "ln.2",
  "rg": "ln.3",
  "jl": "ln.4",
  "js": "ln.5",
  "ng": "ln.6",
  "ql": "ln.7",
  "rb": "ln.8",
  "qj": "ln.9",
  "xs": "ln.a",
  "cn": "ln.b",
  "sb": "ln.c",
  "pv": "ln.&",
  "sx": "ln.~",
  "vl": "vl.1",
  "cz": "vl.2",
  "cg": "vl.3",
  "dk": "vl.4",
  "lf": "vl.5",
  "gc": "vl.6",
  "qc": "vl.7",
  "bs": "vl.8",
  "cb": "vl.9",
  "nc": "vl.a",
  "rj": "vl.b",
  "nr": "vl.c",
  "mr": "vl.&",
  "kb": "vl.~"
};

var chains = {};

function solve1() {
  modules = init1();

  var repeats = {};
  var pulses = [0,0];  // lows, highs
  var queue = [];  // triples: dest, high?, src
  var btn = 0;
  var cache = null;
  var button_pushes = 19999;
  while (btn < button_pushes) {

    // implicit module 'button' fires a low pulse to broadcast
    queue.push(['broadcaster', false, 'button']);

    while (queue.length > 0) {
      var signal = queue.splice(0, 1)[0];
      pulses[signal[1] ? 1 : 0]++;
      var name = signal[0];
      if (!(name in modules)) {
        continue;
      }
      var mod = modules[name];
      if (mod.type == '%') {
        if (signal[1]) {
          continue;  // do nothing if input is high/true
        }
        else {
          mod.state = !mod.state;
        }
      }
      else if (mod.type == '&') {
        mod.inputs[signal[2]] = signal[1];
        mod.state = !Object.values(mod.inputs).reduce((pv, cv) => pv & cv);
        if (Object.values(mod.inputs).length > 1 && mod.state == false) {
          console.log(mod.name + ' completed on step ' + btn);
          if (!(mod.name in chains)) {
            chains[mod.name] = [];
          }
          chains[mod.name].push(btn);
        }
        else if (mod.name == 'zh') {
          var count = Object.values(mod.inputs).filter(i => i).length;
          if (count >= 1) {
            console.log('zh has ' + count + ' highs on step ' + btn);
          }
        }
      }
      // broadcast to all destinations
      for (var d of mod.dest) {
        queue.push([d, mod.state, name]);
      }
    }

    btn++;
    var cache = Object.values(modules).map(m => m.name + ':' + m.state + ',' + Object.values(m.inputs).reduce((pv, cv)=>(pv << 1) | (cv ? 1 : 0), 0));
    cache = cache.join(';');
    if (cache in repeats) {
      break;
    }
    repeats[cache] = [btn, pulses[0], pulses[1]];
  }
  trace1();

  if (btn != button_pushes) {
    var cycle = btn - repeats[cache][0];
    var mod = (button_pushes - btn) % cycle;
    var loops = Math.floor((button_pushes - btn) / cycle);
    var off = repeats[cache][0] + mod;
    var offset = Object.values(repeats).filter(r => r[0] == off)[0];
  
    pulses[0] += (pulses[0] - repeats[cache][1]) * loops + offset[1] - repeats[cache][1];
    pulses[1] += (pulses[1] - repeats[cache][2]) * loops + offset[2] - repeats[cache][2];  
  }
  print(pulses[0] * pulses[1]);
}

function trace1() {
  var list = Object.keys(oldNames);
  list = list.sort();
  for (var k of list) {
    var m = modules[oldNames[k]];
    var state = (m.state ? 'H' : 'l');
    if (m.type == '&') {
      var inp = Object.keys(m.inputs).map(k => renames[k]).sort();
      state = inp.map(i => m.inputs[oldNames[i]] ? 'H' : 'l').join(',');
      state = '[' + state + '] => ' + (m.state ? 'H' : 'l');
    }
    var name = '  (' + m.name + ')';
    if (m.dest.length > 1) {
      name += ' --> ' + m.dest.length;
    }
    trace(m.type + k + ' = ' + state + name);
  }
}

var High = 1;
var Low = 0;
var Carry = 2;
var exit = 'rx';
var preExit = null;

function init2() {
  var mods = {};
  for (var line of lines) {
    var splt = line.split(' -> ');
    var name = splt[0].match(/[a-z]+/)[0];
    var mod = { name:name, type:splt[0][0], dest: splt[1].split(', '), state:Low, inputs:{}, bits:0, raw:line };
    mods[name] = mod;
  }
  // Append the 'rx' exit module
  mods['rx'] = { name:'rx', type:'!', dest: [], state:Low, inputs:{}, bits:0, raw:'' };

  // Identify all & mods' inputs, and give them an order, which indicates which bit they effect
  for (var m of Object.values(mods)) {
    for (var d of m.dest) {
      mods[d].inputs[m.name] = Low;
    }
  }
  preExit = Object.values(mods).filter(m => m.dest.length == 1 && m.dest[0] == exit)[0];
  return mods;
}

// IDEA: think of high=0/false, low=1/true
//   flip = 1 by default
//   flip + 0 == noop
//   flip + 1 == add and carry
//   conj = all true by default
//   call next with compound-or: any lows sends true, else false
// Example:
//   broadcast -> a, b
//     a += 1, b += 1
//   %a -> b
//     after a += 1, b += carry


function solve2() {
  for (var c of Object.keys(chains)) {
    trace('Chain ' + c);
    trace('  ' + chains[c].join(','));
    var deltas = "  Deltas: ";
    for (var i = 1; i < chains[c].length; i++) {
      deltas += (chains[c][i] - chains[c][i - 1]) + ', ';
    }
    trace(deltas);
  }
  print(Object.values(chains).map(c => c[1] - c[0]).reduce((pv,cv)=>pv * cv, 1));
}

var ffFamilies = [];
function refactorCounters() {
  // Find sequences of flip-flops that lead only to 0/1 other flip-flop, and optionally 1 recurring conjunction
  var broadcast = modules['broadcaster'];
  for (var mm of broadcast.dest) {
    var m = modules[mm];
    var fff = [];
    var ffConj = null;
    var n = m;
    var isCounter = true;
    while (isCounter && n != null) {
      fff.push(n);
      var cc = n.dest.filter(d => modules[d].type == '&');  // n can have 1 conjunction dest, but it must be consistent throughout chain
      if (cc.length > 1 || (cc.length == 1 && ffConj != null && ffConj != cc[0])) {
        isCounter = false;
      }
      else if (ffConj == null && cc.length == 1) {
        ffConj = cc[0];
      }
      var nn = n.dest.filter(d => modules[d].type != '&');  // can have have 1 flip-flop dest
      if (nn.length > 1) {
        isCounter = false;
      }
      else {
        n = nn.length == 1 ? modules[nn[0]] : null;
      }
    }
    if (isCounter) {
      // Found a counter!
      console.log('Found a ' + fff.length + '-bit counter: ' + m.name);
      console.log('  Conjunction: ' + ffConj);
      // TODO: scan inputs for anomalies

      var conj = modules[ffConj];
      var counter = { start:m, conj:conj, state:0, max:0, zeroTo:0, dest:[], inputs:{}, raw:'+' + mm };
      var bit = 0;
      var ffSteps = fff.map(f => f.name);
      for (var f of fff) {
        f['bit'] = 0x1 << bit++;
        // var inp = f.inputs.filter(i => i.name != ffConj && ffSteps.indexOf(i.name) < 0);
        // if (bit > 1 && inp.length > 0) {
        //   console.log('  External twiddling of ' + f.name + ' from ' + inp.map(i => i.name).join(','));
        // }
        // if (f.inputs.filter(i => i.name == ffConj).length == 1) {
        //   counter.max |= f.bit;
        // }
        // if (conj.dest.indexOf(f.name) >= 0) {
        //   counter.zeroTo |= f.bit;
        // }
        // trace('"' + f.name + '": "' + mm + '.' + bit.toString(16) + '",');
      }
      counter.max = Object.keys(conj.inputs).map(i => modules[i].bit).reduce((pv, cv) => pv | cv);
      counter.zeroTo = conj.dest.map(d => modules[d].bit).reduce((pv, cv) => pv | cv);

      for (var f of conj.dest) {
        if (ffSteps.indexOf(f) < 0) {
          counter.dest.push(f);
        }
      }
      // Rename conj as inputs elsewhere
      // for (var z of Object.values(modules)) {
      //   z.inputs = z.inputs.map(i => i == ffConj ? mm : i);
      // }
      counter.raw += ' [' + counter.max + ' @ ' + counter.zeroTo; 
      counter.raw += ' -> ' + counter.dest.join(','); 
      console.log(counter.raw);
      ffFamilies.push(counter);
      // modules[mm] = counter;
    }
  }
}

function refactorInverters() {
  var invs = Object.values(modules).filter(m => m.type == '&' && Object.keys(m.inputs).length == 1);
  for (var i of invs) {
    i.type = '~';
    i.raw = '~' + i.name + ' -> ' + i.dest.join(',');
  }
}

function traceTree() {
  // trace as tree
  var treed = {};
  var queue = [['broadcaster',0]];
  while (queue.length > 0) {
    var q = queue.pop();
    if (q[0] in treed) {
      continue;
    }
    treed[q[0]] = true;

    if (!(q[0] in modules)) {
      var str = Array(q[1] + 1).join(' ');  // indent
      trace(str + '** ' + q[0]);
      continue;
    }
    var m = modules[q[0]];
    var str = Array(q[1] + 1).join(' ');  // indent
    trace(str + Object.keys(m.inputs).length + '> ' + m.raw);

    for (var d of m.dest.filter(dd => modules[dd].type == '&').reverse()) {
      queue.push([d, q[1] + 1]);
    }
    for (var d of m.dest.filter(dd => modules[dd].type != '&').reverse()) {
      queue.push([d, q[1] + 1]);
    }
  }
}

function traceStates() {
  var tree = [['broadcaster', 0]];
  while (tree.length > 0) {
    var t = tree.pop();
    var m = modules[t[0]];
    var str = Array(t[1] + 1).join(' ');  // indent
    if (m.type == '~') {
      trace(str + Object.keys(m.inputs)[0].name + ' > ' + m.raw);
    }
    else {
      trace(str + m.raw);
    }
    for (var d of m.dest) {
      tree.push([d, t[1] + 1]);
    }
  }
}

function counterPattern(cnt) {
  trace('Counter from ' + cnt.start.name + ' to ' + cnt.conj.name);
  trace('Expecting rollover at ' + cnt.max + ' to ' + cnt.zeroTo);

  var resets = [];
  var btn = 0;
  var cache = null;
  var button_pushes = cnt.max * 5;
  while (resets.length < 8) {
    btn++;

    // implicit module 'button' fires a low pulse to broadcast
    var queue = [];  // triples: dest, high?, src
    queue.push([cnt.start.name, Low, 'broadcaster']);

    while (queue.length > 0) {
      var signal = queue.splice(0, 1)[0];
      var name = signal[0];
      if (name == preExit.name && signal[1] == High) {
        console.log('Counter ' + cnt.start.name + ' reset at ' + btn);
        resets.push(btn);
        break;
      }
      var mod = modules[name];
      if (mod.type == '%') {
        if (signal[1] == High) {
          continue;  // do nothing if input is high
        }
        else {
          mod.state = 1 - mod.state;
        }
      }
      else if (mod.type == '~') {
        mod.state = 1 - signal[1];
      }
      else if (mod.type == '&') {
        mod.inputs[signal[2]] = signal[1];
        mod.state = Object.values(mod.inputs).filter(i => i == Low).length == 0 ? Low : High;
        if (mod.state == Low) {
          console.log(mod.name + ' completed on step ' + btn + ' from ' + signal[2]);
        }
      }
      // broadcast to all destinations
      for (var d of mod.dest) {
        queue.push([d, mod.state, name]);
      }
    }
  }

    trace(cnt.start.name + ' branch:');
    trace('  ' + resets.join(','));
    trace('  delta: ' + (resets[resets.length - 1] - resets[resets.length - 2]));

}