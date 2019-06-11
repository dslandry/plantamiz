#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// constants
#define N_COL 40
#define N_LINES 20
#define N_SYMBOLS 5

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32

int contrat1[]={12, 12, 12, 12, 12, 20}; // définir le contrat à réaliser ici

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


long int tab_points[]={0,0,0,0,0};
int vies = 5;
long int n_coups=0;
long int n_coups_ref =5;
int contrat_actuel[N_SYMBOLS+1];
int is_in_initialisation_state=1;

char tab_jeu[N_LINES][N_COL];

struct Position position_joueur= {0, 0, 0};


int is_tab_identical(char *tab, int size_tab){
    int i;
    for(i=0; i<size_tab ;i++){
        if(i<size_tab-1){
            if(tab[i+1]!=tab[i]){
                return 0;
            }
        }
    }
    return 1;
}

void init_position_joueur(){
    position_joueur.i=0;
    position_joueur.j=0;
    position_joueur.i=0;
    position_joueur.j=0;
    position_joueur.selected=0;
    n_coups=0;
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
    int n = (rand() % (4 - 0 + 1)) + 0;
    //int n = rand() % 4 + 0;
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
    for(i=0; i<N_LINES; i++){
        for(j=0;j<N_COL; j++){
            printf("%c ",tab_jeu[i][j]);
        }
        printf("\n");
    }
}

void afficher_menu_perte_vie(){
    system("cls");
    printf("Vous avez perdu une vie!\n");
    system("pause");
    system("cls");
}

void afficher_menu_reussite_contrat(){
     system("cls");
    printf("Vous avez gagné!\n");
    system("pause");
    system("cls");
}

void reafficher_tableau(){
    system("cls");
    afficher_tableau_jeu();
    update_position_joueur(position_joueur.i, position_joueur.j);
    gotoxy(0, N_LINES+1);
    afficher_menu();
}

void reset_points(){
    int i;
    for (i=0; i<N_SYMBOLS; i++){
        tab_points[i]=0;
    }
}

void augmenter_points(char symbol){
    int i;
    for(i=0 ; i< N_SYMBOLS; i++){
        if(tab_items[i]==symbol){
            tab_points[i]++;
            break;
        }
    }
}

void prise_en_charge_contrat(int* contrat){
    int i;
    for(i=0; i<N_SYMBOLS+1; i++){
        contrat_actuel[i]=contrat[i];
    }
    n_coups_ref = contrat_actuel[N_SYMBOLS];
}

int is_score_reussi(){
    int i;
    for(i=0; i<N_SYMBOLS; i++){
        if(tab_points[i]<contrat_actuel[i]){
            return 0;
        }
    }
    return 1;
}


void delete_symbol_from_tableau(char symbol){
    int i, j;
    for(i=0; i<N_LINES; i++){
        for(j=0; j<N_COL; j++){
            if(tab_jeu[i][j]==symbol){
                tab_jeu[i][j]='*';
                augmenter_points(symbol);
            }
        }
    }
}

void apply_gravity_to_column(char* column){
    //the column is in the form of an array selected from bottom to top
    int i;
    char gravity_applied[N_LINES];
    int length_true_elements=0;
    for(i=0 ; i<N_LINES ;i++ ){
        if(column[i]!='*'){
            gravity_applied[length_true_elements]=column[i];
            length_true_elements++;
        }
    }
    // fill the remaining spaces on the array

    for(int i=length_true_elements; i<N_LINES; i++){
        gravity_applied[i]= '*';
    }

    // fill the remaining spaces with random elements

    for(i=0;i<N_LINES;i++){
        if(gravity_applied[i]=='*'){
            gravity_applied[i]= get_random_item();
        }
    }


    // fill the column in the parametetr with the filled array of gravity applied
    for(i=0;i<N_LINES;i++){
        column[i]=gravity_applied[i];
    }

}
void apply_gravity(){
    int i, j;
    char column[N_LINES];
    for(j=0; j<N_COL; j++){
            // collect one column at a time
        for(i=N_LINES-1; i>=0; i--){
            column[N_LINES-1-i]=tab_jeu[i][j];
        }

        apply_gravity_to_column(column);

        // replace the column on the actual grid

        for(i=0; i<N_LINES; i++){
            tab_jeu[N_LINES-1-i][j]= column[i];
        }

    }


}

void manger_combinaisons(){
    int i, j ,k ;
    int tableau_a_bouge= 0;
    char symbol;
    char fenetre[5];
    int passages=0;

    do{
        tableau_a_bouge=0;

        /** Recherche des combinaisons 5 par 5**/
        // vertical
        for(j=0; j<N_COL;j++){
           for(i=0; i<N_LINES-5; i++){
                symbol= tab_jeu[i][j];
                for(k=0; k<5; k++){
                    fenetre[k] = tab_jeu[i+k][j];
                }
                if(is_tab_identical(fenetre, 5)){
                   delete_symbol_from_tableau(symbol);
                   tableau_a_bouge=1;
                }
           }
        }

        //horizontal
        for(i=0; i<N_LINES;i++){
           for(j=0; j<N_COL-5; j++){
                symbol= tab_jeu[i][j];
                for(k=0; k<5; k++){
                    fenetre[k] = tab_jeu[i][j+k];
                }
                if(is_tab_identical(fenetre, 5)){
                   delete_symbol_from_tableau(symbol);
                   tableau_a_bouge=1;
                }
           }
        }

        /** Recherche des combinaisons 4 par 4**/

         // vertical
        for(j=0; j<N_COL;j++){
           for(i=0; i<N_LINES-4; i++){
                symbol= tab_jeu[i][j];
                for(k=0; k<4; k++){
                    fenetre[k] = tab_jeu[i+k][j];
                }
                if(is_tab_identical(fenetre, 4)){
                    for(k=0; k<4; k++){
                        tab_jeu[i+k][j]='*';
                        augmenter_points(symbol);
                    }
                   tableau_a_bouge=1;
                }
           }
        }

        //horizontal
        for(i=0; i<N_LINES;i++){
           for(j=0; j<N_COL-4; j++){
                symbol= tab_jeu[i][j];
                for(k=0; k<4; k++){
                    fenetre[k] = tab_jeu[i][j+k];
                }
                if(is_tab_identical(fenetre, 4)){
                    for(k=0; k<4; k++){
                        tab_jeu[i+k][j]='*';
                        augmenter_points(symbol);
                    }

                   tableau_a_bouge=1;
                }
           }
        }

        /** Recherche des combinaisons 3 par 3 **/

         // vertical
        for(j=0; j<N_COL;j++){
           for(i=0; i<N_LINES-3; i++){
                symbol= tab_jeu[i][j];
                for(k=0; k<3; k++){
                    fenetre[k] = tab_jeu[i+k][j];
                }
                if(is_tab_identical(fenetre, 3)){
                    for(k=0; k<3; k++){
                        tab_jeu[i+k][j]='*';
                        augmenter_points(symbol);
                    }
                   tableau_a_bouge=1;
                }
           }
        }

        //horizontal
        for(i=0; i<N_LINES;i++){
           for(j=0; j<N_COL-3; j++){
                symbol= tab_jeu[i][j];
                for(k=0; k<3; k++){
                    fenetre[k] = tab_jeu[i][j+k];
                }
                if(is_tab_identical(fenetre, 3)){
                    for(k=0; k<3; k++){
                        tab_jeu[i+k][j]='*';
                        augmenter_points(symbol);
                    }

                   tableau_a_bouge=1;
                }
           }
        }

        if(tableau_a_bouge==1){
            apply_gravity();
            if(is_in_initialisation_state){
                if(passages==0){
                    system("cls");
                    printf("********INITIALISATION*********");
                }
            }
            else{
               reafficher_tableau();
            }
        }
        passages++;
        if(passages>2){
            passages=2;
        }
    }while(tableau_a_bouge==1);
}
void move_up(){
    char temp;
    if(position_joueur.selected==0){
        remove_position_joueur(position_joueur.i, position_joueur.j);
        if(position_joueur.i>0){
            position_joueur.i--;
        }
        update_position_joueur(position_joueur.i, position_joueur.j);
    }
    else{

         if(position_joueur.i>0){
            n_coups++;
            position_joueur.selected=0;
            temp=tab_jeu[position_joueur.i-1][position_joueur.j];
            tab_jeu[position_joueur.i-1][position_joueur.j]=tab_jeu[position_joueur.i][position_joueur.j];
            tab_jeu[position_joueur.i][position_joueur.j]=temp;
            position_joueur.i--;
            manger_combinaisons();
            reafficher_tableau();
        }
    }
}
void move_down(){
    char temp;
    if(position_joueur.selected==0){
        remove_position_joueur(position_joueur.i, position_joueur.j);
        if(position_joueur.i<N_LINES-1){
            position_joueur.i++;
        }
        update_position_joueur(position_joueur.i, position_joueur.j);
    }
    else{

        if(position_joueur.i<N_LINES-1){
            n_coups++;
            position_joueur.selected=0;
            temp=tab_jeu[position_joueur.i+1][position_joueur.j];
            tab_jeu[position_joueur.i+1][position_joueur.j]=tab_jeu[position_joueur.i][position_joueur.j];
            tab_jeu[position_joueur.i][position_joueur.j]=temp;
            position_joueur.i++;
            manger_combinaisons();
            reafficher_tableau();
        }
    }
}
void move_left(){
    char temp;
    if(position_joueur.selected==0){
        remove_position_joueur(position_joueur.i, position_joueur.j);
        if(position_joueur.j>0){
            position_joueur.j--;
        }
        update_position_joueur(position_joueur.i, position_joueur.j);
    }
    else{

        if(position_joueur.j>0){
            n_coups++;
            position_joueur.selected=0;
            temp=tab_jeu[position_joueur.i][position_joueur.j-1];
            tab_jeu[position_joueur.i][position_joueur.j-1]=tab_jeu[position_joueur.i][position_joueur.j];
            tab_jeu[position_joueur.i][position_joueur.j]=temp;
            position_joueur.j--;
            manger_combinaisons();
            reafficher_tableau();
        }
    }
}
void move_right(){
    char temp;
    if(position_joueur.selected==0){
        remove_position_joueur(position_joueur.i, position_joueur.j);
        if(position_joueur.j<N_COL-1){
            position_joueur.j++;
        }
        update_position_joueur(position_joueur.i, position_joueur.j);
    }
    else{

        if(position_joueur.j<N_COL-1){
            n_coups++;
            position_joueur.selected=0;
            temp=tab_jeu[position_joueur.i][position_joueur.j+1];
            tab_jeu[position_joueur.i][position_joueur.j+1]=tab_jeu[position_joueur.i][position_joueur.j];
            tab_jeu[position_joueur.i][position_joueur.j]=temp;
            position_joueur.j++;
            manger_combinaisons();
            reafficher_tableau();
        }
    }
}

void afficher_menu(){
    int i;
    printf("VIES : %d\n", vies);
    printf("POINTS : \n");
    printf("\tSoleil :%d \n", tab_points[0]);
    printf("\tFraise :%d \n", tab_points[1]);
    printf("\tPomme :%d \n",  tab_points[2]);
    printf("\tOnion :%d \n", tab_points[3]);
    printf("\tMandarine :%d \n",  tab_points[4]);
    printf("COUPS EFFECTUES : %d", n_coups);
    printf("\t\t\t\t");
    printf("CONTRAT: ");
    for(i=0; i<N_SYMBOLS; i++){
        printf("%d %c  ",contrat_actuel[i], tab_items[i]);
    }
    printf("  EN %d COUPS", contrat_actuel[N_SYMBOLS]);
}

void sauvegarder(){
    FILE *f = fopen("sauvegarde.txt", "a");
    char nom_joueur[20];
    system("cls");
    printf("SAUVEGARDE\n");
    printf("Entrez votre nom : ");
    scanf("%s", nom_joueur);
    fprintf(f, "%s", "**********************************************************************************\n\n");
    fprintf(f, "%s", "JOUEUR : ");
    fprintf(f, "%s", nom_joueur);
    fprintf(f, "%s", "\n");
    fprintf(f, "%s", "POINTS : \n");
    fprintf(f, "Soleil :%d \t", tab_points[0]);
    fprintf(f, "Fraise :%d \t", tab_points[1]);
    fprintf(f, "Pomme :%d \t",  tab_points[2]);
    fprintf(f, "Onion :%d \t", tab_points[3]);
    fprintf(f, "Mandarine :%d",  tab_points[4]);
     fprintf(f, "%s", "\n");
     fprintf(f, "%s", "COUPS : ");
    fprintf(f, "%s", n_coups);
    fprintf(f, "%s", "\n\n");
    fclose(f);
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
    is_in_initialisation_state=1;
    system("color 0F");
    srand(time(NULL));
    int keyboard_press;

    /** initialisations **/

    init_tableau_jeu();
    init_position_joueur();

    /** donner le contrat à prendre en charge pour cette partie **/
    prise_en_charge_contrat(contrat1);

     /** premier affichage du tableau **/
    afficher_tableau_jeu();
    update_position_joueur(position_joueur.i, position_joueur.j);


    /** élimination des premières combinaisons **/

    manger_combinaisons();

    /** Reset des points **/

    reset_points();
    reafficher_tableau();
    is_in_initialisation_state=0;
    while(1){
        gotoxy(0, N_LINES+1); //déplacement du curseur pour que le curseur ne se retrouve pas dans la grille
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

        if(n_coups>= n_coups_ref){
            vies--;
            if(vies==0){
                sauvegarder();
                return 0;
            }
            else{
                if(is_score_reussi()){
                    afficher_menu_reussite_contrat();
                    sauvegarder();
                    return 0;
                }
                else{
                    afficher_menu_perte_vie();
                    main();
                }
            }

        }
    }
    return 0;
}



/** FONCTIONS RÉCUPÉRÉES SUR INTERNET POUR LE BON FONCTIONNEMENT DE L'INTERFACE **/

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


