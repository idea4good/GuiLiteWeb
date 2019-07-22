let displayNone, something, surface0, surface1, surface2, layer1, layer2;
let angle = 0;
let step = 0;
let step_layer = 0;
let step_surface = 0;

document.addEventListener('keyup', function(event){
  step++;
});

function setup() {
  displayNone = loadImage('assets/displayNone.png')
  surface0 = loadImage('assets/surface0.png')
  surface1 = loadImage('assets/surface1.png')
  surface2 = loadImage('assets/surface2.png')
  layer1 = loadImage('assets/layer1.png')
  layer2 = loadImage('assets/layer2.png')
  something = createGraphics(200, 200);
  createCanvas(800, 600, WEBGL);
}

function drawPlane(in_texture, x, y, z, rX = -1.2, rY = 0, rZ = 0){
  push();
  translate(x, y, z);
  rotateX(rX);
  texture(in_texture);
  plane(200, 200);
  pop();
}

function showLayers(texture1 = layer1, texture2 = layer2, rX = -1.2) {
  if (step_layer < 25) {
    step_layer++;
  }
  drawPlane(texture1, step_layer, step_layer, -120, rX);
  drawPlane(texture2, -step_layer, -step_layer, -100, rX);
}

function showSurface(x = 0){
  drawPlane(surface0, x - step_surface, 0, -100);
  drawPlane(surface1, x, 0, -100);
  drawPlane(surface2, x + step_surface, 0, -100);
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
function showDisplay(what){
  drawPlane(what, 0, displayY, -100, displayRx);
}

function rotateDisplay(){
  if (displayRx < 0) {
  	displayRx += 0.1;
  }else if(displayY > -240){
  	displayY -= 10;
  }
  drawPlane(displayNone, 0, displayY, -100, displayRx);
}

let layerRx = -1.2
function rotateLayers(){
  if (layerRx < 0) {
    layerRx += 0.1;
  }
  showLayers(layer1, layer2, layerRx);
}

let stepLayerVisibleRect = 0;
function updateLayersVrect(layerRx = -1.2){
  if (stepLayerVisibleRect < 100) {
    stepLayerVisibleRect++;
  }
  something.background(255, 0, 0);
  something.fill(255, 255, 0);
  something.rect(0, 0, stepLayerVisibleRect, stepLayerVisibleRect);
  showDisplay(something);

  something.background(255, 255, 0);
  something.strokeWeight(4);
  something.rect(0, 0, stepLayerVisibleRect, stepLayerVisibleRect);
  something.fill(62, 72, 204);
  something.textSize(14);
  something.textStyle(BOLD);
  something.text(' Visble RECT\nLayer 2', stepLayerVisibleRect, stepLayerVisibleRect);
  showLayers(layer1, something, layerRx);
}

function draw() {
  background(0);

  switch(step){
  	case 0:// clear screen
  	break;
  	case 1:// show display from side
  	showDisplay(displayNone);
  	break;
  	case 2:// show display from front
  	rotateDisplay();
  	break;
  	case 3:// show display & surfaces
  	showDisplay(surface1);
  	showSurface();
  	break;
  	case 4:// flip surface
  	showDisplay(surface0);
  	shiftSurface();
  	break;
  	case 5:// show display & layers
    something.background(255, 0, 0);
    something.fill(255, 255, 0);
  	showDisplay(something);
  	showLayers();
  	break;
    case 6:// demo visble rect
    updateLayersVrect();
    break;
    case 7:// reset display & layers
    something.background(255, 0, 0);
    something.fill(255, 255, 0);
    showDisplay(something);
    showLayers();
    break;
    case 8:// show layers from front
    something.background(255, 0, 0);
    something.fill(255, 255, 0);
    showDisplay(something);
    rotateLayers();
    stepLayerVisibleRect = 0;
    break;
    case 9:// demo visible rect again
    updateLayersVrect(0);
    break;
  	default:
  	step = step_surface = step_layer = stepShift = stepLayerVisibleRect = 0;
  	displayRx = -1.2; displayY = 0;
    layerRx = -1.2;
  	break;
    }
  angle += 0.05;
}
