#version 450 core

layout (location = 0) in vec2 vTextureCoords;

layout (location = 0) out vec4 fragColor;

uniform sampler2D screenTexture;
uniform float mode;

void normal_mode() {
    fragColor = texture(screenTexture, vTextureCoords);
}

void inverse_mode() {
    fragColor = vec4(vec3(1.f - texture(screenTexture, vTextureCoords)), 1.f);
}

void greyscale_mode() {
    fragColor = texture(screenTexture, vTextureCoords);
    float average = 0.2126f * fragColor.r + 0.7152f * fragColor.g + 0.0722f * fragColor.b;
    fragColor = vec4(average, average, average, 1.f);
}

void use_kernel(float kernel[9]) {
    const float screenWidth = 1280.f;
    const float screenHeight = 720.f;
    const float offset_x = 1.f / screenWidth; // Precision of the effect
    const float offset_y = 1.f / screenHeight;

    vec2 offsets[9] = vec2[](
        vec2(-offset_x,  offset_y), // top-left
        vec2( 0.f,    offset_y), // top-center
        vec2( offset_x,  offset_y), // top-right
        vec2(-offset_x,  0.f),   // center-left
        vec2( 0.f,    0.f),   // center-center
        vec2( offset_x,  0.f),   // center-right
        vec2(-offset_x, -offset_y), // bottom-left
        vec2( 0.f,   -offset_y), // bottom-center
        vec2( offset_x, -offset_y)  // bottom-right    
    );

    vec3 color = vec3(0.f);
    for (int i = 0; i < 9; ++i) {
        color += vec3(texture(screenTexture, vTextureCoords.st + offsets[i])) * kernel[i];
    }
    fragColor = vec4(color, 1.f);
}

void blur_mode() {
    float blur_kernel[9] = float[](
        1.f / 16, 2.f / 16, 1.f / 16,
        2.f / 16, 4.f / 16, 2.f / 16,
        1.f / 16, 2.f / 16, 1.f / 16  
    );
    use_kernel(blur_kernel);
}

void edge_mode() {
    float edge_kernel[9] = float[](
        -1.f,  -1.f, -1.f,
        -1.f, 8.f, -1.f,
        -1.f,  -1.f, -1.f  
    );
    use_kernel(edge_kernel);
}

void sharpen_mode() {
    float sharpen_kernel[9] = float[](
        0.f, -1.f, 0.f,
        -1.f, 5.f, -1.f,
        0.f, -1.f, 0.f  
    );
    use_kernel(sharpen_kernel);
}

void night_vision_mode() {
    greyscale_mode();
    fragColor = vec4(fragColor.r, fragColor.g * 3, fragColor.b, 1.f);
}

void emboss_mode() {
    float emboss_kernel[9] = float[](
        -2.f, -1.f, 0.f,
        -1.f, 1.f, 1.f,
        0.f, 1.f, 2.f  
    );
    use_kernel(emboss_kernel);
}

void main() {
    switch (int(mode)) {
        case 0:
            normal_mode();
            break;
        case 1:
            inverse_mode();
            break;
        case 2:
            greyscale_mode();
            break;
        case 3:
            blur_mode();
            break;
        case 4:
            edge_mode();
            break;
        case 5:
            sharpen_mode();
            break;
        case 6:
            night_vision_mode();
            break;
        case 7:
            emboss_mode();
            break;
    }
}
