#include "demo.hxx"
#include "debug.hxx"
#include <cmath>

float const Demo::BPM = 174.f;
int const Demo::RPB = 8;
double const Demo::ROW_RATE = (double(BPM) / 60) * RPB;

#ifndef SYNC_PLAYER
static void pause(void* d, int flag) {
  Player *player = (Player*)d;
  if (flag) {
    player->pause();
  } else {
    player->play();
  }
}

static void setRow(void *d, int row) {
  Player *player = (Player*)d;
  player->setTime(row/Demo::ROW_RATE);
}

static int isPlaying(void *d) {
  Player *player = (Player*)d;
  return player->isPlaying();
}

static struct sync_cb playerControls = {
  pause, setRow, isPlaying
};
#endif

Demo::Demo(Window &window):
  time(0.f),
  fftBass(0.f),
  fftTreble(0.f),
  window(window),
  width(window.getWidth()),
  height(window.getHeight()),
  player("music.ogg", &window),
  projectionMatrix(glm::perspective(glm::radians(45.0f),
        (float)(width) / (float)(height), 0.1f, 100.f))
{
  rocket = sync_create_device("sync");

#ifndef SYNC_PLAYER
  if (sync_tcp_connect(rocket, "localhost", SYNC_DEFAULT_PORT)) {
    die("Sync failed to connect.");
  }
#endif

  player.play();
}

Demo::~Demo() {
#ifndef SYNC_PLAYER
  sync_save_tracks(rocket);
#endif
  sync_destroy_device(rocket);
}

Player const &Demo::getPlayer() const {
  return player;
}

Shaders const &Demo::getShaders() const {
  return shaders;
}

Verts const &Demo::getVerts() const {
  return verts;
}

void Demo::update() {
  time = player.getTime();
  fftBass = player.getFftBass();
  fftTreble = player.getFftTreble();
#ifndef SYNC_PLAYER
  if (sync_update(rocket, (int)std::floor(time*ROW_RATE),
        &playerControls, (void*)&player)) {
    sync_tcp_connect(rocket, "localhost", SYNC_DEFAULT_PORT);
  }
#endif
}

sync_track const *Demo::getRocketTrack(std::string const &name) const {
  return sync_get_track(rocket, name.c_str());
}

float Demo::getValue(sync_track const *track) const {
  return sync_get_val(track, time*ROW_RATE);
}

float Demo::getTime() const {
  return time;
}

float Demo::getFftBass() const {
  return fftBass;
}

float Demo::getFftTreble() const {
  return fftTreble;
}

int Demo::getWidth() const {
  return width;
}

int Demo::getHeight() const {
  return height;
}

glm::mat4 const &Demo::getProjectionMatrix() const {
  return projectionMatrix;
}

