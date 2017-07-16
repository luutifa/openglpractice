#pragma once

#include "window.hxx"
#include "player.hxx"
#include "shaders.hxx"
#include "../rocket/lib/sync.h"
#include <map>

class Demo {
  public:
    Demo(Window &window);
    ~Demo();
    Player const &getPlayer() const;
    Shaders const &getShaders() const;
    void updateRocket();
    sync_track const *getRocketTrack(std::string const &name) const;
    float const getValue(sync_track const *track) const;

    static float const BPM;
    static int const RPB;
    static double const ROW_RATE;

  private:
    Window &window;
    Shaders shaders; // Automatically compile shader storage class
    Player player;
    sync_device *rocket;
    
};
