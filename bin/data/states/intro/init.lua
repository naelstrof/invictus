-- Intro state

function STATE:onInit()
    self.MyIcon = Icon( "invictus" )
    self.MyIcon.pos = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    self.MyIcon.color = Color( 1, 1, 1, 0 )
    self.Check = false
end

function STATE:onExit()
    self.MyIcon:remove()
end

function STATE:onTick( dt )
    self.MyIcon.pos = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    if ( self.Check == false ) then
        self.MyIcon.color = self.MyIcon.color + Color( 0, 0, 0, dt/3 )
        if ( self.MyIcon.color.a >= 1 ) then
            self.Check = true;
        end
    else
        self.MyIcon.color = self.MyIcon.color - Color( 0, 0, 0, dt/3 )
        if ( self.MyIcon.color.a <= 0 ) then
            setState( "game" )
        end
    end
end
