#include "raylib.h"
#include "rlgl.h"
#include <cmath>

enum Transformation
{
    TRANSLATION = 1,
    ROTATION,
    BOUNCE,
    SINE_MOVEMENT,
    TRAJECTORY,
    ORBIT
};

int main()
{
    const int screen_width = 1100;
    const int screen_height = 700;

    InitWindow(screen_width, screen_height, "Transformaciones matematicas 2D/3D - raylib");
    SetTargetFPS(60);

    Transformation current_transformation = TRANSLATION;
    bool is_3d_mode = false;

    Color translation_color = RED;
    Color rotation_color = ORANGE;
    Color bounce_color = GOLD;
    Color sine_color = GREEN;
    Color trajectory_color = BLUE;
    Color orbit_color = PURPLE;

    // estos rectangulos se usan para separar la informacion teorica de la animacion
    Rectangle info_panel = { 30.0f, 115.0f, 360.0f, 520.0f };
    Rectangle animation_area = { 430.0f, 115.0f, 630.0f, 520.0f };

    // variables para la traslacion en 2D
    Vector2 translation_position = {
        animation_area.x + 50.0f,
        animation_area.y + animation_area.height / 2.0f
    };

    float translation_speed = 180.0f;

    // variables para la rotacion. angle es la variable que va cambiando con el tiempo
    float angle = 0.0f;

    Vector2 rotation_center = {
        animation_area.x + animation_area.width / 2.0f,
        animation_area.y + animation_area.height / 2.0f
    };

    Rectangle rotation_rectangle = {
        rotation_center.x,
        rotation_center.y,
        170.0f,
        90.0f
    };

    // variables para el rebote en 2D
    Vector2 bounce_position = {
        animation_area.x + 120.0f,
        animation_area.y + 120.0f
    };

    Vector2 bounce_velocity = { 230.0f, 170.0f };
    float bounce_radius = 28.0f;

    // esta amplitud se usa en el movimiento senoidal
    float amplitude = 90.0f;

    // datos iniciales para la trayectoria parabolica en 2D
    Vector2 initial_position = {
        animation_area.x + 50.0f,
        animation_area.y + 430.0f
    };

    Vector2 initial_velocity = { 220.0f, -410.0f };
    float gravity = 520.0f;

    // configuramos la camara para poder visualizar todas las transformaciones en 3D
    Camera3D camera = { 0 };
    camera.position = { 6.5f, 5.5f, 6.5f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // usamos una textura para dibujar la escena 3D dentro del panel derecho
    RenderTexture2D target_3d = LoadRenderTexture(
        (int)animation_area.width,
        (int)animation_area.height
    );

    // variables para el rebote en 3D. aqui se mueve en X y Z dentro de una caja
    Vector3 bounce_position_3d = { -2.0f, 1.0f, -1.5f };
    Vector3 bounce_velocity_3d = { 2.1f, 0.0f, 1.7f };
    float box_limit_3d = 3.0f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        float time = (float)GetTime();

        // con estas teclas cambiamos la transformacion que se quiere ver
        if (IsKeyPressed(KEY_ONE)) current_transformation = TRANSLATION;
        if (IsKeyPressed(KEY_TWO)) current_transformation = ROTATION;
        if (IsKeyPressed(KEY_THREE)) current_transformation = BOUNCE;
        if (IsKeyPressed(KEY_FOUR)) current_transformation = SINE_MOVEMENT;
        if (IsKeyPressed(KEY_FIVE)) current_transformation = TRAJECTORY;
        if (IsKeyPressed(KEY_SIX)) current_transformation = ORBIT;

        // con TAB cambiamos entre las versiones 2D y 3D
        if (IsKeyPressed(KEY_TAB))
        {
            is_3d_mode = !is_3d_mode;
        }

        // actualizamos la traslacion usando velocidad y delta time
        translation_position.x += translation_speed * dt;

        if (translation_position.x > animation_area.x + animation_area.width - 40.0f)
        {
            translation_position.x = animation_area.x + 40.0f;
        }

        // el angulo aumenta constantemente, por eso se logra la rotacion
        angle += 90.0f * dt;

        // movimiento del rebote en 2D
        bounce_position.x += bounce_velocity.x * dt;
        bounce_position.y += bounce_velocity.y * dt;

        // si el circulo toca los limites en X, se invierte la velocidad horizontal
        if (bounce_position.x >= animation_area.x + animation_area.width - bounce_radius ||
            bounce_position.x <= animation_area.x + bounce_radius)
        {
            bounce_velocity.x *= -1.0f;
        }

        // si el circulo toca los limites en Y, se invierte la velocidad vertical
        if (bounce_position.y >= animation_area.y + animation_area.height - bounce_radius ||
            bounce_position.y <= animation_area.y + bounce_radius)
        {
            bounce_velocity.y *= -1.0f;
        }

        // movimiento del rebote en 3D, usando el plano XZ
        bounce_position_3d.x += bounce_velocity_3d.x * dt;
        bounce_position_3d.z += bounce_velocity_3d.z * dt;

        if (bounce_position_3d.x >= box_limit_3d || bounce_position_3d.x <= -box_limit_3d)
        {
            bounce_velocity_3d.x *= -1.0f;
        }

        if (bounce_position_3d.z >= box_limit_3d || bounce_position_3d.z <= -box_limit_3d)
        {
            bounce_velocity_3d.z *= -1.0f;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Transformaciones matematicas 2D/3D en raylib", 30, 25, 26, BLACK);

        DrawText(
            "1 Traslacion | 2 Rotacion | 3 Rebote | 4 Movimiento senoidal | 5 Trayectoria | 6 Orbita",
            30,
            65,
            18,
            DARKGRAY
        );

        DrawText(
            TextFormat("TAB Cambiar modo | Modo actual: %s", is_3d_mode ? "3D" : "2D"),
            30,
            90,
            18,
            is_3d_mode ? BLACK : DARKGRAY
        );

        DrawRectangleRec(info_panel, { 245, 245, 245, 255 });
        DrawRectangleLinesEx(info_panel, 2.0f, LIGHTGRAY);

        DrawRectangleRec(animation_area, { 250, 250, 250, 255 });
        DrawRectangleLinesEx(animation_area, 2.0f, LIGHTGRAY);

        if (current_transformation == TRANSLATION)
        {
            DrawText(is_3d_mode ? "Traslacion 3D" : "Traslacion 2D", 55, 145, 28, translation_color);

            DrawText("Modelo matematico:", 55, 205, 20, BLACK);

            if (!is_3d_mode)
            {
                DrawText("x = x0 + v * t", 55, 240, 24, DARKGRAY);
                DrawText("y = constante", 55, 275, 24, DARKGRAY);
            }
            else
            {
                DrawText("x = x0 + vx * t", 55, 240, 22, DARKGRAY);
                DrawText("y = constante", 55, 275, 22, DARKGRAY);
                DrawText("z = constante", 55, 310, 22, DARKGRAY);
            }

            DrawText("Interpretacion:", 55, 370, 20, BLACK);
            DrawText("- Cambia la posicion.", 55, 405, 18, GRAY);
            DrawText("- La forma no cambia.", 55, 430, 18, GRAY);

            if (!is_3d_mode)
            {
                DrawLine(
                    (int)animation_area.x + 60,
                    (int)(animation_area.y + animation_area.height / 2.0f),
                    (int)(animation_area.x + animation_area.width - 60),
                    (int)(animation_area.y + animation_area.height / 2.0f),
                    LIGHTGRAY
                );

                DrawCircleV(translation_position, 38.0f, translation_color);
            }
            else
            {
                BeginTextureMode(target_3d);
                ClearBackground({ 250, 250, 250, 255 });

                BeginMode3D(camera);

                DrawGrid(12, 1.0f);

                // calculamos una posicion en X para que el cubo se traslade en 3D
                float x_position = -3.0f + fmodf(time * 1.8f, 6.0f);

                DrawCube({ x_position, 1.0f, 0.0f }, 0.8f, 0.8f, 0.8f, translation_color);
                DrawCubeWires({ x_position, 1.0f, 0.0f }, 0.8f, 0.8f, 0.8f, MAROON);
                DrawLine3D({ -3.0f, 0.05f, 0.0f }, { 3.0f, 0.05f, 0.0f }, LIGHTGRAY);

                EndMode3D();
                EndTextureMode();

                DrawTexturePro(
                    target_3d.texture,
                    { 0.0f, 0.0f, (float)target_3d.texture.width, -(float)target_3d.texture.height },
                    animation_area,
                    { 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
        }

        if (current_transformation == ROTATION)
        {
            DrawText(is_3d_mode ? "Rotacion 3D" : "Rotacion 2D", 55, 145, 28, rotation_color);

            DrawText("Modelo matematico:", 55, 205, 20, BLACK);

            if (!is_3d_mode)
            {
                DrawText("x' = x cos(t) - y sin(t)", 55, 240, 20, DARKGRAY);
                DrawText("y' = x sin(t) + y cos(t)", 55, 275, 20, DARKGRAY);
            }
            else
            {
                DrawText("R_y(theta)", 55, 240, 24, DARKGRAY);
                DrawText("x' = x cos(t) + z sin(t)", 55, 275, 19, DARKGRAY);
                DrawText("z' = -x sin(t) + z cos(t)", 55, 305, 19, DARKGRAY);
            }

            DrawText("Interpretacion:", 55, 370, 20, BLACK);
            DrawText("- El angulo cambia.", 55, 405, 18, GRAY);
            DrawText("- El objeto gira sobre", 55, 430, 18, GRAY);
            DrawText("  un eje.", 55, 455, 18, GRAY);

            if (!is_3d_mode)
            {
                DrawCircleLines(
                    (int)rotation_center.x,
                    (int)rotation_center.y,
                    115.0f,
                    LIGHTGRAY
                );

                DrawCircleV(rotation_center, 5.0f, BLACK);

                DrawRectanglePro(
                    rotation_rectangle,
                    { rotation_rectangle.width / 2.0f, rotation_rectangle.height / 2.0f },
                    angle,
                    rotation_color
                );
            }
            else
            {
                BeginTextureMode(target_3d);
                ClearBackground({ 250, 250, 250, 255 });

                BeginMode3D(camera);

                DrawGrid(12, 1.0f);

                // aqui se aplican transformaciones directamente a la matriz actual
                rlPushMatrix();
                rlTranslatef(0.0f, 1.0f, 0.0f);
                rlRotatef(angle, 0.0f, 1.0f, 0.0f);
                DrawCube({ 0.0f, 0.0f, 0.0f }, 2.0f, 0.8f, 1.0f, rotation_color);
                DrawCubeWires({ 0.0f, 0.0f, 0.0f }, 2.0f, 0.8f, 1.0f, BROWN);
                rlPopMatrix();

                DrawLine3D({ 0.0f, 0.0f, 0.0f }, { 0.0f, 2.5f, 0.0f }, BLACK);

                EndMode3D();
                EndTextureMode();

                DrawTexturePro(
                    target_3d.texture,
                    { 0.0f, 0.0f, (float)target_3d.texture.width, -(float)target_3d.texture.height },
                    animation_area,
                    { 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
        }

        if (current_transformation == BOUNCE)
        {
            DrawText(is_3d_mode ? "Rebote 3D" : "Rebote 2D", 55, 145, 28, bounce_color);

            DrawText("Modelo matematico:", 55, 205, 20, BLACK);

            if (!is_3d_mode)
            {
                DrawText("x = x + vx * dt", 55, 240, 22, DARKGRAY);
                DrawText("y = y + vy * dt", 55, 275, 22, DARKGRAY);
            }
            else
            {
                DrawText("x = x + vx * dt", 55, 240, 22, DARKGRAY);
                DrawText("z = z + vz * dt", 55, 275, 22, DARKGRAY);
            }

            DrawText("v = -v", 55, 310, 22, DARKGRAY);

            DrawText("Interpretacion:", 55, 370, 20, BLACK);
            DrawText("- Al tocar un limite,", 55, 405, 18, GRAY);
            DrawText("  cambia el signo de", 55, 430, 18, GRAY);
            DrawText("  la velocidad.", 55, 455, 18, GRAY);

            if (!is_3d_mode)
            {
                DrawRectangleLinesEx(animation_area, 3.0f, LIGHTGRAY);
                DrawCircleV(bounce_position, bounce_radius, bounce_color);
            }
            else
            {
                BeginTextureMode(target_3d);
                ClearBackground({ 250, 250, 250, 255 });

                BeginMode3D(camera);

                DrawGrid(12, 1.0f);

                // la esfera rebota dentro de una caja, invirtiendo su velocidad en los limites
                DrawCubeWires({ 0.0f, 1.0f, 0.0f }, 6.0f, 2.0f, 6.0f, LIGHTGRAY);
                DrawSphere(bounce_position_3d, 0.35f, bounce_color);

                EndMode3D();
                EndTextureMode();

                DrawTexturePro(
                    target_3d.texture,
                    { 0.0f, 0.0f, (float)target_3d.texture.width, -(float)target_3d.texture.height },
                    animation_area,
                    { 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
        }

        if (current_transformation == SINE_MOVEMENT)
        {
            DrawText(is_3d_mode ? "Movimiento senoidal 3D" : "Movimiento senoidal 2D", 55, 145, 22, sine_color);

            DrawText("Modelo matematico:", 55, 205, 20, BLACK);

            if (!is_3d_mode)
            {
                DrawText("y = y0 + A sin(wt)", 55, 240, 22, DARKGRAY);
            }
            else
            {
                DrawText("x = avance", 55, 240, 22, DARKGRAY);
                DrawText("y = y0 + A sin(wt)", 55, 275, 22, DARKGRAY);
                DrawText("z = constante", 55, 310, 22, DARKGRAY);
            }

            DrawText("Interpretacion:", 55, 370, 20, BLACK);
            DrawText("- A controla la altura.", 55, 405, 18, GRAY);
            DrawText("- sin(wt) produce", 55, 430, 18, GRAY);
            DrawText("  periodicidad.", 55, 455, 18, GRAY);

            if (!is_3d_mode)
            {
                float margin = 60.0f;
                float usable_width = animation_area.width - 2.0f * margin;
                float center_y = animation_area.y + animation_area.height / 2.0f;

                // progress representa cuanto ha avanzado el objeto sobre la curva senoidal
                float progress = fmodf(time * 120.0f, usable_width);
                float x_position = animation_area.x + margin + progress;
                float y_position = center_y + amplitude * sinf(0.035f * progress);

                DrawLine(
                    (int)(animation_area.x + margin),
                    (int)center_y,
                    (int)(animation_area.x + animation_area.width - margin),
                    (int)center_y,
                    LIGHTGRAY
                );

                // dibujamos la curva completa para que se vea la trayectoria que sigue el punto
                for (int i = 0; i < usable_width; i++)
                {
                    float px = animation_area.x + margin + i;
                    float py = center_y + amplitude * sinf(0.035f * i);

                    DrawPixel((int)px, (int)py, LIGHTGRAY);
                    DrawPixel((int)px, (int)py + 1, LIGHTGRAY);
                }

                DrawLine((int)x_position, (int)center_y, (int)x_position, (int)y_position, GRAY);
                DrawCircle((int)x_position, (int)y_position, 30.0f, sine_color);
            }
            else
            {
                BeginTextureMode(target_3d);
                ClearBackground({ 250, 250, 250, 255 });

                BeginMode3D(camera);

                DrawGrid(12, 1.0f);

                // en 3D el movimiento senoidal se dibuja sobre X y Y, dejando Z constante
                float progress = fmodf(time * 1.5f, 6.0f) - 3.0f;
                float y_position = 1.2f + 0.8f * sinf(2.5f * progress);

                for (int i = 0; i < 100; i++)
                {
                    float x1 = -3.0f + 6.0f * ((float)i / 100.0f);
                    float x2 = -3.0f + 6.0f * ((float)(i + 1) / 100.0f);

                    float y1 = 1.2f + 0.8f * sinf(2.5f * x1);
                    float y2 = 1.2f + 0.8f * sinf(2.5f * x2);

                    DrawLine3D({ x1, y1, 0.0f }, { x2, y2, 0.0f }, LIGHTGRAY);
                }

                DrawSphere({ progress, y_position, 0.0f }, 0.32f, sine_color);

                EndMode3D();
                EndTextureMode();

                DrawTexturePro(
                    target_3d.texture,
                    { 0.0f, 0.0f, (float)target_3d.texture.width, -(float)target_3d.texture.height },
                    animation_area,
                    { 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
        }

        if (current_transformation == TRAJECTORY)
        {
            DrawText(is_3d_mode ? "Trayectoria 3D" : "Trayectoria 2D", 55, 145, 28, trajectory_color);

            DrawText("Modelo matematico:", 55, 205, 20, BLACK);

            if (!is_3d_mode)
            {
                DrawText("x = x0 + vx * t", 55, 240, 22, DARKGRAY);
                DrawText("y = y0 + vy * t", 55, 275, 22, DARKGRAY);
                DrawText("    + 1/2 * g * t^2", 55, 305, 22, DARKGRAY);
            }
            else
            {
                DrawText("x = x0 + vx * t", 55, 240, 22, DARKGRAY);
                DrawText("y = y0 + vy * t", 55, 275, 22, DARKGRAY);
                DrawText("    - 1/2 * g * t^2", 55, 305, 22, DARKGRAY);
                DrawText("z = constante", 55, 335, 22, DARKGRAY);
            }

            DrawText("Interpretacion:", 55, 390, 20, BLACK);
            DrawText("- x avanza linealmente.", 55, 425, 18, GRAY);
            DrawText("- y cambia por efecto", 55, 450, 18, GRAY);
            DrawText("  de la gravedad.", 55, 475, 18, GRAY);

            if (!is_3d_mode)
            {
                float trajectory_time = fmodf(time, 2.1f);

                // aqui se aplica la ecuacion parametrica de la trayectoria
                float x_position = initial_position.x + initial_velocity.x * trajectory_time;

                float y_position = initial_position.y +
                                   initial_velocity.y * trajectory_time +
                                   0.5f * gravity * trajectory_time * trajectory_time;

                // este ciclo solo dibuja los puntos de referencia de la parabola
                for (float ti = 0.0f; ti <= 2.1f; ti += 0.03f)
                {
                    float px = initial_position.x + initial_velocity.x * ti;

                    float py = initial_position.y +
                               initial_velocity.y * ti +
                               0.5f * gravity * ti * ti;

                    if (py >= animation_area.y &&
                        py <= animation_area.y + animation_area.height &&
                        px >= animation_area.x &&
                        px <= animation_area.x + animation_area.width)
                    {
                        DrawCircle((int)px, (int)py, 2.0f, LIGHTGRAY);
                    }
                }

                DrawLine(
                    (int)animation_area.x + 40,
                    (int)initial_position.y,
                    (int)(animation_area.x + animation_area.width - 40),
                    (int)initial_position.y,
                    LIGHTGRAY
                );

                if (x_position >= animation_area.x + 26.0f &&
                    x_position <= animation_area.x + animation_area.width - 26.0f &&
                    y_position >= animation_area.y + 26.0f &&
                    y_position <= animation_area.y + animation_area.height - 26.0f)
                {
                    DrawCircle((int)x_position, (int)y_position, 26.0f, trajectory_color);
                }
            }
            else
            {
                BeginTextureMode(target_3d);
                ClearBackground({ 250, 250, 250, 255 });

                BeginMode3D(camera);

                DrawGrid(12, 1.0f);

                float trajectory_time = fmodf(time, 2.2f);

                // para la version 3D dejamos Z constante y usamos X/Y para la parabola
                float x_position = -3.0f + 2.7f * trajectory_time;
                float y_position = 0.4f + 4.2f * trajectory_time - 2.2f * trajectory_time * trajectory_time;
                float z_position = 0.0f;

                for (float ti = 0.0f; ti <= 2.2f; ti += 0.04f)
                {
                    float px = -3.0f + 2.7f * ti;
                    float py = 0.4f + 4.2f * ti - 2.2f * ti * ti;

                    if (py >= 0.0f)
                    {
                        DrawSphere({ px, py, z_position }, 0.04f, LIGHTGRAY);
                    }
                }

                if (y_position >= 0.0f)
                {
                    DrawSphere({ x_position, y_position, z_position }, 0.32f, trajectory_color);
                }

                EndMode3D();
                EndTextureMode();

                DrawTexturePro(
                    target_3d.texture,
                    { 0.0f, 0.0f, (float)target_3d.texture.width, -(float)target_3d.texture.height },
                    animation_area,
                    { 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
        }

        if (current_transformation == ORBIT)
        {
            DrawText(is_3d_mode ? "Orbita 3D" : "Orbita 2D", 55, 145, 28, orbit_color);

            DrawText("Modelo matematico:", 55, 205, 20, BLACK);

            if (!is_3d_mode)
            {
                DrawText("x = x0 + cos(t) * r", 55, 240, 22, DARKGRAY);
                DrawText("y = y0 + sin(t) * r", 55, 275, 22, DARKGRAY);
            }
            else
            {
                DrawText("x = cos(t) * radio", 55, 240, 22, DARKGRAY);
                DrawText("z = sin(t) * radio", 55, 275, 22, DARKGRAY);
                DrawText("y = constante", 55, 310, 22, DARKGRAY);
            }

            DrawText("Interpretacion:", 55, 370, 20, BLACK);
            DrawText("- cos(t) controla X.", 55, 405, 18, GRAY);
            DrawText("- sin(t) controla Y/Z.", 55, 430, 18, GRAY);
            DrawText("- radio define la orbita.", 55, 455, 18, GRAY);

            if (!is_3d_mode)
            {
                Vector2 orbit_center = {
                    animation_area.x + animation_area.width / 2.0f,
                    animation_area.y + animation_area.height / 2.0f
                };

                float orbit_radius = 150.0f;

                // usamos coseno y seno para calcular la posicion del objeto orbitando
                float orbit_x = orbit_center.x + cosf(time) * orbit_radius;
                float orbit_y = orbit_center.y + sinf(time) * orbit_radius;

                DrawCircleLines((int)orbit_center.x, (int)orbit_center.y, orbit_radius, LIGHTGRAY);
                DrawCircleV(orbit_center, 45.0f, YELLOW);
                DrawCircle((int)orbit_x, (int)orbit_y, 28.0f, orbit_color);
            }
            else
            {
                BeginTextureMode(target_3d);
                ClearBackground({ 250, 250, 250, 255 });

                BeginMode3D(camera);

                DrawGrid(12, 1.0f);

                float orbit_radius = 2.0f;

                // en 3D la orbita se forma sobre el plano XZ
                float orbit_x = cosf(time) * orbit_radius;
                float orbit_z = sinf(time) * orbit_radius;

                DrawSphere({ 0.0f, 1.0f, 0.0f }, 0.65f, YELLOW);
                DrawSphere({ orbit_x, 1.0f, orbit_z }, 0.35f, orbit_color);

                DrawCircle3D(
                    { 0.0f, 1.0f, 0.0f },
                    orbit_radius,
                    { 1.0f, 0.0f, 0.0f },
                    90.0f,
                    LIGHTGRAY
                );

                EndMode3D();
                EndTextureMode();

                DrawTexturePro(
                    target_3d.texture,
                    { 0.0f, 0.0f, (float)target_3d.texture.width, -(float)target_3d.texture.height },
                    animation_area,
                    { 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
        }

        DrawRectangleLinesEx(animation_area, 2.0f, LIGHTGRAY);

        EndDrawing();
    }

    // liberamos la textura usada para renderizar la escena 3D
    UnloadRenderTexture(target_3d);
    CloseWindow();

    return 0;
}