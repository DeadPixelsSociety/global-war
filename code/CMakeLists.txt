set(GLOBAL_WAR_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/global-war")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

#
# global-war
#

add_executable(global-war
  global-war.cc
  bits/client/ArmySelection.cc
  bits/client/ClientArmy.cc
  bits/client/ClientMap.cc
  bits/client/GameState.cc
  bits/client/Hexagon.cc

  bits/common/Sockets.cc
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
  bits/server/Lobby.cc
  bits/server/Player.cc
  bits/server/ThreadCom.cc

  bits/common/Sockets.cc
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
