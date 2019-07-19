let layer1, layer2;
let angle = 0;

function setup() {
  layer1 = loadImage('assets/layer1.png')
  layer2 = loadImage('assets/layer2.png')
  createCanvas(300, 300, WEBGL);
}

function drawLayer(layer, x, y, z){
  push();
  translate(x, y, z);
  texture(layer);
  plane(200, 200);
  pop();
}

function draw() {
  background(0);
  rotateX(-0.8);

  drawLayer(layer1, 0, -100, -100);
  drawLayer(layer2, 50, -200, -100);

  angle += 0.05;
}
