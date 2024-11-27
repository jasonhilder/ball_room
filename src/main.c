#include "raylib.h"
#include <math.h>
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

#define WIN_WIDTH 800.0f
#define WIN_HEIGHT 450.0f
#define BG CLITERAL(Color){0x18, 0x18, 0x18, 0xFF}
#define GRAVITY 10.0f
#define DRAG 0.95f

typedef enum  
{
    Circle
} entity_type;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    int size;
    entity_type type;
} Entity;

typedef struct {
    bool is_running;
    Entity* entities;
    int entity_count;
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

void update_entities(Entity* entities, int count, float dt)
{

    for (int i = 0; i < count; i++)
    {
        Entity* ball = &entities[i];

        // Apply gravity to the ball's velocity 
        ball->velocity.y += GRAVITY;

        // Update position based on velocity
        ball->position.y += ball->velocity.y * dt;
        ball->position.x += ball->velocity.x * dt;

        if (ball->position.y >= (GetScreenHeight() - ball->size)) {
            ball->position.y = GetScreenHeight() - ball->size;
            ball->velocity.y = -DRAG * ball->velocity.y; 
        }

        if (ball->position.y < ball->size) {
            ball->position.y = ball->size;
            ball->velocity.y = -DRAG * ball->velocity.y;  

            if(fabsf(ball->velocity.y) <= 0.1)
            {
                ball->velocity.y = 0;
            }
        }

    }
}

void generate_entities(App* app, int count)
{
    while(app->entity_count < count) {
        Entity c = {
            .position = { WIN_WIDTH / 2, 20 },
            .velocity = {0, 0},
            .color = (Color){
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                255
            },
            .size = 20,
            .type = Circle
        };

        arrput(app->entities, c);
        app->entity_count++;
    }

    return;
}

int main(void) {

    bool is_paused = false;

    Entity* entity_array = NULL;

    App my_app = {
        .is_running = true,
        .entities = entity_array,
        .entity_count = 0
    };

    // raylib begin
    // ---------------------------------------------
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Ball Room");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();

        generate_entities(&my_app, 1);

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
