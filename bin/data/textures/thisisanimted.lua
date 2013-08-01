local idle = Animation(   "idle",
                          "data/textures/thisisanimated/thisis.png",
                          "data/textures/thisisanimated/animated.png")
idle.fps = 5
idle.loop = true

addTexture( "thisisanimated", idle )
