
Port Monitor carrier 
====================

- What is the port monitor?

  It is a run-time monitor object implemented as a new carrier which 
  allows to modify incoming (in future also outgoing) data to a port 
  using user script. Currently only Lua is supported by the port monitor.  


- What i need to use port monitor? 

  You need to compile yarp-lua binding and set the 'LUA_CPATH' to find 
  yarp-lua binding library. 


- How it works? 

  You should connect two ports using yarp 'portmonitor' carrier modifier:
  
  $ yarp connect /out /in tcp+recv.portmonitor+script.lua+context.myapp+file.my_lua_script
  
  'script.lua' tells the carrier to create a port monitor object for Lua.  
  'context.myapp' tells the resource finder to load the script from the 'myapp' context. 
  'file.my_lua_script' indicates 'my_lua_script' should be loaded by monitor object. 
  'my_lua_script' is located using standard yarp Resource Finder policy. the postfix 
  (e.g., '.lua') is not necessary.
 
  When data arrive to an input port, the port monitor will call the corresponding 
  handler (i.e., PortMonitor.update) from lua script and passes an instance of 
  connection reader to the handler. Using the connection reader, user script can 
  access the data, modify it and return it to the port monitor object. Beside the 
  'PortMonitor.update' The following handlers can be also impelemented within the 
  global 'PortMonitor' table:

   PortMonitor.create
   ------------------
   This is called when the script is loaded and the port monitor object is created.
   Returning 'false' will avoid calling other functions and stop the monitor object.
   
   PortMonitor.create = function() 
        ... 
        return true     --default 
   end 


   PortMonitor.destroy
   -------------------
   This is called when the monitor object is destroying (e.g., on disconnect)
   
   PortMonitor.destroy = function() 
        ... 
   end 


   PortMonitor.accept
   ------------------
   This is called when a new data arrives to the port. Users can access the data and 
   check whether it should accept or discard it. Returing 'false' will discard delivering 
   data to the port and prohabit calling PortMonitor.update(). 

   PortMonitor.accept = function(reader)
        ... 
        return true     --default
   end

   
   PortMonitor.update
   ------------------
   This will be called if the data is accepted by PortMonitor.update(). User can modify and 
   return it using 'reader' object. 
   an event to into port event record

   PortMonitor.update = function(reader)
        ... 
        return reader 
   end 


   PortMonitor.setparam/getparam
   -----------------------------
   This will be called by the yarp port administrator when users try to reconfigure the monitor
   object using yarp port administrative commands (See ./image_modification/README.txt).  
   
   PortMonitor.setparam = function(param)
        ... 
   end

   PortMonitor.getparam = function()
        ... 
        return param 
   end
   
   PortMonitor.trig
   ----------------
   This will be called when one of the peer connections to the same input port receives data. 
   Please refer to the ./arbitration example for a use case for PortMonitor.trig. 
   
   PortMonitor.trig = function() 
        ... 
   end
  
  Beside the port monitor handlers, there is a set of auxilary functions which is offered by the 
  PortMonitor. These auxilary functions are used with the PortMonitor to arbitrator multiple 
  coonection to the same input port of a module. (See ./arbitration examples) 

    - PortMonitor.setEvent(event)       : set an event to into port event record
    - PortMonitor.unsetEvent(event)     : unset an event to into port event record
    - PortMonitor.setConstraint(rule)   : set the selection rule 
    - PortMonitor.getConstraint()       : get the selection rule


  Port monitor carrier looks for a global table name 'PortMonitor' in the user 
  script and calls its corresponding functions if its implemented. Notice that 
  the PortMonitor is a global table and should not be altered or reassigned. 
   

