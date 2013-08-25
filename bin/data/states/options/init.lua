-- Options state

function STATE:onInit()
    self.entering = true
    addTimer( function() self.entering = false end, 1 )

    self.window = Icon( "window", 16 )
    self.window.pos = Vector( getWindowWidth()+getWindowWidth()/2, getWindowHeight()/2, 0 )
    self.window.scale = Vector( getWindowWidth()/2, getWindowHeight()/2, 0 )
    tween( 0.5, self.window, { x=getWindowWidth()/2 }, "inQuad" )

    self.check = Checkbox( "checkbox", 1 )
    self.check:setParent( self.window )
    self.check.pos = Vector( -getWindowWidth()/4+32, getWindowHeight()/4-32-32, 0 )
    self.check.scale = Vector( 20, 20, 1 )
    self.check.checked = fullscreen
    self.check.onCheck = function()
        fullscreen = true
        updateWindow()
    end
    self.check.onUncheck = function()
        fullscreen = false
        updateWindow()
    end
    self.checkText = Text( "Fullscreen" )
    self.checkText.size = 24
    self.checkText:setParent( self.check )
    self.checkText.pos = Vector( 64, 0, 0 )

    self.noborder = Checkbox( "checkbox", 1 )
    self.noborder:setParent( self.window )
    self.noborder.pos = Vector( -getWindowWidth()/4+32, getWindowHeight()/4-32-32-44, 0 )
    self.noborder.scale = Vector( 20, 20, 1 )
    self.noborder.checked = noBorder;
    self.noborder.onCheck = function()
        noBorder = true
        updateWindow()
    end
    self.noborder.onUncheck = function()
        noBorder = false
        updateWindow()
    end
    self.noborderText = Text( "No Border" )
    self.noborderText.size = 24
    self.noborderText:setParent( self.noborder )
    self.noborderText.pos = Vector( 64, 0, 0 )

    self.resText = Text( "Fullscreen resolution: " )
    self.resText.size = 24
    self.resText:setParent( self.window )
    self.resText.pos = Vector( -getWindowWidth()/4+self.resText.width/2+16, getWindowHeight()/4-84, 0 )
    self.res = Dropdown( "button", 1 )
    self.res:setParent( self.resText )
    self.res.scale = Vector( 64, 32, 1 )
    for k, v in pairs( getWindowModes() ) do
        self.res:addRow( v )
    end
    self.res.pos = Vector( self.res.scale.x/2+self.resText.width/2, 0, 0 )
    self.res.onChange = function( state, index, element )
        videoMode = index+1
        updateWindow()
    end
    self.res.selection = videoMode-1

    self.back = Button( "button", 1 )
    self.back:setParent( self.window )
    self.back.scale = Vector( 64, 32, 1 )
    self.back.pos = Vector( -getWindowWidth()/4+32+16, getWindowHeight()/4-32, 0 )
    self.backText = Text( "Back" )
    self.backText.size = 28
    self.backText:setParent( self.back )

    self.exiting = false

    self.back.onRelease = function()
        self.exiting = true
        tween( 0.5, self.window, { x=getWindowWidth()+getWindowWidth()/2 }, "inQuad" )
        addTimer( function() setState( "game" ) end, 0.5 )
    end

end

function STATE:onExit()
    self.window:remove()
    self.back:remove()
    self.res:remove()
    self.check:remove()
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
        self.back.pos = Vector( -x/2+32+16, y/2-32, 0 )
        self.check.pos = Vector( -x/2+32, y/2-32-32, 0 )
        self.noborder.pos = Vector( -x/2+32, y/2-32-32-44, 0 )
        self.resText.pos = Vector( -x/2+self.resText.width/2+16, y/2-84, 0 )
        self.res.pos = Vector( self.res.scale.x/2+self.resText.width/2, 0, 0 )
        self.window.scale = Vector( x, y, 0 )
    end
end
