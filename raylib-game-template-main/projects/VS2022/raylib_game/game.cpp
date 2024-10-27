// Sreeram Vuppala
#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "item.h"
#include <vector>

#include <iostream>

using namespace std;

bool can_see(AgentOne player, Item item) {
    return item.alive && CheckCollisionCircles(player.position, player.sight, item.position, player.seeRange);
}

bool can_see(AgentTwo player, Item item) {
    return item.alive && CheckCollisionCircles(player.position, player.sight, item.position, player.seeRange) && abs(atan((item.position.y - player.position.y) / (item.position.x - player.position.x)) * RAD2DEG) < 60;
}

bool can_see(AgentThree player, Item item) {
    return item.alive && ((CheckCollisionCircles(player.position, player.sight, item.position, player.coneRange) && abs(atan((item.position.y - player.position.y) / (item.position.x - player.position.x)) * RAD2DEG) < 60) ||
        CheckCollisionCircles(player.position, player.sight, item.position, player.aroundRange));
}

int main(void)
{
    InitWindow(800, 800, "raylib [core] example - basic window");
    SetTargetFPS(30);
    Item items[100];
    int resets = 0;
    for (int i = 0; i < 100 - resets * 5; i++) items[i].rand();
    int agent_count = 5;
    vector<AgentOne> agents_one(agent_count);
    vector<AgentTwo> agents_two(agent_count);
    //vector<AgentThree> agents_super;
    float time = 0;
    Image food_img = LoadImage("resources/food.png");
    ImageResize(&food_img, 30, 30);
    Texture food = LoadTextureFromImage(food_img);

    
    float foodTimer = 20.0;
    bool evolving = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(SKYBLUE);
        if (evolving) goto DRAW;
        for (int i = 0; i < 100 - resets * 5; i++) {
            if (!items[i].alive) continue;
            for (int j = 0; j < agents_one.size(); j++) {
                if (!agents_one[j].alive) continue;
                if (Vector2Distance(agents_one[j].position, items[i].position) < 30) {
                    agents_one[j].points += items[i].points;
                    items[i].hide();
                }
                if (can_see(agents_one[j], items[i])) agents_one[j].see(&items[i]);
            }
            for (int j = 0; j < agents_two.size(); j++) {
                if (!agents_two[j].alive) continue;
                if (Vector2Distance(agents_two[j].position, items[i].position) < 30) {
                    agents_two[j].points += items[i].points;
                    items[i].hide();
                }
                if (can_see(agents_two[j], items[i])) agents_two[j].see(&items[i]);
            }
        }

        for (int i = 0; i < agents_one.size(); i++) {
            if (agents_one[i].alive && agents_one[i].current_target != nullptr && Vector2Distance(agents_one[i].position, agents_one[i].current_target->position) < 30) {
                agents_one[i].targets.erase(agents_one[i].current_target);
                agents_one[i].current_target = nullptr;
            }
        }
        for (int i = 0; i < agents_two.size(); i++) {
            if (agents_two[i].alive && agents_two[i].current_target != nullptr && Vector2Distance(agents_two[i].position, agents_two[i].current_target->position) < 30) {
                agents_two[i].targets.erase(agents_two[i].current_target);
                agents_two[i].current_target = nullptr;
            }
        }

        
    DRAW:
        if (!evolving) for (int i = 0; i < 100 - resets * 5; i++) items[i].draw(food);
        for (int i = 0; i < agents_one.size(); i++) {
            if (agents_one[i].alive) agents_one[i].run();
            agents_one[i].draw(evolving, time);
            if (agents_one[i].merging != nullptr && Vector2Distance(agents_one[i].position, agents_one[i].merging->position) < 50) {
                int child = GetRandomValue(0, 1);
                if (child == 0) {
                    AgentOne to_add;
                    to_add.maxForce = (agents_one[i].maxForce + agents_one[i].merging->maxForce) / 2;
                    to_add.maxSpeed = (agents_one[i].maxSpeed + agents_one[i].merging->maxSpeed) / 2;
                    to_add.seeRange = (agents_one[i].seeRange + agents_one[i].merging->seeRange) / 2 - 25;
                    to_add.position = Vector2Scale(Vector2Add(agents_one[i].position, agents_one[i].merging->position), .5);
                    agents_one.push_back(to_add);
                    to_add.points = 1;
                }
                else {
                    AgentTwo to_add;
                    to_add.maxForce = (agents_one[i].maxForce + agents_one[i].merging->maxForce) / 2;
                    to_add.maxSpeed = (agents_one[i].maxSpeed + agents_one[i].merging->maxSpeed) / 2;
                    to_add.seeRange = (agents_one[i].seeRange + agents_one[i].merging->seeRange) / 2 + 25;
                    to_add.position = Vector2Scale(Vector2Add(agents_one[i].position, agents_one[i].merging->position), .5);
                    agents_two.push_back(to_add);
                    to_add.points = 1;
                }
                agents_one[i].merging->merging = nullptr;
                agents_one[i].merging = nullptr;
            }
        }
        for (int i = 0; i < agents_two.size(); i++) {
            if (agents_two[i].alive) agents_two[i].run();
            agents_two[i].draw(evolving, time);
        }
        //DrawCircle(items[0].position.x, items[0].position.y, 3, RED);
        time += GetFrameTime();
        if (time > foodTimer && !evolving) {
            resets++;
            for (int i = 0; i < 100 - resets * 5; i++) items[i].rand();
            for (int i = 0; i < agents_one.size(); i++) {
                agents_one[i].current_target = nullptr;
                if (agents_one[i].points < 50) agents_one[i].alive = false;
                else {
                    agents_one[i].evolve();
                    agents_one[i].targets.clear();
                }
            }
            for (int i = 0; i < agents_two.size(); i++) {
                agents_two[i].current_target = nullptr;
                if (agents_two[i].points < 50) agents_two[i].alive = false;
                else {
                    agents_two[i].evolve();
                    agents_two[i].targets.clear();
                }
            }
            for (int i = 0; i < agents_one.size(); i++) {
                if (!agents_one[i].alive) continue;
                int close = -1;
                float closest = 0;
                for (int j = 0; j < agents_two.size(); j++) {
                    if (agents_two[j].alive && agents_two[j].merging == nullptr && Vector2Distance(agents_one[i].position, agents_two[j].position) < 200) {
                        close = j;
                        closest = Vector2Distance(agents_one[i].position, agents_two[j].position);
                    }
                }
                if (close != -1) {
                    agents_one[i].merge(&agents_two[close]);
                    agents_two[close].merge(&agents_one[i]);
                }
            }
            evolving = true;
        }
        if (time > foodTimer + 3) {
            time -= (foodTimer + 3);
            evolving = false;
            for (int i = 0; i < agents_one.size(); i++) {
                agents_one[i].merging = nullptr;
                if (!agents_one[i].alive) agents_one[i].position = { -200, -200 };
            }
            for (int i = 0; i < agents_two.size(); i++) {
                agents_two[i].merging = nullptr;
                if (!agents_two[i].alive) agents_two[i].position = { -200, -200 };
            }
        }
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

/*
A lot of pacmans
eat each other
or passive with each other
evolve speed + detect range
*/
/*
Spritesheet generation
    <!DOCTYPE html>
<html>
<body>

<canvas id="myCanvas" width="1500" height="50" style="border:1px solid #000000;">
</canvas>


<script>
var c = document.getElementById("myCanvas");
var ctx = c.getContext("2d");
ctx.fillStyle = "yellow";
for (let i = 0; i < 30; i++) {
    	ctx.moveTo(25 + 50 * i, 25);
        ctx.arc(25 + 50 * i, 25,25, 1.5 * i * Math.PI / 180, 2 * Math.PI - 1.5 * i * Math.PI / 180);
        ctx.lineTo(25 + 50 * i, 25);
        ctx.fill();
}
 // or context.fill()
</script>
</body>
</html>
*/