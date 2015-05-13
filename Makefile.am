
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = hbx

AUTOMAKE_OPTIONS = subdir-objects

hbx_SOURCES =  src/Client.cpp src/connection.cpp src/main.cpp src/funcs.cpp \
                                        src/Gate.cpp src/GServer.cpp src/Inventory.cpp src/InventoryMgr.cpp src/Item.cpp \
                                        src/LServer.cpp src/Magic.cpp src/Map.cpp src/Npc.cpp \
                                        src/Party.cpp src/PartyMgr.cpp src/request_handler.cpp \
                                        src/Server.cpp src/Tile.cpp src/Unit.cpp src/Misc.cpp \
                                        src/chat_handler.cpp src/DelayEvent.cpp src/Skill.cpp

hbx_CPPFLAGS = -std=gnu++11 -Wno-write-strings -Wno-narrowing -Wno-conversion-null
hbx_LDADD = -llua5.2 -lPocoUtil -lPocoNet -lPocoDataMySQL -lPocoDataSQLite -lPocoData -lPocoFoundation -lmysqlclient -lboost_chrono -lboost_date_time -lboost_system -lboost_thread -lpthread




