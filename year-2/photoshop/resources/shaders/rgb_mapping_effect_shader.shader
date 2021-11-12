uniform sampler2D texture;
uniform sampler2D back_buffer;

uniform float red[256];
uniform float green[256];
uniform float blue[256];

void main() {
    vec4 layer_texture = texture2D(texture, gl_TexCoord[0].xy);

    vec4 pixel = texture2D(back_buffer, gl_TexCoord[0].xy);
    vec4 new_pixel = pixel;

    new_pixel.x = red[int(pixel.x * 255.0)];
    new_pixel.y = green[int(pixel.y * 255.0)];
    new_pixel.z = blue[int(pixel.z * 255.0)];

    gl_FragColor = pixel * (1.0 - layer_texture.a) + new_pixel * layer_texture.a;
}