-- Game state

function STATE:onInit()
    self.MyText = Text( "And this is the game state!" )
    self.MyText.color = Color( 1, 0, 0, 1 )
    self.MyText.pos = Vector( getWindowWidth()/2, getWindowHeight()-128, 0 )
end

function STATE:onExit()
    self.MyText:remove()
end

function STATE:onTick( dt )
    self.MyText.pos = self.MyText.pos - Vector( 0, dt*32, 0 )
    self.MyText.ang = self.MyText.ang + Vector( 0, 0, dt )
end
