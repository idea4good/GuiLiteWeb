let layer1, layer2;
let angle = 0;
let step = 0;
let step_layer = 0;
let step_surface = 0;

document.addEventListener('keyup', function(event){
  step++;
});

function setup() {
  layer1 = loadImage('assets/layer1.png')
  layer2 = loadImage('assets/layer2.png')
  createCanvas(700, 600, WEBGL);
}

function drawLayer(layer, x, y, z, rX = -1.2, rY = 0, rZ = 0){
  push();
  translate(x, y, z);
  rotateX(rX);
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

function showSurface(x = 0){
  drawLayer(layer1, x, 0, -100);
  drawLayer(layer2, x - step_surface, 0, -100);
  drawLayer(layer2, x + step_surface, 0, -100);
  if (step_surface < 220) {
  	step_surface +=10;
  }
}

let stepShift = 0;
function shiftSurface(){
  if (stepShift < 200) {
	stepShift += 10;
  }
  showSurface(stepShift);
}

let displayRx = -1.2;
let displayY = 0;
function showDisplay(){
  drawLayer(layer1, 0, displayY, -100, displayRx);
}

function rotateDisplay(){
  if (displayRx < 0) {
  	displayRx += 0.1;
  }else if(displayY > -200){
  	displayY -= 10;
  }
  drawLayer(layer1, 0, displayY, -100, displayRx);
}

function draw() {
  background(0);

  switch(step){
  	case 0:
  	break;
  	case 1:
  	showDisplay();
  	break;
	case 2:
	rotateDisplay();
	break;
	case 3:
	showDisplay();
	showSurface();
	break;
	case 4:
	showDisplay();
	shiftSurface();
	break;
	case 5:
	showDisplay();
	showLayers();
	break;
	default:
	step = step_surface = step_layer = stepShift = 0;
	displayRx = -1.2; displayY = 0;
	break;
    }
  angle += 0.05;
}
