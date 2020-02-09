require("boot/class")

Game = BtClass(function(self)

    -- event callbacks:
    self.on_app_init = function(self)
    end

    self.on_app_uninit = function(self)
    end

    self.on_tick = function(self)
    end

end)

game = Game()
