#version 120

uniform sampler2D texture;
uniform vec4 color;
uniform vec2 textureSize;

varying vec2 uvCoord;

void main()
{
    // Get the color of the text pixel
    vec4 t = texture2D( texture, uvCoord );
    vec4 textColor = color;
    textColor.a = textColor.a * t.r;

    // Get the color of shadow
    vec4 shadowColor = vec4(1,1,1,1)-textColor;

    // Check how far we are away from other pixels.
    //ivec2 texSize = textureSize( texture, 0 );
    vec2 texSize = textureSize;
    float glow = 0;
    float glowSize = 5/texSize.x; // Glow by 5 pixels
    float glowStep = 1/texSize.x; // Step by 1 pixel
    int count = 0;
    for ( float x = -glowSize; x < glowSize; x += glowStep ) {
        for ( float y = -glowSize; y < glowSize; y += glowStep ) {
            glow += texture2D( texture, uvCoord+vec2( x, y ) ).r;
            count++;
        }
    }
    glow = glow/float(count);

    // Make sure we don't apply the shadow to the actual text.
    glow = glow*( 1-textColor.a );
    shadowColor.a = glow;

    // Now we mix the two together.
    gl_FragColor = textColor*textColor.a;
    gl_FragColor = gl_FragColor + shadowColor*shadowColor.a;
    gl_FragColor.a = ( textColor.a+shadowColor.a ) * color.a;
}
