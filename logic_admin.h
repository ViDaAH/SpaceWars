#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

class logic_admin
{
private:
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_BITMAP *fondo = al_load_bitmap("recursos/juego/fondo.png"); // Imagen para el fondo del juego
    ALLEGRO_BITMAP *g_o = al_load_bitmap("recursos/juego/lost.png");    // Imagen para la pantalla al perder el juego
    ALLEGRO_BITMAP *won = al_load_bitmap("recursos/juego/won.png");     // Imagen para la pantalla al finalizar el juego (completarlo)
    ALLEGRO_FONT *font;
    int nivel = 0, vida = 0;
    int cont_e;
    bool enter = false;
    bool win, game_over;
    // VARIABLES JUGADOR
    ALLEGRO_BITMAP *jugador = al_load_bitmap("recursos/juego/jugador/player1.png"); // Imagen de la nave del Jugador
    double posx = 1920 / 2 - 77, posy = 1000;                                       // Variable para manejar la Posici�n del Jugador en pantalla (variar� seg�n el momento e instrucciones, la posici�n defininda inicialmente es para cuando el juego inicial)
    double posx_min = 0, posx_max = 1843;                                           // L�mite (Muro invisible) de movimiento del jugador <<en este caso, en el eje X>>
    double posy_min = 1000, posy_max = 700;                                         // L�mite (Muro invisible) de movimiento del jugador <<en este caso, en el eje Y>>
    int mov = 0;                                                                    // Variable para regular el movimiento del jugador

    // VARIABLES DISPAROS
    ALLEGRO_BITMAP *disparo_j = al_load_bitmap("recursos/juego/shot.png"); // Imagen del disparo del jugador
    bool disparo_activo = false;                                           // Variables para regular el retraso entre los disparos del jugador
    int d = 0;                                                             // Variable para regular que los disparos realizados por ronda sean equivalentes a los regulados por "cantdm"
    int posx_d[5] = {-100, -100, -100, -100, -100};                        // Disparo en x
    int posy_d[5] = {-100, -100, -100, -100, -100};                        // Disparo en y
    int cantdm = 0;                                                        // Cantidad de disparos m�ximos (se definir� seg�n el nivel en que se est�
    int contador;                                                          // Variable para manejar los disparos totales realizados por el jugador

    // VARIABLES ENEMIGOS
    ALLEGRO_BITMAP *enemigos[3] = {al_load_bitmap("recursos/juego/enemigos/1_1.png"), // Imagen de naves enemigas [0]: nivel 1, [1]: nivel 2, [2]: nivel 3
                                   al_load_bitmap("recursos/juego/enemigos/2_2.png"),
                                   al_load_bitmap("recursos/juego/enemigos/3_3.png")};

    double posx_n_e[5];                                                      // Nave en x
    double posy_n_e[5];                                                      // Nave en y
    int posx_d_e[5];                                                         // Disparo en x
    int posy_d_e[5];                                                         // Disparo en y
    int ancho_x = 75, alto_y = 75;                                           // Tama�o en x, y. De las naves enemigas
    int cont_enemigos = 0, vida_e[5], m_e;                                   // Vida de las 5 naves que se deber�n representar en cada "ronda"
    double mov_ex[5], mov_ey[5];                                             // Movimiento inicial de las naves enemigas mientra "inic" est� activa y ascx/ascy/dscx/dscy est�n desactivadas
    bool ascx[5], dscx[5], ascy[5], dscy[5], inic;                           // Movimiento de las naves dependiendo de la posici�n de las naves enemigos respecto a los bordes
    ALLEGRO_BITMAP *disparo_e = al_load_bitmap("recursos/juego/shot_e.png"); // Imagen de Disparo para naves enemigas

public:
    // Manejo y reestablecimiento de las variables para el juego (dentro de la ejecuci�n de este)

    logic_admin(int vida, int nivel, int cont_e, bool enter, bool win, bool game_over, ALLEGRO_FONT *font)
    {
        this->vida = vida;
        this->nivel = nivel;
        this->cont_e = cont_e;
        this->enter = enter;
        this->win = win;
        this->game_over = game_over;
        this->font = font;
    }

    void preparar()
    {
        game_over = false;
        win = false;
        enter = false;
        switch (nivel)
        {
        case 1:
            cont_e = 40;
            m_e = 8;
            cantdm = 5;
            mov = 1;
            contador = 100;
            break;
        case 2:
            cont_e = 45;
            mov = 1;
            m_e = 9;
            cantdm = 4;
            contador = 100;
            break;
        case 3:
            cont_e = 50;
            mov = 1;
            m_e = 10;
            cantdm = 3;
            contador = 100;
            break;
        }
        for (int i = 0; i < 5; i += 1)
        {
            inic = true;
            mov_ex[i] = 1;
            mov_ey[i] = 1;
            posx_d[i] = -100;
            posy_d[i] = -100;
            posx_d_e[i] = -1000;
            posy_d_e[i] = -1000;
            vida_e[i] = 100;
            posx_n_e[i] = 960;
            posy_n_e[i] = 200;
        }
    }

    // Manejo de movimientos del jugador y sus disparos (en base a "mov" y a "cantdm")
    void player()
    {
        for (int e = 0; e <= 4; e += 1)
        {
            if ((posx_d_e[e] >= posx && posx_d_e[e] <= posx + 77) &&
                (posy_d_e[e] >= posy && posy_d_e[e] <= posy + 77))
            {
                posy_d_e[e] = -1100;
                vida -= 25;
            }
        }
        if (vida == 0)
        {
            game_over = true;
        }
        // Movimiento JUGADOR
        if (!(al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) && !(al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) &&
            !al_key_down(&keyboard_state, ALLEGRO_KEY_UP) && !al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN))
        {
            al_draw_bitmap(jugador, posx, posy, 0);
        }
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT) && posx <= posx_max)
        {
            al_draw_bitmap(jugador, posx += mov, posy, 0);
        }
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT) && posx >= posx_min)
        {
            al_draw_bitmap(jugador, posx -= mov, posy, 0);
        }

        if (al_key_down(&keyboard_state, ALLEGRO_KEY_UP) && posy >= posy_max)
        {
            al_draw_bitmap(jugador, posx, posy -= mov, 0);
        }
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN) && posy <= posy_min)
        {
            al_draw_bitmap(jugador, posx, posy += mov, 0);
        }

        // DISPARO
        if (d != cantdm)
        {

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && !disparo_activo)
            {
                posx_d[d] = posx + 37;
                posy_d[d] = posy - 66;

                d += 1;
                disparo_activo = true;
                contador -= 1;
            }
            if (contador > 0 && win == false)
            {
                if (!al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE))
                {
                    disparo_activo = false;
                }
            }
        }

        if (d > 0)
        {

            for (int c = 0; c < d; c++)
            {
                if (posy_d[c] > -66)
                {
                    al_draw_bitmap(disparo_j, posx_d[c], posy_d[c] -= 2, 1);
                }
            }
            if (d == cantdm && posy_d[cantdm - 1] < -60)
            {
                d = 0;
            }
        }
        if (contador == 0 && win == false)
        {
            game_over = true;
        }
    }

    // Proceso para Finalizaci�n del Nivel en base a variables "game_over" o "win"
    string g_over()
    {
        // Si se ha perdido el juego (por gastar las municiones o por vida=0
        if (game_over == true)
        {
            // Se establecen los valores de vida y nivel en 0 (dentro del archivo) para que en menu() se reestablezcan nuevamente a 1, 500
            ofstream archivo_e;
            archivo_e.open("recursos/informacion.txt", ios::out);
            archivo_e << "0"
                      << " "
                      << "0" << endl;

            // Mostrar imagen de "GAME OVER" por 5 segundos
            for (int e = 0; e <= 4; e++)
            {
                al_draw_bitmap(g_o, 0, 0, 0);
                al_flip_display();
                al_rest(1);
            }
            nivel = 0; // Se establece Nivel en 0 debido a que en contadores no se vuelve a leer el archivo por lo que, al evaluar en que nivel se est�, requiere una re definici�n. En este caso, nivel=0;
            return "game_over";
        }

        // Si se ha ganado el juego (nivel)
        if (win == true)
        {
            // Variable para reescribir el archivo seg�n el punto y progreso actual
            ofstream archivo_e_n;

            // evaluando seg�n nivel
            switch (nivel)
            {
            case 1:
                // Si el nivel es 1 y se ha ganado. Se reescribe el archivo ahora con NIVEL=2 y la VIDA=(vida que se tenga actualmente)
                archivo_e_n.open("recursos/informacion.txt", ios::out);
                archivo_e_n << "2"
                            << " "
                            << vida << endl;
                nivel = 2; // Misma l�gica que cuando se pierde, se establece el nivel en 0; en este caso: Nivel=2;
                break;

            case 2:
                archivo_e_n.open("recursos/informacion.txt", ios::out);
                archivo_e_n << "3"
                            << " " << vida << endl;
                nivel = 3;
                break;

            case 3:
                // SI SE HA COMPLETADO EL JUEGO (NIVEL 3)
                // Establecer el nivel en 1 (ya que no hay m�s) e igual con vida=500
                archivo_e_n.open("recursos/informacion.txt", ios::out);
                archivo_e_n << "1"
                            << " " << 500 << endl;
                // Mostrar imagen "FELICIDADES, HAS FINALIZADO EL JUEGO" por 7 Segundos
                for (int e = 0; e <= 6; e++)
                {
                    al_draw_bitmap(won, 0, 0, 0);
                    al_flip_display();
                    al_rest(1);
                }
                // Se escribe 0 el nivel para que, al momento de regresar a "contadores()" se muestre el contador neutral
                nivel = 0;
                break;
            }
            return "won"; // Regresar a contadores
        }
    }

    // Manejo de movimientos, vida y disparos de las naves enemigas
    void enemigo()
    {
        for (int e = 0; e <= 4; e += 1)
        {
            if (inic == true)
            {
                for (int f = 0; f <= 4; f += 1)
                {
                    vida_e[f] = 100;
                    if (rand() % 2 == 1)
                    {
                        posx_n_e[f] = 960 + rand() % 500;
                        posy_n_e[f] = 200 + rand() % 200;
                    }
                    else
                    {
                        posx_n_e[f] = 960 + rand() % 500 - 500;
                        posy_n_e[f] = 200 + rand() % 200 - 200;
                    }

                    if ((rand() % 2) == 1)
                    {
                        mov_ex[f] *= -1;
                    }
                    if ((rand() % 2) == 0)
                    {
                        mov_ey[e] *= -1;
                    }
                    if (e > 0 && mov_ey[e] == -0.5 && mov_ey[e - 1] == -0.5)
                    {
                        mov_ey[e] *= -1;
                    }
                    if (e > 0 && mov_ex[e] == -0.5 && mov_ex[e - 1] == -0.5)
                    {
                        mov_ex[e] *= -1;
                    }
                }
                inic = false;
            }

            // Movimiento de las naves enemigas en el eje x

            if (posx_n_e[e] <= 1850 && posx_n_e[e] >= 10)
            {
                if (ascx[e] == false && dscx[e] == false)
                {
                    posx_n_e[e] += mov_ex[e];
                }

                if (posx_n_e[e] == 1850)
                {
                    ascx[e] = false;
                    dscx[e] = true;
                }
                else if (posx_n_e[e] == 10)
                {
                    dscx[e] = false;
                    ascx[e] = true;
                }

                if (ascx[e])
                {
                    posx_n_e[e] += 0.5;
                }
                else if (dscx[e])
                {
                    posx_n_e[e] -= 0.5;
                }
            }

            // Movimiento de las naves enemigas en el eje y
            if (posy_n_e[e] <= 400 && posy_n_e[e] >= 10)
            {

                if (ascy[e] == false && dscy[e] == false)
                {
                    posy_n_e[e] += mov_ey[e];
                }

                if (posy_n_e[e] == 400)
                {
                    ascy[e] = false;
                    dscy[e] = true;
                }
                else if (posy_n_e[e] == 10)
                {
                    dscy[e] = false;
                    ascy[e] = true;
                }

                if (ascy[e])
                {
                    posy_n_e[e] += 0.5;
                }
                else if (dscy[e])
                {
                    posy_n_e[e] -= 0.5;
                }
            }

            for (int i = 0; i <= 4; i += 1)
            {
                if (i != e && ((posx_n_e[i] == posx_n_e[e] && posy_n_e[i] == posy_n_e[e]) ||
                               ((((1850 - posx_n_e[i]) - (1850 - posx_n_e[e])) <= 100) ||
                                (((1850 - posx_n_e[i]) + (1850 - posx_n_e[e])) <= 100) ||
                                (((400 - posy_n_e[i]) - (400 - posy_n_e[e])) <= 100) ||
                                (((400 - posy_n_e[i]) + (400 - posy_n_e[e])) <= 100))))
                {
                    if (ascx[i] == true)
                    {
                        ascx[i] = false;
                        dscx[i] = true;
                    }
                    else
                    {
                        ascx[i] = true;
                        dscx[i] = false;
                    }

                    if (ascy[i] == true)
                    {
                        ascy[i] = false;
                        dscy[i] = true;
                    }
                    else
                    {
                        ascy[i] = true;
                        dscy[i] = false;
                    }
                }
                for (int u = 0; u <= 4; u += 1)
                {
                    if ((posx_d[u] >= posx_n_e[i] && posx_d[u] <= posx_n_e[i] + 75) &&
                        (posy_d[u] >= posy_n_e[i] && posy_d[u] <= posy_n_e[i] + 75))
                    {
                        posx_d[u] = -150;
                        posy_d[u] = -150;
                        posy_n_e[i] = -500;
                        posx_n_e[i] = -500;
                        vida_e[i] = 0;
                        cont_enemigos += 1;
                        cont_e -= 1;
                        if (cont_enemigos == 5)
                        {
                            cont_enemigos = 0;
                            m_e -= 1;

                            if (m_e == 0)
                            {
                                win = true;
                            }
                            else
                            {
                                inic = true;
                            }
                        }
                    }
                }
            }

            if (vida_e[e] == 100)
            {
                al_draw_bitmap(enemigos[nivel - 1], posx_n_e[e], posy_n_e[e], 0);
            }
            if (vida_e[e] == 100 && posy_d_e[e] == -1100)
            {
                posx_d_e[e] = posx_n_e[e] + 35;
                posy_d_e[e] = posy_n_e[e] + 75;
            }
            if (posy_d_e[e] <= 1100 && posy_d_e[e] > 10)
            {
                al_draw_bitmap(disparo_e, posx_d_e[e], posy_d_e[e] += 2, 0);
            }
            else
            {
                posy_d_e[e] = -1100;
            }
        }
    }

    // Muestra todas los elementos en pantalla
    void mostrar()
    {
        // Si no se ha ganado o perdido mostrar enemigos, jugador, etc. En pantalla
        if (win == false && game_over == false)
        {
            al_draw_bitmap(fondo, 0, 0, 0);         // Dibujar el fondo en pantalla
            al_get_keyboard_state(&keyboard_state); // Obtenci�n de l estado del teclado (que tecla se est� presionando)
            player();
            enemigo();
            // Mostrar contadores de vida, enemigos restantes y disparos restantes (jugador)
            al_draw_text(font,
                         al_map_rgb(100, 255, 177),
                         0,
                         0,
                         NULL,
                         ("Disparos Restantes " + to_string(contador)).c_str());
            al_draw_text(font,
                         al_map_rgb(255, 77, 111),
                         0,
                         30,
                         NULL,
                         ("Enemigos Restantes " + to_string(cont_e)).c_str());
            al_draw_text(font, al_map_rgb(64, 224, 208), 0, 60, NULL, ("Vida " + to_string(vida)).c_str());
        }
        // Si se ha ganado o perdido, Llamar a "g_over()" donde se ejecutar� seg�n el caso, las instrucciones necesarias
        else if (win == true || game_over == true)
        {
            g_over();
        }
        // Actualizaci�n de pantalla
        al_flip_display();
    }
};
