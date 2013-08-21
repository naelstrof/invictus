-- Options state

function STATE:onInit()
    self.entering = true
    addTimer( function() self.entering = false end, 1 )

    self.window = Icon( "window", 16 )
    self.window.pos = Vector( getWindowWidth()+getWindowWidth()/2, getWindowHeight()/2, 0 )
    self.window.scale = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    tween( 1, self.window, { x=getWindowWidth()/2 }, "inQuad" )

    self.back = Button( "button", 1 )
    self.back:setParent( self.window )
    self.back.scale = Vector( 128, 64, 1 )
    self.back.pos = Vector( 0, -32, 0 )
    self.backText = Text( "Back" )
    self.backText.size = 42
    self.backText:setParent( self.back )

    self.exiting = false

    self.back.onRelease = function()
        self.exiting = true
        tween( 1, self.window, { x=getWindowWidth()+getWindowWidth()/2 }, "inQuad" )
        addTimer( function() setState( "game" ) end, 1 )
    end

end

function STATE:onExit()
    self.window:remove()
    self.back:remove()
    self.backText:remove()
end

function STATE:onTick( dt )
    tween.update( dt )
    if ( self.exiting == false and self.entering == false ) then
        self.window.pos = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
        local x = 128+32
        local y = 128+32
        if ( getWindowWidth()/2 > x ) then
            x = getWindowWidth()/2
        end
        if ( getWindowHeight()/2 > y ) then
            y = getWindowHeight()/2
        end
        self.window.scale = Vector( x, y, 0 )
    end
end
