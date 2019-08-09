let wasm = '../out/main.wasm';
let instance = null;
let memoryStates = new WeakMap();
let mode = 1
let ctx

function syscall(instance, n, args) {
  switch (n) {
    default:
      // console.log("Syscall " + n + " NYI.");
      break;
    case /* brk */ 45: return 0;
    case /* writev */ 146:
      return instance.exports.writev_c(args[0], args[1], args[2]);
    case /* mmap2 */ 192:
      // debugger;
      const memory = instance.exports.memory;
      let memoryState = memoryStates.get(instance);
      const requested = args[1];
      if (!memoryState) {
        memoryState = {
          object: memory,
          currentPosition: memory.buffer.byteLength,
        };
        memoryStates.set(instance, memoryState);
      }
      let cur = memoryState.currentPosition;
      if (cur + requested > memory.buffer.byteLength) {
        const need = Math.ceil((cur + requested - memory.buffer.byteLength) / 65536);
        memory.grow(need);
      }
      memoryState.currentPosition += requested;
      return cur;
  }
}

let s = "";
fetch(wasm).then(response =>
  response.arrayBuffer()
).then(bytes =>
  WebAssembly.instantiate(bytes, {
    env: {
      __syscall0: function __syscall0(n) { return syscall(instance, n, []); },
      __syscall1: function __syscall1(n, a) { return syscall(instance, n, [a]); },
      __syscall2: function __syscall2(n, a, b) { return syscall(instance, n, [a, b]); },
      __syscall3: function __syscall3(n, a, b, c) { return syscall(instance, n, [a, b, c]); },
      __syscall4: function __syscall4(n, a, b, c, d) { return syscall(instance, n, [a, b, c, d]); },
      __syscall5: function __syscall5(n, a, b, c, d, e) { return syscall(instance, n, [a, b, c, d, e]); },
      __syscall6: function __syscall6(n, a, b, c, d, e, f) { return syscall(instance, n, [a, b, c, d, e, f]); },
      
      putc_js: function (c) {
        c = String.fromCharCode(c);
        if (c == "\n") {
          console.log(s);
          s = "";
        } else {
          s += c;
        }
      },
      
      drawGuiLitePixel: function (x, y, color) {
        ctx.fillStyle = 'rgb(' + ((color & 0xff0000) >> 16) + ', ' + ((color & 0xff00) >> 8) + ', ' + (color & 0xff) + ')'
        ctx.fillRect(x, y, 1, 1)
      }
    }
  })
).then(results => {
  var canvas = document.getElementById("Canvas")
  ctx = canvas.getContext("2d")

  instance = results.instance;
  instance.exports.main(mode);

  if(mode != 0) { // mode 1
    setInterval(function(){
        instance.exports.updateHelloWave()
      }, 30);
    return;
  }
  
  // mode 0
  var buffer = new Uint8Array(instance.exports.memory.buffer)
  setInterval(function(){
    var pointer = instance.exports.updateHelloWave()
    for(let y = 0; y < 320; y++) {
      for(let x = 0; x < 240; x++) {
        ctx.fillStyle = 'rgb(' + buffer[pointer + 2] + ', ' + buffer[pointer + 1] + ', ' + buffer[pointer] + ')'
        ctx.fillRect(x, y, 1, 1)
        pointer = pointer + 4
      }
    }
  }, 30);
}).catch(console.error);
