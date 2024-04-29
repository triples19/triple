#ifdef VERTEX

layout (location = 0) in vec2 vVertex;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexCoords;

out vec2 fTexCoords;
out vec4 fColor;

void main()
{
    fTexCoords = vTexCoords;
    fColor = vColor;
    gl_Position = vec4(vVertex, 0.0, 1.0);
}

#endif

#ifdef FRAGMENT

in vec2 fTexCoords;
in vec4 fColor;
out vec4 oColor;

uniform sampler2D image;

void main()
{
    oColor = fColor * texture(image, fTexCoords);
}

#endif