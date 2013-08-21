-- Game state

function STATE:onInit()
    self.window = Icon( "window", 16 )
    self.window.pos = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    self.window.scale = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )

    self.play = Button( "button", 1 )
    self.play:setParent( self.window )
    self.play.scale = Vector( 128, 64, 1 )
    self.play.pos = Vector( 0, 0, 0 )

    self.playText = Text( "Play" )
    self.playText.size = 42
    self.playText:setParent( self.play )
end

function STATE:onExit()
    self.window:remove()
    self.play:remove()
    self.playText:remove()
end

function STATE:onTick( dt )
    self.window.pos = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    self.window.scale = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
end
