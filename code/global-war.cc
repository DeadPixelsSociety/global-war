#include <iostream>

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/ResourceManager.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

#include <boost/asio.hpp>

#include "bits/common/Packet.h"
#include "bits/client/ArmySelection.h"
#include "bits/client/ClientArmy.h"
#include "bits/client/ClientMap.h"

#include "config.h"

using boost::asio::ip::tcp;

namespace {

  constexpr int MaxLength = 1024;

  gf::Id playerID = gw::InvalidPlayerID;

  void ping(tcp::socket& socket) {
    gw::Packet packet;
    packet.type = gw::PacketType::Ping;
    packet.ping.sequence = 42;

    uint8_t data[MaxLength];

    gf::MemoryOutputStream stream(data);
    gf::Serializer ar(stream);

    ar | packet;

    boost::asio::write(socket, boost::asio::buffer(data, MaxLength));
  }

  void quickMacth(tcp::socket& socket) {
    gw::Packet packet;
    packet.type = gw::PacketType::QuickMatch;
    packet.quickMatch.playerID = playerID;

    uint8_t data[MaxLength];

    gf::MemoryOutputStream stream(data);
    gf::Serializer ar(stream);

    ar | packet;

    boost::asio::write(socket, boost::asio::buffer(data, MaxLength));
  }

  gw::Packet newPlayer(tcp::socket &socket) {
    uint8_t data[MaxLength];

    boost::system::error_code error;

    size_t length = socket.read_some(boost::asio::buffer(data), error);

    if (error) {
      throw boost::system::system_error(error); // Some other error.
    }

    gf::MemoryInputStream stream(gf::ArrayRef<uint8_t>(data, length));
    gf::Deserializer ar(stream);

    gw::Packet packet;
    ar | packet;

    assert(packet.type == gw::PacketType::NewPlayer);

    return packet;
  }

}

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: global-war <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::socket socket(io_service);
    tcp::resolver resolver(io_service);
    boost::asio::connect(socket, resolver.resolve({ argv[1], argv[2] }));

    // Receive ID
    auto packet = newPlayer(socket);
    ::playerID = packet.newPlayer.playerID;
    gf::Log::info("Player ID: %lx\n", ::playerID);

    quickMacth(socket);

    static constexpr gf::Vector2u ScreenSize(1024, 576);
    static constexpr gf::Vector2f ViewSize(1000.0f, 1000.0f); // dummy values
    static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f); // dummy values

    // initialization

    gf::Window window("Global War", ScreenSize);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    gf::RenderWindow renderer(window);

    gf::ResourceManager resources;
    resources.addSearchDir(GLOBAL_WAR_DATA_DIR);

    // views

    gf::ViewContainer views;

    gf::ExtendView mainView(ViewCenter, ViewSize);
    views.addView(mainView);

    gf::ScreenView hudView;
    views.addView(hudView);

    views.setInitialScreenSize(ScreenSize);

    gf::ZoomingViewAdaptor adaptor(renderer, mainView);

    // actions

    gf::ActionContainer actions;

    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);

    gf::Action fullscreenAction("Fullscreen");
    fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    actions.addAction(fullscreenAction);

    gf::Action leftAction("Left");
    leftAction.addScancodeKeyControl(gf::Scancode::A);
    leftAction.addScancodeKeyControl(gf::Scancode::Left);
    leftAction.setContinuous();
    actions.addAction(leftAction);

    gf::Action rightAction("Right");
    rightAction.addScancodeKeyControl(gf::Scancode::D);
    rightAction.addScancodeKeyControl(gf::Scancode::Right);
    rightAction.setContinuous();
    actions.addAction(rightAction);

    gf::Action upAction("Up");
    upAction.addScancodeKeyControl(gf::Scancode::W);
    upAction.addScancodeKeyControl(gf::Scancode::Up);
    upAction.setContinuous();
    actions.addAction(upAction);

    gf::Action downAction("Down");
    downAction.addScancodeKeyControl(gf::Scancode::S);
    downAction.addScancodeKeyControl(gf::Scancode::Down);
    downAction.setContinuous();
    actions.addAction(downAction);

    gf::Action pingAction("Ping");
    pingAction.addScancodeKeyControl(gf::Scancode::P);
    actions.addAction(pingAction);

    // entities

    gf::EntityContainer mainEntities;
    // add entities to mainEntities
    gw::ClientMap map(resources.getAbsolutePath("map.txt"));
    mainEntities.addEntity(map);

    gw::ClientArmy army(resources);
    mainEntities.addEntity(army);

    gf::EntityContainer hudEntities;
    // add entities to hudEntities


    // state machines;
    gw::ArmySelection armySelection(map, army, renderer, mainView);

    // game loop

    renderer.clear(gf::Color::White);

    gf::Clock clock;

    while (window.isOpen()) {
      // 1. input

      gf::Event event;

      while (window.pollEvent(event)) {
        actions.processEvent(event);
        views.processEvent(event);
        adaptor.processEvent(event);
        armySelection.processEvent(event);
      }

      if (closeWindowAction.isActive()) {
        window.close();
      }

      if (fullscreenAction.isActive()) {
        window.toggleFullscreen();
      }

      if (rightAction.isActive()) {
        // do something
      } else if (leftAction.isActive()) {
        // do something
      } else if (upAction.isActive()) {
        // do something
      } else if (downAction.isActive()) {
        // do something
      } else {
        // do something
      }


      if (pingAction.isActive()) {
        ping(socket);
      }

      // 2. update

      gf::Time time = clock.restart();
      mainEntities.update(time);
      hudEntities.update(time);


      // 3. draw

      renderer.clear();

      renderer.setView(mainView);
      mainEntities.render(renderer);

      renderer.setView(hudView);
      hudEntities.render(renderer);

      renderer.display();

      actions.reset();
    }

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
