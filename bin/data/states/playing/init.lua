-- Playing state

function STATE:onInit()
    self.array = {}
end

function STATE:onExit()
end

function STATE:onTick( dt )
    if ( getMouse().isDown( 0 ) ) then
        local new = Icon( "dothan" )
        new.pos = Vector( 0, 0, 20 )
        getScene( "world" ).addNode( new )
        table.insert( self.array, new )
    end
    for index,value in pairs( self.array ) do
        value.pos = value.pos+Vector( math.random() * dt )
    end
end
