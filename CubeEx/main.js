const cubeSize = 180;
const gain = 1.1;
const animationFrames = 300;
const transparency = 0.3;
let isWAready = false;
let instance = null;
let buffer;
let graphic;

function setup() {
  createCanvas(500, 500, WEBGL);
  angleMode(DEGREES);
  graphic = createGraphics(240, 320);
  startWebAssembly();
}

const faces = [
  [0,   0, 0, '255, 0, 0'],
  [0,  90, 0, '0, 255, 0'],
  [0, 180, 0, '255, 0, 0'],
  [0, -90, 0, '0, 255, 0'],
  [90,  0, 0, '0, 0, 255'],
  [270, 0, 0, '0, 0, 255'],
  ];

function textureFace(face, index) {
  if(!isWAready){
    return;
  }

  var pointer = instance.exports.updateHelloParticle()
  for(let y = 0; y < 320; y++) {
    for(let x = 0; x < 240; x++) {
      graphic.stroke('rgb(' + buffer[pointer + 1] + ', ' + buffer[pointer + 2] + ', ' + buffer[pointer + 3] + ')');
      graphic.ellipse(x, y, 1, 1);
      pointer = pointer + 4;
    }
  }
  texture(graphic);
}

function draw() {
  const progress = min(frameCount / animationFrames, 1);
  background('lightblue');
  noStroke();
  //rotateX(-30);
  rotateY(frameCount);
  faces.forEach((face, i) => {
    fill(`rgba(${face[3]}, ${transparency})`);
    textureFace(face, i);

    push();
    rotateX(face[0] * progress);
    rotateY(face[1] * progress);
    rotateZ(face[2] * progress);
    translate(0, 0, cubeSize / 2 * gain * progress);
    plane(cubeSize);
    pop();
  });
}

function startWebAssembly() {
  let x = 'libs/main.wasm';
  let memoryStates = new WeakMap();
  function syscall(instance, n, args) {
    switch (n) {
      default:
        // console.log("Syscall " + n + " NYI.");
        break;
      case /* brk */ 45: return 0;
      case /* writev */ 146:
        return instance.exports.writev_c(args[0], args[1], args[2]);
      case /* mmap2 */ 192:
        debugger;
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
  fetch(x).then(response =>
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
        }
      }
    })
  ).then(results => {
    instance = results.instance;
    instance.exports.main();
    buffer = new Uint8Array(instance.exports.memory.buffer)
    isWAready = true;
  }).catch(console.error);
}
