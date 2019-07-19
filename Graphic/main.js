let layer1, layer2;
let angle = 0;
let step_layer = 0;
let step_surface = 0;

function setup() {
  layer1 = loadImage('assets/layer1.png')
  layer2 = loadImage('assets/layer2.png')
  createCanvas(700, 600, WEBGL);
}

function drawLayer(layer, x, y, z){
  push();
  translate(x, y, z);
  rotateX(-1.2);
  //texture(layer);
  plane(200, 200);
  pop();
}

function showLayers() {
  drawLayer(layer1, step_layer, step_layer, -100);
  drawLayer(layer2, -step_layer, -step_layer, -100);
  if (step_layer < 25) {
  	step_layer++;
  }
}

function showSurface(){
  if (step_surface > 220) {
  	return true;
  }
  step_surface += 10;
  drawLayer(layer1, 0, 0, -100);
  drawLayer(layer2, -step_surface, 0, -100);
  drawLayer(layer2, step_surface, 0, -100);
  return false;
}

function draw() {
  background(0);
  if (showSurface()) {
  	showLayers();
  }
  angle += 0.05;
}
