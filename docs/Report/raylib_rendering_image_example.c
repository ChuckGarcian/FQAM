#include "raylib.h"

#define NUM_TEXTURES  9      // Currently we have 8 generation algorithms but some have multiple purposes (Linear and Square Gradients)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main (void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - procedural images generation");

    
    Image myimage = GenImageColor (screenWidth, screenHeight, RED);
    
    ImageDrawRectangleV(&myimage, (Vector2) {50, 50 }, (Vector2) {50, 50}, BLUE);
    ExportImage(myimage, "saved_image.png");

    Texture2D texture;

    texture = LoadTextureFromImage(myimage);

    // Unload image data (CPU RAM)
    UnloadImage(myimage);

    int currentTexture = 0;

    SetTargetFPS(60);
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTexture(texture, 0, 0, WHITE);

            DrawRectangle(30, 400, 325, 30, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(30, 400, 325, 30, Fade(WHITE, 0.5f));
            DrawText("MOUSE LEFT BUTTON to CYCLE PROCEDURAL TEXTURES", 40, 410, 10, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Unload textures data (GPU VRAM)
    for (int i = 0; i < NUM_TEXTURES; i++) UnloadTexture(texture);

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
