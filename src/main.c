#include "raylib.h"
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

#define WIN_WIDTH 1000.0
#define WIN_HEIGHT 800.0
#define BG CLITERAL(Color){0x18, 0x18, 0x18, 0xFF}

// Adjust gravity value for smoother fall
#define GRAVITY 1000.0f 
#define MAX_SPEED 16.0f

typedef enum  
{
    Circle
} entity_type;

typedef struct {
    Vector2 position;
    Color color;
    int size;
    entity_type type;
} Entity;

typedef struct {
    bool is_running;
    Entity* entities;
} App;

void render_entities(Entity* entities, int count)
{
    for (int i = 0; i < count; i++) {
        switch(entities[i].type)
        {
            case Circle:
                DrawCircle(entities[i].position.x, entities[i].position.y, entities[i].size, entities[i].color);
                break;
        }
    }
}

void update_entities(Entity* entities, int count, float deltaTime)
{
    for (int i = 0; i < count; i++)
    {
        // Calculate new position based on gravity and deltaTime
        float fallSpeed = GRAVITY * deltaTime;
        // Clamp the fall speed to prevent choppiness
        if (fallSpeed > MAX_SPEED) fallSpeed = MAX_SPEED;

        // Update the position of each entity
        if(!(entities[i].position.y >= WIN_HEIGHT - entities[i].size))
        {
            entities[i].position.y += fallSpeed;
        }
    }
}


int main(void) {

    bool is_paused = false;

    Entity* entity_array = NULL;

    Entity c = {
        { WIN_WIDTH / 2, 0 },
        RED,
        20,
        Circle
    };
    arrput(entity_array,c);

    App my_app = {
        true,
        entity_array
    };

    // raylib begin
    // ---------------------------------------------
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Ball Room");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();

        BeginDrawing();

            ClearBackground(BG);

            // Inputs
            if (IsKeyPressed(KEY_SPACE)) is_paused = !is_paused;

            // Render app
            render_entities(my_app.entities, arrlen(my_app.entities));

            // Update app
            if(!is_paused) 
                update_entities(my_app.entities, arrlen(my_app.entities), dt);

            DrawFPS(10, 10);
        EndDrawing();
    }

    // Clean up
    // ---------------------------------------------
    
    arrfree(entity_array);
    CloseWindow();
    return 0;
}
