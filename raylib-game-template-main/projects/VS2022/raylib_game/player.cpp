// Sreeram Vuppala
#include "raylib.h"
#include "raymath.h"
#include "player.h"


void AgentOne::steer(Vector2 desired) {
    Vector2 steering = Vector2Subtract(desired, velocity);
    steering = Vector2ClampValue(steering, 0, maxForce);
    acceleration = Vector2Scale(steering, 1.0f / mass);
    velocity = Vector2Add(velocity, acceleration);
    velocity = Vector2ClampValue(velocity, 0, maxSpeed);
    position = Vector2Add(position, velocity);
}

void AgentOne::seek(Vector2 target) {
    Vector2 desired = Vector2Subtract(target, position);
    desired = Vector2Scale(Vector2Normalize(desired), maxSpeed);
    steer(desired);
}

void AgentOne::approach(Vector2 target) {
    Vector2 desired = Vector2Subtract(target, position);
    desired = Vector2ClampValue(desired, 0, maxSpeed);
    steer(desired);
}

void AgentOne::flee(Vector2 target) {
    Vector2 desired = Vector2Subtract(position, target);
    desired = Vector2Scale(Vector2Normalize(desired), maxSpeed);
    steer(desired);
}

void AgentOne::see(Item* target) {
    targets.insert(target);
}

void AgentOne::wander() {
    if (position.x < 100 || position.x > 700 || position.y < 100 || position.y > 700) seek(Vector2{ 400, 400 });
    else {
        Vector2 project = Vector2Add(position, Vector2Scale(Vector2Normalize(velocity), 100));
        if (wandertimer == 0) {
            wanderdeg += GetRandomValue(45, 90) * (GetRandomValue(0, 1) * 2 - 1) * RAD2DEG;
            wandertimer = 60;
        }
        wandertimer--;
        Vector2 rad = Vector2Add(project, Vector2Rotate(Vector2{ 50, 0 }, wanderdeg));
        DrawLine(position.x, position.y, rad.x, rad.y, ORANGE);
        seek(rad);
    }
}

void AgentOne::run() {
    if (merging != nullptr) {
        seek(merging->position);
    }
    if (current_target != nullptr && current_target->alive) {
        seek(current_target->position);
        return;
    }
    if (current_target != nullptr && !current_target->alive) {
        targets.erase(current_target);
        current_target = nullptr;
    }
    if (targets.size() == 0) {
        wander();
        return;
    }
    Item* closest = *targets.begin();
    float closest_dist = Vector2Distance(closest->position, position);
    for (auto i = targets.begin(); i != targets.end(); i++) {
        if (!(*i)->alive) continue;
        if (Vector2Distance((*i)->position, position) < closest_dist) {
            closest = *i;
            closest_dist = Vector2Distance((*i)->position, position);
        }
    }
    current_target = closest;
    seek(current_target->position);
}

void AgentOne::draw(bool evolving, float total) {
    elapsed += GetFrameTime();
    if (elapsed > 2) elapsed -= 2;
    if (!alive) {
        DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ 1, 1, 1, (3 - (total - 20)) / 3 }));
        return;
    }
    if (evolving && points == 1) {
        DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ 1, 1, 1, (total - 20) / 3 }));
        return;
    }
    if (!evolving && current_target != nullptr && current_target->alive == true) {
        DrawCircle(current_target->position.x, current_target->position.y, 3, GREEN);
        DrawLine(position.x, position.y, current_target->position.x, current_target->position.y, ORANGE);
    }
    DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, WHITE);
    if (evolving) DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ GREEN.r, GREEN.b, GREEN.g, float(1.0 - abs((total - 20) - 1.5) / 1.5) }));
}

void AgentOne::merge(AgentTwo* other) {
    merging = other;
}

AgentOne::AgentOne() {
    position = { float(GetRandomValue(100, 700)), float(GetRandomValue(100, 700)) };
    Image image = LoadImage("resources/pacmansprite.png");
    texture = LoadTextureFromImage(image);
}


void AgentTwo::steer(Vector2 desired) {
    Vector2 steering = Vector2Subtract(desired, velocity);
    steering = Vector2ClampValue(steering, 0, maxForce);
    acceleration = Vector2Scale(steering, 1.0f / mass);
    velocity = Vector2Add(velocity, acceleration);
    velocity = Vector2ClampValue(velocity, 0, maxSpeed);
    position = Vector2Add(position, velocity);
}

void AgentTwo::seek(Vector2 target) {
    Vector2 desired = Vector2Subtract(target, position);
    desired = Vector2Scale(Vector2Normalize(desired), maxSpeed);
    steer(desired);
}

void AgentTwo::approach(Vector2 target) {
    Vector2 desired = Vector2Subtract(target, position);
    desired = Vector2ClampValue(desired, 0, maxSpeed);
    steer(desired);
}

void AgentTwo::flee(Vector2 target) {
    Vector2 desired = Vector2Subtract(position, target);
    desired = Vector2Scale(Vector2Normalize(desired), maxSpeed);
    steer(desired);
}

void AgentTwo::see(Item* target) {
    targets.insert(target);
}

void AgentTwo::wander() {
    if (position.x < 100 || position.x > 700 || position.y < 100 || position.y > 700) seek(Vector2{ 400, 400 });
    else {
        Vector2 project = Vector2Add(position, Vector2Scale(Vector2Normalize(velocity), 100));
        if (wandertimer == 0) {
            wanderdeg += GetRandomValue(45, 90) * (GetRandomValue(0, 1) * 2 - 1) * RAD2DEG;
            wandertimer = 60;
        }
        wandertimer--;
        Vector2 rad = Vector2Add(project, Vector2Rotate(Vector2{ 50, 0 }, wanderdeg));
        DrawLine(position.x, position.y, rad.x, rad.y, ORANGE);
        seek(rad);
    }
}

void AgentTwo::run() {
    if (merging != nullptr) {
        seek(merging->position);
    }
    if (current_target != nullptr && current_target->alive) {
        seek(current_target->position);
        return;
    }
    if (current_target != nullptr && !current_target->alive) {
        targets.erase(current_target);
        current_target = nullptr;
    }
    if (targets.size() == 0) {
        wander();
        return;
    }
    Item* valuable = *targets.begin();
    float valuable_dist = Vector2Distance(valuable->position, position);
    for (auto i = targets.begin(); i != targets.end(); i++) {
        if (!(*i)->alive) continue;
        if ((*i)->points < valuable->points || ((*i)->points == valuable->points && Vector2Distance((*i)->position, position) < valuable_dist)) {
            valuable = *i;
            valuable_dist = Vector2Distance((*i)->position, position);
        }
    }
    current_target = valuable;
    seek(current_target->position);
}

void AgentTwo::draw(bool evolving, float total) {
    elapsed += GetFrameTime();
    if (elapsed > 2) elapsed -= 2;
    if (!alive) {
        DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ 1, 1, 1, (3 - (total - 20)) / 3 }));
        return;
    }
    if (evolving && points == 1) {
        DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ 1, 1, 1, (total - 20) / 3 }));
        return;
    }
    if (!evolving && current_target != nullptr && current_target->alive == true) {
        DrawCircle(current_target->position.x, current_target->position.y, 3, GREEN);
        DrawLine(position.x, position.y, current_target->position.x, current_target->position.y, ORANGE);
    }
    DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, WHITE);
    if (evolving) DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ YELLOW.r, YELLOW.b, YELLOW.g, float(1.0 - abs((total - 20) - 1.5) / 1.5) }));
}

void AgentTwo::merge(AgentOne* other) {
    merging = other;
}

AgentTwo::AgentTwo() {
    position = { float(GetRandomValue(100, 700)), float(GetRandomValue(100, 700)) };
    Image image = LoadImage("resources/redpac.png");
    texture = LoadTextureFromImage(image);
}

void AgentThree::steer(Vector2 desired) {
    Vector2 steering = Vector2Subtract(desired, velocity);
    steering = Vector2ClampValue(steering, 0, maxForce);
    acceleration = Vector2Scale(steering, 1.0f / mass);
    velocity = Vector2Add(velocity, acceleration);
    velocity = Vector2ClampValue(velocity, 0, maxSpeed);
    position = Vector2Add(position, velocity);
}

void AgentThree::seek(Vector2 target) {
    Vector2 desired = Vector2Subtract(target, position);
    desired = Vector2Scale(Vector2Normalize(desired), maxSpeed);
    steer(desired);
}

void AgentThree::approach(Vector2 target) {
    Vector2 desired = Vector2Subtract(target, position);
    desired = Vector2ClampValue(desired, 0, maxSpeed);
    steer(desired);
}

void AgentThree::flee(Vector2 target) {
    Vector2 desired = Vector2Subtract(position, target);
    desired = Vector2Scale(Vector2Normalize(desired), maxSpeed);
    steer(desired);
}

void AgentThree::see(Item* target) {
    targets.insert(target);
}

void AgentThree::wander() {
    if (position.x < 100 || position.x > 700 || position.y < 100 || position.y > 700) seek(Vector2{ 400, 400 });
    else steer(Vector2Scale(Vector2Normalize(Vector2Rotate(velocity, GetRandomValue(-10, 10) * DEG2RAD)), maxSpeed));
}

void AgentThree::run() {
    if (current_target != nullptr && current_target->alive) {
        seek(current_target->position);
        return;
    }
    if (current_target != nullptr && !current_target->alive) {
        targets.erase(current_target);
        current_target = nullptr;
    }
    if (targets.size() == 0) {
        wander();
        return;
    }
    Item* closest = *targets.begin();
    float closest_dist = Vector2Distance(closest->position, position);
    for (auto i = targets.begin(); i != targets.end(); i++) {
        if (!(*i)->alive) continue;
        if (Vector2Distance((*i)->position, position) < closest_dist) {
            closest = *i;
            closest_dist = Vector2Distance((*i)->position, position);
        }
    }
    current_target = closest;
    seek(current_target->position);
}

void AgentThree::draw(bool evolving, float total) {
    elapsed += GetFrameTime();
    if (elapsed > 2) elapsed -= 2;
    if (!alive) {
        DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ 1, 1, 1, (3 - (total - 20)) / 3 }));
        return;
    }
    if (points == -1) {
        DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ 1, 1, 1, (total - 20) / 3 }));
        return;
    }
    DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, WHITE);
    if (evolving) DrawTexturePro(texture, { float(50) * std::min(30 - int(30 * float(abs(1.0 - elapsed))), 29), 0, 50, 50 }, { position.x, position.y, 50, 50 }, { 25, 25 }, atan2(velocity.y, velocity.x) * RAD2DEG, ColorFromNormalized({ YELLOW.r, YELLOW.b, YELLOW.g, float(1.0 - abs((total - 20) - 1.5) / 1.5) }));
}

AgentThree::AgentThree() {
    position = { float(GetRandomValue(100, 700)), float(GetRandomValue(100, 700)) };
    Image image = LoadImage("resources/superpacman.png");
    texture = LoadTextureFromImage(image);
}