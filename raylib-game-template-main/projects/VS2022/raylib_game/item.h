// Sreeram Vuppala
#pragma once
#include "raylib.h"
#include "raymath.h"
#include <tuple>

struct Item {
    Vector2 position;
    int points;
    bool alive = true;
    void rand() {
        alive = true;
        position.x = GetRandomValue(100, 700);
        position.y = GetRandomValue(100, 700);
        points = GetRandomValue(5, 10);
    }
    void draw(Texture texture) {
        if (!alive) return;
        DrawTexture(texture, position.x - 15, position.y - 15, WHITE);
    }
    bool operator<(const Item& other) const {
        return std::tie(position.x, position.y, points) < std::tie(other.position.x, other.position.y, other.points);
    }
    void hide() {
        alive = false;
    }
};