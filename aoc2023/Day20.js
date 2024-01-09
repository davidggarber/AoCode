function init1() {
  var mods = {};
  for (var line of lines) {
    var splt = line.split(' -> ');
    var name = splt[0].match(/[a-z]+/)[0];
    var mod = { name:name, type:splt[0][0], dest: splt[1].split(', '), state:false, inputs:{} };
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
  return mods;
}

function solve1() {
  var modules = init1();

  var pulses = [0,0];  // lows, highs
  var queue = [];  // triples: dest, high?, src
  var btn = 0;
  var button_pushes = 1000;
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
        }
      }
      // broadcast to all destinations
      for (var d of mod.dest) {
        queue.push([d, mod.state, name]);
      }
    }

    btn++;
  }

  print(pulses[0] * pulses[1]);
}

function findChains() {
  // Find the frequency with which each conjunction (ignoring inverters) fills up.
  // Note: it should also be discernable as bit-wise math, looking at the feeder flip-flops to each conjunction.
  //       Maybe I'll write that version separately.
  var chains = {};
  var modules = init1();

  var queue = [];  // triples: dest, high?, src
  var btn = 0;
  var button_pushes = 19999;
  while (btn < button_pushes) {

    // implicit module 'button' fires a low pulse to broadcast
    queue.push(['broadcaster', false, 'button']);

    while (queue.length > 0) {
      var signal = queue.splice(0, 1)[0];
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
      }
      // broadcast to all destinations
      for (var d of mod.dest) {
        queue.push([d, mod.state, name]);
      }
    }

    btn++;
  }
  return chains;
}

function solve2() {
  var chains = findChains();
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
