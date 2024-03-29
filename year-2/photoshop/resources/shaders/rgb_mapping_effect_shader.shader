uniform sampler2D primary_texture;
uniform sampler2D preview_texture;
uniform sampler2D back_buffer;

uniform float red[256];
uniform float green[256];
uniform float blue[256];

void main() {
    vec4 primary_texture = texture2D(primary_texture, gl_TexCoord[0].xy);
    vec4 preview_texture = texture2D(preview_texture, gl_TexCoord[0].xy);

    vec4 layer_texture;

    layer_texture.rgb = primary_texture.a * primary_texture.rgb + (1.0 - primary_texture.a) * preview_texture.rgb;
    layer_texture.a = primary_texture.a + preview_texture.a * (1.0 - primary_texture.a);

    vec4 pixel = texture2D(back_buffer, gl_TexCoord[0].xy);
    vec4 new_pixel = pixel;

    new_pixel.x = red[int(pixel.x * 255.0)];
    new_pixel.y = green[int(pixel.y * 255.0)];
    new_pixel.z = blue[int(pixel.z * 255.0)];

    gl_FragColor.rgb = pixel.rgb * (1.0 - layer_texture.rgb * layer_texture.a) + new_pixel.rgb * layer_texture.rgb * layer_texture.a;
    gl_FragColor.a = pixel.a;
}