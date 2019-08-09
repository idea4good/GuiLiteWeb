//////////////////////// WebAssembly Code ////////////////////////
let instance = [null, null]
let memoryStates = new WeakMap();
let mode = 1
let buffer = null

function startWebAssembly(index, wasmFilePath, drawPixel) {
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
  fetch(wasmFilePath).then(response =>
    response.arrayBuffer()
  ).then(bytes =>
    WebAssembly.instantiate(bytes, {
      env: {
        __syscall0: function __syscall0(n) { return syscall(instance[index], n, []); },
        __syscall1: function __syscall1(n, a) { return syscall(instance[index], n, [a]); },
        __syscall2: function __syscall2(n, a, b) { return syscall(instance[index], n, [a, b]); },
        __syscall3: function __syscall3(n, a, b, c) { return syscall(instance[index], n, [a, b, c]); },
        __syscall4: function __syscall4(n, a, b, c, d) { return syscall(instance[index], n, [a, b, c, d]); },
        __syscall5: function __syscall5(n, a, b, c, d, e) { return syscall(instance[index], n, [a, b, c, d, e]); },
        __syscall6: function __syscall6(n, a, b, c, d, e, f) { return syscall(instance[index], n, [a, b, c, d, e, f]); },
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
          drawPixel(x, y, color)
        }
      }
    })
  ).then(results => {
    instance[index] = results.instance
    instance[index].exports.main(mode)
  }).catch(console.error);
}

//////////////////////// Cube Code ////////////////////////
const cubeSize = 180;
const gain = 1;
const animationFrames = 200;
let graphic1, graphic2, graphic3;

function setup() {
  createCanvas(500, 500, WEBGL);
  angleMode(DEGREES);
  graphic1 = createGraphics(240, 320);
  graphic2 = createGraphics(240, 320);
  graphic3 = createGraphics(240, 320);
  graphic3.background(210, 0, 0)
  graphic3.fill(255)
  graphic3.textSize(64)
  graphic3.text('   👑\nGuiLite\n on 3D', 20, 100);
  startWebAssembly(0, 'libs/particle.wasm', drawPixelOnGraphic1);
  startWebAssembly(1, 'libs/wave.wasm', drawPixelOnGraphic2);
}

const faces = [
  [0,   0, 0, '255, 0, 0'],
  [0,  90, 0, '0, 255, 0'],
  [0, 180, 0, '255, 0, 0'],
  [0, -90, 0, '0, 255, 0'],
  [90,  0, 0, '0, 0, 255'],
  [270, 0, 0, '0, 0, 255'],
  ];

function draw() {
  const progress = min(frameCount / animationFrames, 1);
  background('gray');
  noStroke();
  rotateX(-30);
  rotateY(frameCount);

  if(progress == 1) {
    updateGraphic(0);
    updateGraphic(1);
  }
  
  faces.forEach((face, i) => {
    textureFace(face, i, progress)

    push();
    rotateX(face[0] * progress);
    rotateY(face[1] * progress);
    rotateZ(face[2] * progress);
    translate(0, 0, cubeSize / 2 * gain * progress);
    plane(cubeSize);
    pop();
  });
}

function updateGraphic(index) {
  if(!instance[index]){
    return;
  }

  if (index == 0){
    var pointer = instance[index].exports.updateHelloParticle()
    var graphic = graphic1
  } else{
    var pointer = instance[index].exports.updateHelloWave()
    var graphic = graphic2
  }

  if(mode != 0){
    return;
  }

  if(buffer == null){
    buffer = new Uint8Array(instance[index].exports.memory.buffer)
  }
  for(let y = 0; y < 320; y++) {
    for(let x = 0; x < 240; x++) {
      graphic.stroke('rgb(' + buffer[pointer + 1] + ', ' + buffer[pointer + 2] + ', ' + buffer[pointer + 3] + ')')
      graphic.rect(x, y, 1, 1);
      pointer = pointer + 4;
    }
  }
}

function textureFace(face, i, progress) {
  if(progress < 1) {
    return fill(`rgba(${face[3]}, 0.5)`);
  }

  if(i == 0 || i == 2) {
    
  } else {
    
  }
  switch(i){
    case 0:
    case 2:
      texture(graphic2)
    break;
    case 1:
    case 3:
      texture(graphic1)
    break;
    default:
      texture(graphic3)
    break;
  }
}

function drawPixelOnGraphic1(x, y, color) {
  graphic1.stroke('rgb(' + ((color & 0xff0000) >> 16) + ', ' + ((color & 0xff00) >> 8) + ', ' + (color & 0xff) + ')')
  graphic1.rect(x, y, 1, 1);
}

function drawPixelOnGraphic2(x, y, color) {
  graphic2.stroke('rgb(' + ((color & 0xff0000) >> 16) + ', ' + ((color & 0xff00) >> 8) + ', ' + (color & 0xff) + ')')
  graphic2.rect(x, y, 1, 1);
}