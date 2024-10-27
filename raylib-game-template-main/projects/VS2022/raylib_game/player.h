// Sreeram Vuppala
#pragma once
#include "raylib.h"
#include "raymath.h"
#include <set>
#include "item.h"
#include <iostream>

class AgentTwo;

class AgentOne {
    float mass = 5.0;
    Vector2 acceleration = Vector2Zero();
public:
    float maxForce = .15;
    float maxSpeed = 3.0;
    Vector2 velocity = Vector2Zero();
    Vector2 position;
    Texture texture;
    std::set<Item*> targets;
    Item* current_target = nullptr;
    float elapsed = 0.0;
    int points = 0;
    float sight = 10.0;
    bool alive = true;
    AgentTwo* merging = nullptr;
    int seeRange = 100;
    float wanderdeg = GetRandomValue(0, 360) * DEG2RAD;
    int wandertimer = 30;
    void steer(Vector2 desired);
    void seek(Vector2 target);
    void approach(Vector2 target);
    void flee(Vector2 target);
    void see(Item* target);
    void wander();
    void draw(bool evolving, float total);
    void evolve() {
        int levels = points / 50;
        points = 0;
        switch (GetRandomValue(0, 2)) {
        case 0: maxForce += .05 * levels; break;
        case 1: maxSpeed += .5 * levels; break;
        default: seeRange += 25 * levels;
        }
        switch (GetRandomValue(0, 2)) {
        case 0: maxForce -= .025 * levels; break;
        case 1: maxSpeed -= .25 * levels; break;
        default: seeRange -= 12 * levels;
        }
    }
    void merge(AgentTwo* other);
    void run();
    AgentOne();
};

class AgentTwo {
    float mass = 5.0;
    Vector2 acceleration = Vector2Zero();
public:
    float maxForce = .15;
    float maxSpeed = 3.0;
    Vector2 velocity = Vector2Zero();
    Vector2 position;
    Texture texture;
    std::set<Item*> targets;
    Item* current_target = nullptr;
    float elapsed = 0.0;
    int points = 0;
    float sight = 10.0;
    bool alive = true;
    AgentOne* merging = nullptr;
    int seeRange = 150;
    float wanderdeg = GetRandomValue(0, 360) * DEG2RAD;
    int wandertimer = 30;
    void steer(Vector2 desired);
    void seek(Vector2 target);
    void approach(Vector2 target);
    void flee(Vector2 target);
    void see(Item* target);
    void wander();
    void draw(bool evolving, float total);
    void evolve() {
        int levels = points / 50;
        points = 0;
        maxForce += .05 * levels;
        maxSpeed += .5 * levels;
        switch (GetRandomValue(0, 2)) {
        case 0: maxForce += .05 * levels; break;
        case 1: maxSpeed += .5 * levels; break;
        default: seeRange += 25 * levels;
        }
        switch (GetRandomValue(0, 2)) {
        case 0: maxForce -= .025 * levels; break;
        case 1: maxSpeed -= .25 * levels; break;
        default: seeRange -= 12 * levels;
        }
    }
    void merge(AgentOne* other);
    void run();
    AgentTwo();
};

class AgentThree {
    float mass = 5.0;
    Vector2 acceleration = Vector2Zero();
public:
    float maxForce = .15;
    float maxSpeed = 3.0;
    Vector2 velocity = Vector2Zero();
    Vector2 position;
    Texture texture;
    std::set<Item*> targets;
    Item* current_target = nullptr;
    float elapsed = 0.0;
    int points = 0;
    float sight = 10.0;
    bool alive = true;
    int aroundRange = 100;
    int coneRange = 150;
    void steer(Vector2 desired);
    void seek(Vector2 target);
    void approach(Vector2 target);
    void flee(Vector2 target);
    void see(Item* target);
    void wander();
    void draw(bool evolving, float total);
    void evolve() {
        int levels = points / 50;
        points = 0;
        switch (GetRandomValue(0, 3)) {
        case 0: maxForce += .05 * levels; break;
        case 1: maxSpeed += .5 * levels; break;
        case 2: aroundRange += 25 * levels; break;
        case 3: coneRange += 25 * levels; break;
        default: break;
        }
    }
    void run();
    AgentThree();
};