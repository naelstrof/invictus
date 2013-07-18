print( "Lua powered config: data/config/init.lua" );

-- Set whether or not to be fullscreen.
-- Will attempt to use the video mode specified by the variables `width` and `height`.
-- If the specified video mode is not supported by your monitor it will use your desktop resolution.
fullscreen = false

-- Only used in fullscreen mode. It removes window decorations from the client.
noBorder = false

-- Sets the width and height of the window in windowed mode or fullscreen.
-- In windowed mode these numbers can be anything, but in fullscreen mode the width and height must match one of the available video modes printed to console at run. Otherwise it will use desktop resolution.
width = 512
height = 512

-- Sets the name of the window on boot, can be anything you want.
windowName = "Astrostruct"

-- Sets the maximum framerate of the application, setting it to 0 will disable.
-- Please note that there's no reason to run your computer at 100% at all times, I highly recommend you set this to a reasonable number.
maxFPS = 300

-- Disables or enables vertical sync. It keeps your screen from tearing when there's lots of stuff going on in the game, but it can lock your fps to a low number!
verticalSync = false

-- Set starting sizes of texture atlases which are currently just used for font rendering.
-- Provides best performance as a large power of 2.
-- This should only be changed if you have stuttering from too many texture atlas resizes or if you really like filling your GPU memory.
-- Safe values are 256, 512, and 1024.
textureAtlasStartSize = 512;
