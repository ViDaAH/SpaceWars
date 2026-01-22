#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "logic_admin.h"
#include "SharedStates.h"
#include "userGameData.h"

using namespace std;

SharedStates *sharedStates = new SharedStates();
UserGameData userGameData;
int main();
void menu();
void juego();
void contadores();
ALLEGRO_DISPLAY *ventana;
ALLEGRO_KEYBOARD_STATE keyboard_state;
ALLEGRO_MOUSE_STATE mouse_state;
ALLEGRO_FONT *font;
int cont_e;
bool enter = false;

int jugar_x = 1485;
int jugar_y = 870;
int botonj_w = 195;
int botonj_h = 95;
int salir_x = 1570;
int salir_y = 985;
int botons_w = 195;
int botons_h = 95;
int x = 0, y = 0;
ALLEGRO_BITMAP *menu_imagen[3];
ALLEGRO_BITMAP *Cont_init1[4];
ALLEGRO_BITMAP *Cont_init2[4];
ALLEGRO_BITMAP *Cont_init3[4];
ALLEGRO_BITMAP *Cont[3];
ALLEGRO_BITMAP **cont_Init;
int cont_en;
bool win, game_over;

int main()
{
    al_init();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();

    UserGameData userGameData; // Registra nivel y vida del jugador

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    ventana = al_create_display(1920, 1080);
    al_set_window_title(ventana, "SpaceWars");
    srand(time(NULL));

    menu_imagen[0] = al_load_bitmap("recursos/menu/Menu_SpaceWars.png");
    menu_imagen[1] = al_load_bitmap("recursos/menu/Menu_SpaceWars_Jugar.png");
    menu_imagen[2] = al_load_bitmap("recursos/menu/Menu_SpaceWars_Salir.png");

    // DECLARACION DE VARIABLES PARA IMAGENES DE CONTADOR INICIAL, NIVEL 1
    Cont_init1[0] = al_load_bitmap("recursos/Contadores/Inicial/nivel1/1.png");
    Cont_init1[1] = al_load_bitmap("recursos/Contadores/Inicial/nivel1/2.png");
    Cont_init1[2] = al_load_bitmap("recursos/Contadores/Inicial/nivel1/3.png");
    Cont_init1[3] = al_load_bitmap("recursos/Contadores/Inicial/nivel1/1_1.png");

    // DECLARACI�N DE VARIABLES PARA IM�GENES DE CONTADOR INICIAL, NIVEL 2
    Cont_init2[0] = al_load_bitmap("recursos/Contadores/Inicial/nivel2/1.png");
    Cont_init2[1] = al_load_bitmap("recursos/Contadores/Inicial/nivel2/2.png");
    Cont_init2[2] = al_load_bitmap("recursos/Contadores/Inicial/nivel2/3.png");
    Cont_init2[3] = al_load_bitmap("recursos/Contadores/Inicial/nivel2/2_2.png");

    // DECLARACI�N DE VARIABLES PARA IM�GENES DE CONTADOR INICIAL, NIVEL 3
    Cont_init3[0] = al_load_bitmap("recursos/Contadores/Inicial/nivel3/1.png");
    Cont_init3[1] = al_load_bitmap("recursos/Contadores/Inicial/nivel3/2.png");
    Cont_init3[2] = al_load_bitmap("recursos/Contadores/Inicial/nivel3/3.png");
    Cont_init3[3] = al_load_bitmap("recursos/Contadores/Inicial/nivel3/3_3.png");

    // DECLARACI�N DE VARIABLES PARA IM�GENES DE CONTADOR GENERAL, PAUSA-REANUDACI�N
    Cont[0] = al_load_bitmap("recursos/Contadores/gen/1.png");
    Cont[1] = al_load_bitmap("recursos/Contadores/gen/2.png");
    Cont[2] = al_load_bitmap("recursos/Contadores/gen/3.png");
    font = al_load_ttf_font("recursos/f.ttf", 22, 0);

    if (true)
    {
        menu();
    }
    return 0;
}

void menu()
{

    while (enter == false) // Mientras que No se presione uno de los botones
    {
        al_get_mouse_state(&mouse_state); // OBTENCI�N DEL PUNTO EN QUE SE ENCUENTRA EL CURSOR EN PANTALLA
        x = mouse_state.x;                // POSICI�N DEL CURSOR RESPECTO AL EJE X
        y = mouse_state.y;                // POSICI�N DEL CURSOR RESPECTO AL EJE Y

        // SI EL CURSOR EST� SOBRE EL BOT�N "SALIR", MOSTRAR IMAGEN CON "SALIR" RESALTADO
        if (x >= jugar_x && x <= jugar_x + botonj_w && y >= jugar_y && y <= jugar_y + botonj_h)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(menu_imagen[1], 0, 0, 0);
            if (al_mouse_button_down(&mouse_state, 1))
            {
                enter = true; // Salir del bucle
                contadores(); // Entrar a Contadores
            }
        }

        if (x >= salir_x && x <= salir_x + botons_w && y >= salir_y && y <= salir_y + botons_h)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(menu_imagen[2], 0, 0, 0);

            if (al_mouse_button_down(&mouse_state, 1))
            {
                exit(1);
                al_destroy_bitmap(menu_imagen[0]);
                al_destroy_bitmap(menu_imagen[1]);
                al_destroy_bitmap(menu_imagen[2]);
                al_destroy_display(ventana);
            }
        }

        // SI NO EST� EL CURSOR SOBRE LOS BOTONES, MOSTRAR LA IMAGEN EST�NDAR
        if (!(x >= salir_x && x <= salir_x + botons_w && y >= salir_y && y <= salir_y + botons_h) &&
            !(x >= jugar_x && x <= jugar_x + botonj_w && y >= jugar_y && y <= jugar_y + botonj_h))
        {
            al_draw_bitmap(menu_imagen[0], 0, 0, 0);
        }

        al_flip_display();
    }
}

void contadores()
{
    sharedStates->readUserGameData();
    userGameData = sharedStates->getUserGameData();
    switch (userGameData.nivel)
    {
    case 1:
        cont_en = 40;
        cont_Init = Cont_init1;
        break;
    case 2:
        cont_en = 45;
        cont_Init = Cont_init2;
        break;
    case 3:
        cont_en = 50;
        cont_Init = Cont_init3;
        break;
    default:
        cont_Init = Cont;
        break;
    }
    if (userGameData.nivel != 0 && userGameData.vida != 0)
    {
        for (int i = 0; i < 4; i++)
        {
            al_draw_bitmap(cont_Init[i], 0, 0, 0);
            al_draw_text(font, al_map_rgb(255, 77, 111), 1350, 950, NULL, ("Enemigos: " + to_string(cont_en)).c_str());
            al_draw_text(font, al_map_rgb(255, 77, 111), 1350, 980, NULL, ("Vida: " + to_string(userGameData.vida)).c_str());
            al_flip_display();
            al_rest(1);
        }
        juego();
    }

    if (userGameData.vida == 0 || userGameData.nivel == 0 || game_over == true)
    {
        for (int i = 0; i <= 2; i++)
        {
            al_draw_bitmap(Cont[i], 0, 0, 0);
            al_flip_display();
            al_rest(1);
        }
        menu();
    }
}

// PROCESO "PRINCIPAL" DEL JUEGO
void juego()
{
    // INICIALIZACION DE INSTANCIA PARA LA CLASE "game"
    logic_admin *admin = new logic_admin(cont_e, enter, win, game_over, font, sharedStates);

    // Llamado a proceso prepapar() de la clase game, inicializado en "j1" para limpiar valores residuales posiblemente almacenados en memoria que utilizan el jugador, los enemigos y el juego en general
    admin->preparar();

    // Variable para manejo de ejecucion del juego (para salir mediante ESC)
    bool salir = false;

    // Mientras salir=false (no se ha presionado "ESC" y nivel no sea "0" (no se perdido el juego/nivel)
    while (salir == false && userGameData.nivel >= 1)
    {
        // Llamar a "mostrar()" que visualizar� todo en pantalla (enemigos y sus disparos, fondo, jugador y sus disparos, contadores de vida, enemigos, disparos)
        admin->mostrar();

        // Si se presiona "ESC" del teclado, declarar "salir=true;" para que el bucle finalice y retorne a menu()". ESTO PUEDE SER TOMADO COMO UNA PAUSA, AUNQUE AL HACERLO, TENDR�S QUE VOLVER A JUGAR EL NIVEL ENTERO. NO SE VER�N AFECTADOS LOS DISPAROS DISPONIBLES NI LA VIDA (QUE YA SE TEN�A AL INICIAR EL NIVEL)
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE))
        {
            salir = true;
            menu();
        }
    }
}