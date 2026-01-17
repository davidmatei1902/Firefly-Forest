#version 330

// input from vertex shader
in vec3 frag_position;

// uniforms
uniform vec3 object_color;

// fog variables
uniform int u_render_fog;
uniform float fog_max_dist;
uniform vec3 fog_color;
uniform vec3 eye_position;

layout(location = 0) out vec4 out_color;

void main() {
    // base object color
    vec4 color_final = vec4(object_color, 1.0);

    // apply fog
    if (u_render_fog == 1) {
        float dist = distance(eye_position, frag_position);
        
        // calculate fog factor (0.0 = near, 1.0 = far)
        float fog_factor = clamp(dist / fog_max_dist, 0.0, 1.0);
        
        // mix object color with fog color
        color_final = mix(color_final, vec4(fog_color, 1.0), fog_factor);
    }

    out_color = color_final;
}