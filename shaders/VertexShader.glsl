#version 330

// attributes
in vec3 v_position;
in vec3 v_normal;
in vec2 v_texture_coord;

// matrices
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// animation
uniform float Time;

// object control
uniform int u_object_type;      // 0 = static, 1 = trunk, 2 = foliage
uniform float u_bend_strength;
uniform float u_tree_height;

// outputs to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 texcoord;

void main()
{
    vec3 pos = v_position;
    vec4 world_pos = Model * vec4(pos, 1.0);

    // avoid division by zero
    float height = max(u_tree_height, 0.0001);
    float t = clamp(world_pos.y / height, 0.0, 1.0);

    // trunk tapering
    if (u_object_type == 1)
    {
        float scale = 1.0 - t;
        pos.x *= scale;
        pos.z *= scale;
        world_pos = Model * vec4(pos, 1.0);
    }
    // foliage wind sway
    else if (u_object_type == 2)
    {
        float wave = sin(Time * 3.0 + world_pos.x + t);
        float offset = wave * u_bend_strength * t;
        world_pos.x += offset;
    }

    // outputs
    frag_position = vec3(world_pos);
    frag_normal   = normalize(mat3(Model) * v_normal);

    texcoord = vec2(v_texture_coord.x, 1.0 - v_texture_coord.y);

    gl_Position = Projection * View * world_pos;
}
