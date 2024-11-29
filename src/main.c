#include "raylib.h"
#include <math.h>
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

#define WIN_WIDTH 800.0f
#define WIN_HEIGHT 450.0f
#define BG CLITERAL(Color){0x18, 0x18, 0x18, 0xFF}
#define GRAVITY 10.0f
#define DRAG 0.80f
#define SPAWN_DELAY 0.2f

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
    float spawn_timer;
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

        ball->position.y += ball->velocity.y * dt;
        ball->position.x += ball->velocity.x * dt;

        // check collison
        for (int j = 0; j < count; j++)
        {
            if(j != i) 
            {
                Entity* other_ball = &entities[j];

                float distx = ball->position.x - other_ball->position.x;
                float disty = ball->position.y - other_ball->position.y;

                float distance = sqrtf((distx * distx) + (disty * disty));

                if(distance <= (ball->size + other_ball->size))
                {
                    // Separate the balls to prevent overlap
                    float overlap = (ball->size + other_ball->size) - distance;
                    float normalization_factor = overlap / distance;

                    // Move the balls apart based on the overlap amount
                    // Move ball 1
                    ball->position.x += distx * normalization_factor * 0.5f; 
                    ball->position.y += disty * normalization_factor * 0.5f;

                    // Move ball 2
                    other_ball->position.x -= distx * normalization_factor * 0.5f; 
                    other_ball->position.y -= disty * normalization_factor * 0.5f;

                    // Swap velocities
                    Vector2 tmp = ball->velocity;
                    ball->velocity = other_ball->velocity;
                    other_ball->velocity = tmp;
                }
            }
        }

        // check y bounds
        if (ball->position.y >= (GetScreenHeight() - ball->size)) 
        {
            ball->position.y = GetScreenHeight() - ball->size;
            // Reverse and dampen the velocity (bounce)
            ball->velocity.y = -DRAG * ball->velocity.y; 
        }
        // check y bounds
        if (ball->position.y < ball->size) 
        {
            ball->position.y = ball->size;
            ball->velocity.y = -DRAG * ball->velocity.y;  
        }

        // check x bounds
        if(ball->position.x >= (GetScreenWidth() - ball->size)) 
        {
            ball->position.x = GetScreenWidth() - ball->size;
            ball->velocity.x = -DRAG * ball->velocity.x;  
        }

        // check x bounds
        if(ball->position.x <= 0 + ball->size) {
            ball->position.x = ball->size;
            ball->velocity.x = -DRAG * ball->velocity.x;  
        }

        // Check if the ball has come to rest on the ground
        if (ball->position.y >= (float)(WIN_HEIGHT - ball->size) && fabsf(ball->velocity.y) < 4.50f)
        {
            ball->velocity.x = 0.99 * ball->velocity.x; 

            // If the ball velocity is really slow, stop it
            if (fabsf(ball->velocity.x) <= 0.01f)
            {
                ball->velocity.x = 0;
            }
        }
    }
}

void generate_entities(App* app, int count, float dt)
{
    app->spawn_timer += dt;

    while(app->entity_count < count && app->spawn_timer >= SPAWN_DELAY) {
        int rand_dir = GetRandomValue((-WIN_WIDTH / 2), WIN_WIDTH / 2);
        Entity c = {
            .position = { WIN_WIDTH / 2, 20 },
            .velocity = {rand_dir, 0},
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

        app->spawn_timer = 0.0f;
    }

    return;
}

int main(void) {

    bool is_paused = false;

    Entity* entity_array = NULL;

    App my_app = {
        .is_running = true,
        .entities = entity_array,
        .entity_count = 0,
        .spawn_timer = 0
    };

    // raylib begin
    // ---------------------------------------------
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Ball Room");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();

        generate_entities(&my_app, 5, dt);

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
