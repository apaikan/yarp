-- call a C++ function
require("yarp")


PortMonitor = {}

--
-- this function is called when the port monitor is created \
--
-- alternatively :
-- function PortMonitor.create() ... end 

PortMonitor.create = function()
    print("in create!")
    return true;
end


-- 
-- this function is called when monitor object is destroyed
--
PortMonitor.destroy = function()
    print("in destroy!")
    return
end


--
-- update is called every time the port receives new data
-- reader is of type 'ConnectionReader'
PortMonitor.update = function(reader)
    
    bt = yarp.Bottle()
    bt:read(reader)
    print("READ:", bt:toString())
    return reader
end

--
-- setparam is called on setCarrierParams by the port administrator  
-- param is of type 'Property'
PortMonitor.setparam = function(param) 

    return
end

--
-- getparan is called on getCarrierParams by the port administrator
-- param should be of type 'Property'
PortMonitor.getparam = function() 

    return param
end

--
-- not implemented yet
--
--[[
PortMonitor.error = function(error_code) 

    return true
end
]]--
