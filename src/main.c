#include "raylib.h"
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

#define WIN_WIDTH 800.0
#define WIN_HEIGHT 450.0
#define BG CLITERAL(Color){0x18, 0x18, 0x18, 0xFF}

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

void update_entities(Entity* entities, int count, float deltaTime)
{
    for (int i = 0; i < count; i++)
    {
        Entity* ball = &entities[i];

        // Update position based on velocity
        ball->position.y += ball->velocity.y;
        ball->position.x += ball->velocity.x;

        // Check Y boundaries (top and bottom)
        if (ball->position.y >= (GetScreenHeight() - ball->size)) {
            if (ball->velocity.y > 0) {
                int randVelocity = GetRandomValue(-15, -5); 
                ball->velocity.y = (float)randVelocity;
            }
        }
        else if (ball->position.y <= ball->size) {
            if (ball->velocity.y < 0) {
                int randVelocity = GetRandomValue(5, 15);
                ball->velocity.y = (float)randVelocity;
            }
        }

        if (ball->position.x >= (GetScreenWidth() - ball->size)) {
            if (ball->velocity.x > 0) {
                int randVelocity = GetRandomValue(-15, -5);
                ball->velocity.x = (float)randVelocity;
            }
        }
        else if (ball->position.x <= ball->size) {
            if (ball->velocity.x < 0) {
                int randVelocity = GetRandomValue(5, 15);
                ball->velocity.x = (float)randVelocity;
            }
        }
    }
}

void generate_entities(App* app, int count)
{
    while(app->entity_count < count) {
        int rand_grav_x = GetRandomValue(-10, 10);
        int rand_grav_y = GetRandomValue(5, 15);
        int rand_size = GetRandomValue(15, 30);

        Entity c = {
            .position = { WIN_WIDTH / 2, WIN_HEIGHT / 2 },
            .velocity = {(float)rand_grav_x, (float)rand_grav_y},
            .color = (Color){
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                GetRandomValue(0, 255),
                255
            },
            .size = rand_size,
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

        generate_entities(&my_app, 333);

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
