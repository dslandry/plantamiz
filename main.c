#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// constants
#define N_COL 40
#define N_LINES 20
#define N_SYMBOLS 5

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32

void SetColor(int ForgC);
void SetColorAndBackground(int ForgC, int BackC);           //function declaration
void gotoxy(int x, int y);



typedef struct Position
{
    int i; // line
    int j; // column
    int selected;// boolean variable 0 : unselected , 1 selected
};

char tab_items[]= {'S', 'F', 'P', 'O', 'M'}; //5 items
char tab_items_selectionne[]={'s', 'f', 'p', 'o', 'm' };

char tab_jeu[N_LINES][N_COL];

struct Position position_joueur;

void init_position_joueur(){
    position_joueur.i=0;
    position_joueur.j=0;
    position_joueur.selected=0;
}

void mettre_couleur_defaut(){
    SetColorAndBackground(255, 0);
}
void mettre_couleur_position_joueur(){
     SetColorAndBackground(34, 0);
}
void mettre_couleur_selectionne(){
    SetColorAndBackground(34, 255);
}

char get_random_item(){
    int n = rand() % 4 + 0;
    return tab_items[n];
}

void init_tableau_jeu(){
    int i, j;
    for(i=0; i<N_LINES; i++){
        for(j=0;j<N_COL; j++){
            tab_jeu[i][j]= get_random_item();
        }

    }
}

void afficher_tableau_jeu(){
    int i, j;
    /*
    for(i=0; i<N_LINES; i++){
        for(j=0;j<N_COL; j++){
            if(position_joueur.i==i && position_joueur.j==j){
                mettre_couleur_position_joueur();
                printf("%c ", tab_jeu[i][j]);
                mettre_couleur_defaut();

            }
            else{
                printf("%c ",tab_jeu[i][j]);
            }
        }
        printf("\n");
    }
    */
    for(i=0; i<N_LINES; i++){
        for(j=0;j<N_COL; j++){
            printf("%c ",tab_jeu[i][j]);
        }
        printf("\n");
    }
}





void move_up(){
    remove_position_joueur(position_joueur.i, position_joueur.j);
    if(position_joueur.i>0){
        position_joueur.i--;
    }
    update_position_joueur(position_joueur.i, position_joueur.j);
}
void move_down(){
    remove_position_joueur(position_joueur.i, position_joueur.j);
    if(position_joueur.i<N_LINES-1){
        position_joueur.i++;
    }
    update_position_joueur(position_joueur.i, position_joueur.j);
}
void move_left(){
    remove_position_joueur(position_joueur.i, position_joueur.j);
    if(position_joueur.j>0){
        position_joueur.j--;
    }
    update_position_joueur(position_joueur.i, position_joueur.j);
}
void move_right(){
    remove_position_joueur(position_joueur.i, position_joueur.j);
    if(position_joueur.j<N_COL-1){
        position_joueur.j++;
    }
    update_position_joueur(position_joueur.i, position_joueur.j);
}
void select_or_unselect(){
    remove_position_joueur(position_joueur.i, position_joueur.j);
    if(position_joueur.selected==0){
        position_joueur.selected=1;
        mettre_couleur_selectionne();
    }
    else{
        position_joueur.selected=0;
        mettre_couleur_position_joueur();
    }
    gotoxy(position_joueur.j*2, position_joueur.i);
    printf("%c",tab_jeu[position_joueur.i][position_joueur.j] );
    mettre_couleur_defaut();
}

void remove_position_joueur(int i, int j){
    gotoxy(j*2, i);
    mettre_couleur_defaut();
    printf("%c", tab_jeu[i][j]);
}

void update_position_joueur(int i, int j){
    gotoxy(j*2, i);
    mettre_couleur_position_joueur();
    printf("%c", tab_jeu[i][j]);
    mettre_couleur_defaut();
}
int main()
{
    int keyboard_press;

    /** initialisations **/

    init_tableau_jeu();
    init_position_joueur();


    afficher_tableau_jeu();

    update_position_joueur(position_joueur.i, position_joueur.j);

    while(1){
        gotoxy(0, N_LINES+4); // pour que le curseur ne se retrouve pas dans la grille
        keyboard_press=getch();
        switch(keyboard_press){
            case KEY_SPACE:
                select_or_unselect();
                break;
            case KEY_UP:
                // UP
                move_up();
                break;
            case KEY_DOWN:
                //DOWN
                move_down();
                break;
            case KEY_LEFT:
                //LEFTs
                move_left();
                break;
            case KEY_RIGHT:
                //RIGHT
                move_right();
                break;
        }
    }
}



void SetColor(int ForgC)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

void SetColorAndBackground(int ForgC, int BackC)
{
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


