#pragma once

#include "math.hpp"
#include "arena_allocator.hpp"
//#include "common.hpp"

struct Entity
{
    unsigned int id;
};

struct TransformComponent
{
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    unsigned int entity_handle;
};

struct StaticSpriteComponent
{
    Vector2 sprite_offset;
    unsigned int entity_handle;
    unsigned int texture_id;
};

struct AnimatedSpriteComponent
{
    Vector2 sprite_offset;
    float animation_speed;
    float frame_accumilator;
    unsigned int texture_id;
    unsigned int entity_handle;
    int frame_count;
    int frame;
};

struct ScriptComponent
{
    void(*on_start)(unsigned int entity_handle, void* void_game_state);
    void(*on_update)();
    unsigned int entity_handle;
};

typedef void on_start_t(unsigned int entity_handle, void* void_game_state);
typedef void on_update_t();

struct CameraComponent
{
    Matrix4x4 view_matrix;
    Matrix4x4 projection_matrix;
    Vector3 position;
    unsigned int entity_handle;
    bool is_active;
};

struct Tile
{
    int index;
    bool is_visible;
};

struct TilemapComponent
{
    Tile* tilemap;
    size_t capacity;
    size_t chunck_size;
    unsigned int texture_id;
    unsigned int entity_handle;
};

struct ECSData
{
    Entity* entities;
    TransformComponent* transforms;
    StaticSpriteComponent* static_sprites;
    AnimatedSpriteComponent* animated_sprites;
    ScriptComponent* scripts;
    CameraComponent* cameras;
    TilemapComponent* tilemaps;

    size_t entity_size;
    size_t transform_size;
    size_t static_sprite_size;
    size_t animated_sprite_size;
    size_t script_size;
    size_t camera_size;
    size_t tilemap_size;

    unsigned int entity_handle_counter;
};

const ECSData* get_ecs_data();
void initialize_ecs(Arena* persistance_storage, ECSData* ecs_data);
Entity create_entity();
TransformComponent* add_transform_component(unsigned int entity_handle);
TransformComponent* get_transform_component(unsigned int entity_handle);
StaticSpriteComponent* add_static_sprite_component(unsigned int entity_handle);
StaticSpriteComponent* get_static_sprite_component(unsigned int entity_handle);
AnimatedSpriteComponent* add_animated_sprite_component(unsigned int entity_handle);
AnimatedSpriteComponent* get_animated_sprite_component(unsigned int entity_handle);
ScriptComponent* add_script_component(unsigned int entity_handle);
ScriptComponent* get_script_component(unsigned int entity_handle);
CameraComponent* add_camera_component(unsigned int entity_handle);
CameraComponent* get_camera_component(unsigned int entity_handle);
CameraComponent* get_active_camera_component();
TilemapComponent* add_tilemap_component(unsigned int entity_handle);
TilemapComponent* get_tilemap_component(unsigned int entity_handle);