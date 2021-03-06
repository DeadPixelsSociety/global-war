set(GLOBAL_WAR_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/global-war")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

#
# global-war
#

add_executable(global-war
  global-war.cc
  bits/client/ArmySelection.cc
  bits/client/GameScene.cc
  bits/client/GameState.cc
  bits/client/HUD.cc
  bits/client/LobbyScene.cc
  bits/client/MapRender.cc
  bits/client/NetworkManagerClient.cc
  bits/client/RegimentRender.cc
  bits/client/Singletons.cc
  bits/client/Stage.cc
  bits/client/WaitScreen.cc

  bits/common/Data.cc
  bits/common/Hexagon.cc
  bits/common/MapData.cc
  bits/common/Sockets.cc
  # bits/common/ThreadCom.cc
)

target_include_directories(global-war
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(global-war
  gf::gf0
  ${Boost_LIBRARIES}
)

install(
  TARGETS global-war
  RUNTIME DESTINATION games
)

#
# global-war-server
#

add_executable(global-war-server
  global-war-server.cc
  bits/server/GameSession.cc
  bits/server/GameState.cc
  bits/server/Lobby.cc
  bits/server/NetworkManagerServer.cc
  # bits/server/Player.cc
  bits/server/Singletons.cc
  bits/server/Utils.cc

  bits/common/Data.cc
  bits/common/Hexagon.cc
  bits/common/MapData.cc
  bits/common/Sockets.cc
  # bits/common/ThreadCom.cc
)

target_include_directories(global-war-server
  PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(global-war-server
  gf::gf0
  ${Boost_LIBRARIES}
)

install(
  TARGETS global-war-server
  RUNTIME DESTINATION games
)
