BtClass = function(ctor)
    local prototype = {}
    prototype.__index = prototype
    prototype.ctor = ctor

    setmetatable(prototype, {
        __call = function(...)
            local obj = {}
            setmetatable(obj, prototype)

            if prototype.ctor then
                prototype.ctor(obj, ...)
            end
            return obj
        end
    })

    return prototype
end


