//#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 LocalPos;
void main()
{
    LocalPos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
//#shader fragment
#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
in vec3 LocalPos;
uniform vec3 bulbColor;
void main()
{
    vec2 xz = LocalPos.xz;
    float maxExtent = max(abs(xz.x), abs(xz.y));
    
    vec2 normalized = xz / 0.04;
    float edge = dot(normalized, normalized);
    if (edge > 1.0)
    discard;

    FragColor = vec4(bulbColor, 1.0);
    float brightness = dot(bulbColor, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
    BrightColor = vec4(bulbColor, 1.0);
    else
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}