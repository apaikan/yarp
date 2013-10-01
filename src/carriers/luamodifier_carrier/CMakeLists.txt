# Copyright (C) 2012 IITRBCS
# Authors: Ali Paikan and Paul Fitzpatrick
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

if (COMPILE_PLUGIN_LIBRARY)
  yarp_prepare_carrier(luamodifier_carrier TYPE LuaModifier INCLUDE LuaModifier.h)
  yarp_add_carrier_fingerprint(luamodifier.ini luamodifier_carrier)
endif (COMPILE_PLUGIN_LIBRARY)

if (NOT SKIP_luamodifier_carrier)

  find_package(YARP REQUIRED)
  include_directories(${YARP_INCLUDE_DIRS})

  yarp_add_plugin(yarp_luamodifier LuaModifier.h LuaModifier.cpp)

  if(CREATE_LIB_MATH)
    ADD_DEFINITIONS( -DWITH_YARPMATH )
    target_link_libraries(yarp_luamodifier YARP_OS YARP_sig YARP_math)
  else(CREATE_LIB_MATH)
     target_link_libraries(yarp_luamodifier YARP_OS YARP_sig)
  endif(CREATE_LIB_MATH)

  target_link_libraries(yarp_luamodifier ${ACE_LIBRARIES})

endif ()

