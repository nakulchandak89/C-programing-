#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define HEIGHT 5
#define WIDTH 5

typedef struct {
    int hp;    // health points for player
    int atk;   // attack points for the player
    int dfp;   // defense points for player
    int x, y;  // position coordinates for player
    int potions; // number of health potions
} Player;

typedef struct {
    int hp;    // health points for Enemy
    int atk;   // attack points for enemy
    int alive; // is enemy alive or dead
    int x, y;  // position coordinates for the enemy
} Enemy;


// declearing all the function which we are going to use 

void Display_Grid(Player player, Enemy enemy1, Enemy enemy2, int potion_x, int potion_y); // this function will display the map of the game

void move_player(Player *player, char direction); // this function wil help us to move the player (joy stick)

int enemy_near(Player player, Enemy enemy); // this function is going to track the enemy near player 

void battle(Player *player, Enemy *enemy); // this function will track the battel betwwen player and enemy and this will only display the end reasult 

void use_potion(Player *player); // potion tracker 

int main() {
    srand(time(NULL));

    // Initializing player at (0, 0), and two enemies (skeleton and hell)
    Player player = {100, 25, 5, 0, 0, 1};  // Player with 1 potion
    Enemy skeleton = {100, 25, 1, 1, 1};    // Skeleton enemy
    Enemy hell = {100, 15, 8, 8, 1};      // hell enemy, placed randomly

    // Random skeleton placemen
    skeleton.x = rand() % WIDTH;
    skeleton.y = rand() % HEIGHT;

    // Random hell placement
    hell.x = rand() % WIDTH;
    hell.y = rand() % HEIGHT;

    // Random potion placement
    int potion_x = rand() % WIDTH;
    int potion_y = rand() % HEIGHT;

    int game_0 = 0;

    printf("Welcome to the dungeon crawler game!\n");
    printf("You need to move and defeat the enemies!\n");

    while (!game_0) {
        Display_Grid(player, skeleton, hell, potion_x, potion_y); // displaying the map of the game

        if (enemy_near(player, skeleton)) {
            printf("You encountered a skeleton!\n"); // if you get the enemy one on one the function will track it and the as the condition satisfy then the msg will be print 
            battle(&player, &skeleton); // who had encountered whom (player encountered skeleton)
            if (skeleton.alive == 0) {  // when the player defeate the monster it will through the msg 
                printf("Skeleton defeated!\n");
            }
        }

        if (enemy_near(player, hell)) {
            printf("You encountered a hell!\n");
            battle(&player, &hell);
            if (hell.alive == 0) {
                printf("hell defeated!\n");
            }
        }

        // Player picks up the potion as we get it by the coordenets
        if (player.x == potion_x && player.y == potion_y) {
            printf("You found a health potion!\n");
            player.potions++;
            potion_x = -1;  // Remove potion from the grid
            potion_y = -1;
        }

        if (player.hp <= 0) { 
            printf("You have been defeated!\n"); // end statement for the battel
            game_0 = 1;
        } else if (skeleton.alive == 0 && hell.alive == 0) {
            printf("You defeated all the enemies! You WON the battle!\n"); 
            game_0 = 1;
        } else {
            char input;
            printf("Move (W, S, A, D) or use potion (P)\n: ");
            scanf(" %c", &input);  
            if (input == 'p' || input == 'P') {
                use_potion(&player); // storing potions HP to players HP 
            } else {
                move_player(&player, input); 
            }
        }
    }

    return 0;
}

void Display_Grid(Player player, Enemy enemy1, Enemy enemy2, int potion_x, int potion_y) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (player.x == j && player.y == i) {
                printf("P ");
            } else if (enemy1.x == j && enemy1.y == i && enemy1.alive) {
                printf("S ");  // S for Skeleton
            } else if (enemy2.x == j && enemy2.y == i && enemy2.alive) {
                printf("S ");  // s for hell
            } else if (potion_x == j && potion_y == i) {
                printf(" "); // O for Potion
            } 
            else {
                printf("- ");
            }
        }
        printf("\n");
    }
}


// Joy stick for movement of the player 
void move_player(Player *player, char direction) {
    switch (direction) {
        case 'w':
        case 'W':
            if (player->y > 0) player->y--;
            break;
        case 's':
        case 'S':
            if (player->y < HEIGHT - 1) player->y++;
            break;
        case 'a':
        case 'A':
            if (player->x > 0) player->x--;
            break;
        case 'd':
        case 'D':
            if (player->x < WIDTH - 1) player->x++;
            break;
        default:
            printf("Invalid move! Use W, A, S, D for movement or P to use a potion.\n");
            break;
    }
}


// check the coordanets of teh enemy and player for the battel
int enemy_near(Player player, Enemy enemy) {
    return (player.x == enemy.x && player.y == enemy.y && enemy.alive);
}


// battel function definitation
void battle(Player *player, Enemy *enemy) {
    while (player->hp > 0 && enemy->hp > 0) {
        int enemy_damage = player->atk;
        enemy->hp -= enemy_damage;
        printf("You hit the enemy for %d damage. Enemy HP: %d\n", enemy_damage, enemy->hp);

        if (enemy->hp > 0) {
            int player_damage = enemy->atk - player->dfp;
            if (player_damage < 0) player_damage = 0;
            player->hp -= player_damage;
            printf("Enemy hits you for %d damage. Your HP is %d\n", player_damage, player->hp);
        } else {
            enemy->alive = 0;
        }
    }
}

// 
void use_potion(Player *player) {
    if (player->potions > 0) {
        player->hp += 15;  // Restore 15 health points
        player->potions--; // Decrease potion count
        printf("You used a potion! Restored 15 health points. Your HP: %d\n", player->hp);
    } else {
        printf("No potions left!\n");
    }
}
