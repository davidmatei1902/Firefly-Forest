#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Time; 
uniform int u_object_type;      // 1 = trunk, 2 = foliage, 0 = static
uniform float u_bend_strength;  // amplitude
uniform float u_tree_height;

out vec3 frag_position;
out vec3 frag_normal;

void main() {
    vec3 pos = v_position;
    vec4 world_pos = Model * vec4(pos, 1.0); 

    float t = clamp(world_pos.y / u_tree_height, 0.0, 1.0);

    // main deformation logic
    if (u_object_type == 1) // trunk (tapering)
    {
        float scale = 1.0 - t;

        pos.x *= scale;
        pos.z *= scale;
        
        world_pos = Model * vec4(pos, 1.0);
    }
    else if (u_object_type == 2) // foliage (wind)
    {
        float wave = sin(Time * 3.0 + t * 0.5 + world_pos.x);
        float offset = wave * u_bend_strength * t;

        world_pos.x += offset;
    }

    // set final outputs
    frag_position = vec3(world_pos);
    frag_normal = normalize(mat3(Model) * v_normal); 

    gl_Position = Projection * View * world_pos;
}