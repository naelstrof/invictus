-- Intro state

function STATE:onInit()
    self.MyIcon = Icon( "invictus" )
    self.MyIcon.pos = Vector( getWindowWidth()/2, getWindowHeight(), 0 )
    tween( 3, self.MyIcon, { y=getWindowHeight()/2 }, 'outBounce' )

    self.MyText = Text( "Invictus" )
    self.MyText.pos = Vector( getWindowWidth()/2, getWindowHeight()/2 - 84, 0 )
    self.MyText.color = Color( 1, 1, 1, 0 )
    self.MyText.size = 32
    addTimer( function() tween( 1, self.MyText, { a=1 } ) end, 3 )

    self.MySound = Sound( "scream" )
    self.MySound.pitch = 0.8
    addTimer( function() self.MySound:play() end, 3.5 )

    addTimer( function()
        tween( 1, self.MyText, { a=0 } )
        tween( 1, self.MyIcon, { a=0 } )
    end, 5 )

    addTimer( function()
        setState( "game" )
    end, 6 )
end

function STATE:onExit()
    self.MyText:remove()
    self.MyIcon:remove()
    self.MySound:remove()
end

function STATE:onTick( dt )
    tween.update( dt )
end
