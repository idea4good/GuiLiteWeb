let vid;
let angle = 0;

function preload(){
  vid = createVideo(['assets/test.mp4']);
}

function setup() {
  createCanvas(800, 600, WEBGL);  
  vid.elt.muted = true;
  vid.loop();
  vid.hide();
}

function drawCube(angle){
  push();
  //rotateZ(angle * 0.1);
  rotateX(0.5);
  rotateY(angle * 0.1);
  texture(vid);
  box(200, 200, 200);
  pop();
}

function draw() {
  background('rgb(100, 100, 100)');
  drawCube(angle);
  angle += 0.05;
}
