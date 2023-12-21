var modules = {};

function solve1() {
  for (var line of lines) {
    var splt = line.split(' -> ');
    var name = splt[0].match(/[a-z]+/)[0];
    var mod = { name:name, type:splt[0][0], dest: splt[1].split(', '), state:false, inputs:{} };
    modules[name] = mod;
  }
  // Set all & modules' inputs to off
  for (var m of Object.keys(modules)) {
    for (var d of modules[m].dest) {
      if (d in modules && modules[d].type == '&') {
        modules[d].inputs[m] = false;
      }
    }
  }

  var repeats = {};
  var pulses = [0,0];  // lows, highs
  var queue = [];  // triples: dest, high?, src
  var btn = 0;
  var cache = null;
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


function solve2() {
  for (var line of lines) {
    var splt = line.split(' -> ');
    var name = splt[0].match(/[a-z]+/)[0];
    var mod = { name:name, type:splt[0][0], dest: splt[1].split(', '), state:false, inputs:{} };
    modules[name] = mod;
  }
  // Set all & modules' inputs to off
  for (var m of Object.keys(modules)) {
    for (var d of modules[m].dest) {
      if (d in modules && modules[d].type == '&') {
        modules[d].inputs[m] = false;
      }
    }
  }

  // var pulses = [0,0];  // lows, highs
  var queue = [];  // triples: dest, high?, src
  var btn = 0;
  while (++btn > 0) {

    // implicit module 'button' fires a low pulse to broadcast
    queue.push(['broadcaster', false, 'button']);

    while (queue.length > 0) {
      var signal = queue.splice(0, 1)[0];
      // pulses[signal[1] ? 1 : 0]++;
      var name = signal[0];
      if (name == 'rx' && !signal[1]) {
        break;
      }
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
      }
      // broadcast to all destinations
      for (var d of mod.dest) {
        queue.push([d, mod.state, name]);
      }
    }
  }

  print(btn);
}
