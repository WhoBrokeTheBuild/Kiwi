#version 450 core

layout(binding = 0) uniform sampler2D u_Texture;

layout(push_constant) uniform PushConstants {
    vec2 u_Size;
    vec2 u_Offset;
};

layout(location = 0) in vec2 v_TexCoord;

layout(location = 0) out vec4 o_Color;

void main()
{
    vec2 UV = (v_TexCoord - u_Offset) / u_Size;
    if (UV.x < 0.0 || UV.x > 1.0 || UV.y < 0.0 || UV.y > 1.0) {
        o_Color = vec4(0, 0, 0, 1);
    } else {
        o_Color = texture(u_Texture, UV);
    }
}