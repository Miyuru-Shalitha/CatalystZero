#include "ecs.hpp"

#include "log.hpp"

constexpr size_t MAX_EC_COUNT = 1000;

static ECSData* g_ecs_data = nullptr;

const ECSData* get_ecs_data()
{
    CZ_ASSERT(g_ecs_data != nullptr, "g_ecs_data is not assigned");
    return g_ecs_data;
}

static void reset_ecs()
{
    for (size_t i = 0; i < g_ecs_data->entity_size; i++)
    {
        g_ecs_data->entities[i] = {};
    }

    g_ecs_data->entity_handle_counter = -1;
    g_ecs_data->entity_size = 0;

    for (size_t i = 0; i < g_ecs_data->transform_size; i++)
    {
        g_ecs_data->transforms[i] = {};
    }

    g_ecs_data->transform_size = 0;

    for (size_t i = 0; i < g_ecs_data->static_sprite_size; i++)
    {
        g_ecs_data->static_sprites[i] = {};
    }

    g_ecs_data->static_sprite_size = 0;

    for (size_t i = 0; i < g_ecs_data->animated_sprite_size; i++)
    {
        g_ecs_data->animated_sprites[i] = {};
    }

    g_ecs_data->animated_sprite_size = 0;
}

void initialize_ecs(Arena* persistance_storage, ECSData* ecs_data)
{
    g_ecs_data = ecs_data;
    g_ecs_data->entity_handle_counter = -1;
    g_ecs_data->entities = (Entity*)arena_alloc(persistance_storage, sizeof(Entity) * MAX_EC_COUNT);
    g_ecs_data->transforms = (TransformComponent*)arena_alloc(persistance_storage, sizeof(TransformComponent) * MAX_EC_COUNT);
    g_ecs_data->static_sprites = (StaticSpriteComponent*)arena_alloc(persistance_storage, sizeof(StaticSpriteComponent) * MAX_EC_COUNT);
    g_ecs_data->animated_sprites = (AnimatedSpriteComponent*)arena_alloc(persistance_storage, sizeof(AnimatedSpriteComponent) * MAX_EC_COUNT);
    g_ecs_data->scripts = (ScriptComponent*)arena_alloc(persistance_storage, sizeof(ScriptComponent) * MAX_EC_COUNT);
    g_ecs_data->cameras = (CameraComponent*)arena_alloc(persistance_storage, sizeof(CameraComponent) * MAX_EC_COUNT);
    g_ecs_data->tilemaps = (TilemapComponent*)arena_alloc(persistance_storage, sizeof(TilemapComponent) * MAX_EC_COUNT);
}

Entity* create_entity()
{
    Entity entity = {};
    entity.id = ++g_ecs_data->entity_handle_counter;
    entity.parent_entity_handle = -1;
    g_ecs_data->entities[g_ecs_data->entity_size] = entity;

    return &g_ecs_data->entities[g_ecs_data->entity_size++];
}

Entity* get_entity(unsigned int entity_handle)
{
    for (size_t i = 0; i < g_ecs_data->entity_size; i++)
    {
        if (g_ecs_data->entities[i].id == entity_handle)
        {
            return &g_ecs_data->entities[i];
        }
    }

    return nullptr;
}

TransformComponent* add_transform_component(unsigned int entity_handle)
{
    TransformComponent transform = {};
    transform.entity_handle = entity_handle;
    g_ecs_data->transforms[g_ecs_data->transform_size] = transform;
    
    return &g_ecs_data->transforms[g_ecs_data->transform_size++];    
}

TransformComponent* get_transform_component(unsigned int entity_handle)
{
    for (size_t i = 0; i < g_ecs_data->transform_size; i++)
    {
        if (g_ecs_data->transforms[i].entity_handle == entity_handle)
        {
            return &g_ecs_data->transforms[i];
        }
    }

    CZ_ASSERT(false, "Component does not exists");
    return nullptr;
}

StaticSpriteComponent* add_static_sprite_component(unsigned int entity_handle)
{
    StaticSpriteComponent static_sprite = {};
    static_sprite.entity_handle = entity_handle;
    g_ecs_data->static_sprites[g_ecs_data->static_sprite_size] = static_sprite;

    return &g_ecs_data->static_sprites[g_ecs_data->static_sprite_size++];
}

StaticSpriteComponent* get_static_sprite_component(unsigned int entity_handle)
{
    for (size_t i = 0; i < g_ecs_data->static_sprite_size; i++)
    {
        if (g_ecs_data->static_sprites[i].entity_handle == entity_handle)
        {
            return &g_ecs_data->static_sprites[i];
        }
    }

    //CZ_ASSERT(false, "Component does not exists");
    return nullptr;
}

AnimatedSpriteComponent* add_animated_sprite_component(unsigned int entity_handle)
{
    AnimatedSpriteComponent animated_sprite = {};
    animated_sprite.entity_handle = entity_handle;
    g_ecs_data->animated_sprites[g_ecs_data->animated_sprite_size] = animated_sprite;

    return &g_ecs_data->animated_sprites[g_ecs_data->animated_sprite_size++];
}

AnimatedSpriteComponent* get_animated_sprite_component(unsigned int entity_handle)
{
    for (size_t i = 0; i < g_ecs_data->animated_sprite_size; i++)
    {
        if (g_ecs_data->animated_sprites[i].entity_handle == entity_handle)
        {
            return &g_ecs_data->animated_sprites[i];
        }
    }

    CZ_ASSERT(false, "Component does not exists");
    return nullptr;
}

ScriptComponent* add_script_component(unsigned int entity_handle)
{
    ScriptComponent script = {};
    script.entity_handle = entity_handle;
    script.on_start = nullptr;
    script.on_update = nullptr;
    g_ecs_data->scripts[g_ecs_data->script_size] = script;

    return &g_ecs_data->scripts[g_ecs_data->script_size++];
}

ScriptComponent* get_script_component(unsigned int entity_handle)
{
    for (size_t i = 0; i < g_ecs_data->script_size; i++)
    {
        if (g_ecs_data->scripts[i].entity_handle == entity_handle)
        {
            return &g_ecs_data->scripts[i];
        }
    }

    CZ_ASSERT(false, "Component does not exists");
    return nullptr;
}

CameraComponent* add_camera_component(unsigned int entity_handle)
{
    CameraComponent camera = {};
    camera.entity_handle = entity_handle;
    g_ecs_data->cameras[g_ecs_data->camera_size] = camera;

    return &g_ecs_data->cameras[g_ecs_data->camera_size++];
}

CameraComponent* get_camera_component(unsigned int entity_handle)
{
    for (size_t i = 0; i < g_ecs_data->camera_size; i++)
    {
        if (g_ecs_data->cameras[i].entity_handle == entity_handle)
        {
            return &g_ecs_data->cameras[i];
        }
    }

    CZ_ASSERT(false, "Component does not exists");
    return nullptr;
}

CameraComponent* get_active_camera_component()
{
    for (size_t i = 0; i < g_ecs_data->camera_size; i++)
    {
        if (g_ecs_data->cameras[i].is_active)
        {
            return &g_ecs_data->cameras[i];
        }
    }

    CZ_ASSERT(false, "Component does not exists");
    return nullptr;
}

TilemapComponent* add_tilemap_component(unsigned int entity_handle)
{
    TilemapComponent tilemap = {};
    tilemap.entity_handle = entity_handle;
    g_ecs_data->tilemaps[g_ecs_data->tilemap_size] = tilemap;
    
    return &g_ecs_data->tilemaps[g_ecs_data->tilemap_size++];
}

TilemapComponent* get_tilemap_component(unsigned int entity_handle)
{
    for (size_t i = 0; i < g_ecs_data->tilemap_size; i++)
    {
        if (g_ecs_data->tilemaps[i].entity_handle == entity_handle)
        {
            return &g_ecs_data->tilemaps[i];
        }
    }

    CZ_ASSERT(false, "Component does not exists");
    return nullptr;
}