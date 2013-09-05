-- Game state

function STATE:onInit()
    self.window = Icon( "window", 16 )
    self.window.pos = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    self.window.scale = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    self.window.color = Color( 1, 1, 1, 0 )
    tween( 0.5, self.window, { a=1 } )

    self.play = Button( "button", 1 )
    self.play:setParent( self.window )
    self.play.scale = Vector( 128, 64, 1 )
    self.play.pos = Vector( 0, 32, 0 )
    self.play.color = Color( 1, 1, 1, 0 )
    self.playText = Text( "Play" )
    self.playText.color = Color( 1, 1, 1, 0 )
    self.playText.size = 42
    self.playText:setParent( self.play )
    addTimer( function() tween( 0.5, self.play, { a=1 } ) end, 0.5 )
    addTimer( function() tween( 0.5, self.playText, { a=1 } ) end, 0.5 )

    self.options = Button( "button", 1 )
    self.options:setParent( self.window )
    self.options.scale = Vector( 128, 64, 1 )
    self.options.pos = Vector( 0, -32, 0 )
    self.options.color = Color( 1, 1, 1, 0 )
    self.optionsText = Text( "Options" )
    self.optionsText.color = Color( 1, 1, 1, 0 )
    self.optionsText.size = 42
    self.optionsText:setParent( self.options )
    addTimer( function() tween( 0.5, self.options, { a=1 } ) end, 0.5 )
    addTimer( function() tween( 0.5, self.optionsText, { a=1 } ) end, 0.5 )

    self.exiting = false

    self.options.onRelease = function()
        self.exiting = true
        tween( 0.5, self.window, { x=-getWindowWidth()/2 }, "inQuad" )
        addTimer( function() setState( "options" ) end, 0.5 )
    end

    self.options.onPress = function()
        self.exiting = true
        tween( 0.5, self.window, { x=-getWindowWidth()/2 }, "inQuad" )
        addTimer( function() setState( "playing" ) end, 0.5 )
    end

end

function STATE:onExit()
    self.window:remove()
    self.play:remove()
    self.playText:remove()
    self.options:remove()
    self.optionsText:remove()
end

function STATE:onTick( dt )
    tween.update( dt )
    if ( self.exiting == false ) then
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
