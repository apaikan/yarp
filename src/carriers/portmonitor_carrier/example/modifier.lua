-- call a C++ function
require("yarp")



function luamodifier_create()
    print("in luamodifier_create")
    return true;
end


function luamodifier_configure() 

    return true;
end


function luamodifier_trig(bt)

    --[[
    print(type(bt))
    mt = getmetatable(bt)
    print(tostring(bt))
    for k,v in pairs(mt) do
        print("Global key", k, "value", v)
    end
    --]]
    --
    print(bt:get(0):asInt())
    print(bt:get(1):asString())

    bt:addString("modified from Lua")
    bt:addInt(45)
    return bt
end


function luamodifier_destroy()

    return true
end


function luamodifier_error(error_code) 

    return true
end

