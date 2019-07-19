let video;
let image;
let angle = 0;

function setup() {
  video = createVideo(['assets/mcu-wave.mp4']);
  video.elt.muted = true;
  video.loop();
  video.hide();
  image = loadImage('assets/background.jpg')
  createCanvas(800, 600, WEBGL);
}

function drawBoxWithVideo(angle){
  texture(video);
  box(200, 200, 200);
}

function drawBoxWithImage(angle){
  texture(image);
  box(200, 200, 200);
}

function drawCustomShap(angle){
  background('rgb(100, 100, 100)');

  fill(0)
  //texture(image);
  beginShape();
  vertex(0, 0, 50, 0, 0);
  vertex(100, 0, 50, 1, 0);
  vertex(100, 100, 50, 1, 1);
  vertex(0, 100, 50, 0, 1);
  endShape(CLOSE);
}

function draw() {
  rotateZ(0);
  rotateX(0.5);
  rotateY(angle * 0.1);

  //drawBoxWithImage(angle);
  drawBoxWithVideo(angle);
  //drawCustomShap(angle);
  angle += 0.05;
}
