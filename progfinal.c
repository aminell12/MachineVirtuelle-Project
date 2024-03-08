#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define N 25600
#define O 16384

typedef struct ligne{
  char * etiq;
  int code_op;
  int rd;
  int rn;
  int imm;
  int s;
  int adr;
  char * jmp;
  struct ligne * suiv;
}Ligne;

// structure représentantn tout le prgm assembleur
typedef struct code{
  Ligne * premier;
}Code_a;


/*---Fontions d'initialisation et d'affichage---*/
//foncgtion qui creer une ligne avec tous les caracteristoques d'une ligne
Ligne * creerLigne(int code_op, int rd, int rn, int imm, int s, int adr, char * etiq, char * jmp){
  Ligne * l = malloc(O*sizeof(Ligne));
  if(l == NULL) return NULL;
  l->code_op = code_op;
  l->rd = rd;
  l->rn = rn;
  l->imm = imm;
  l->s = s;
  l->adr = adr;
  l->etiq = etiq;
  l->jmp = jmp;
  return l;
}
/*-----------------------------------------*/
//fonction qui créer une structure contenant tout les lignes du prgm assembleur
Code_a * creerCode_a(){
  Code_a * c = malloc(O*sizeof(Code_a));
  if(c == NULL) return NULL;
  c->premier = NULL;
  return c;
}
/*-----------------------------------------*/
//fonction qui affiche le code contenant chaque instruction en décimal
void afficher_code(Code_a * c){
  Ligne * l = c->premier;
  while(l!= NULL){
    if(l->etiq != NULL){
      printf("\nLa ligne %d : \n L'etiquette est : '%s' \n L'opérande est : %d \n rd est le registre num : %d \n rn est le registre num : %d \n S est le nombre : %d \n Imm est : %d\n", l->adr, l->etiq, l->code_op,l->rd,l->rn, l->s, l->imm);
    }
    else{
      printf("\nLa ligne %d : \n Il n'y pas d'etiquette \n L'opérande est : %d \n rd est le registre num : %d \n rn est le registre num : %d \n S est le nombre : %d \n Imm est : %d\n", l->adr, l->code_op,l->rd,l->rn, l->s, l->imm);
    }
    l=l->suiv;
  }
  return;
}
/*---Fonctions Pour la partie Traduction et Écriture---*/
/*---------------------------------*/
// fonction qui permet renvoyer le nombre en décimal de l'opérande
int op_en_decimal(char ch[]){
  if (strcmp(ch,"and") == 0) return 0;
  if (strcmp(ch,"or") == 0) return 1;
  if (strcmp(ch,"xor") == 0) return 2;
  if (strcmp(ch,"mul") == 0) return 3;
  if (strcmp(ch,"div") == 0) return 4;
  if (strcmp(ch,"add") == 0) return 5;
  if (strcmp(ch,"sub") == 0) return 6;
  if (strcmp(ch,"shr") == 0) return 7;
  if (strcmp(ch,"ldb") == 0) return 8;
  if (strcmp(ch,"ldh") == 0) return 9;
  if (strcmp(ch,"ldw") == 0) return 10;
  if (strcmp(ch,"stb") == 0) return 11;
  if (strcmp(ch,"sth") == 0) return 12;
  if (strcmp(ch,"stw") == 0) return 13;
  if (strcmp(ch,"jmp") == 0) return 14;
  if (strcmp(ch,"jzs") == 0) return 15;
  if (strcmp(ch,"jzc") == 0) return 16;
  if (strcmp(ch,"jcs") == 0) return 17;
  if (strcmp(ch,"jcc") == 0) return 18;
  if (strcmp(ch,"jns") == 0) return 19;
  if (strcmp(ch,"jnc") == 0) return 20;
  if (strcmp(ch,"in") == 0) return 21;
  if (strcmp(ch,"out") == 0) return 22;
  if (strcmp(ch,"rnd")== 0) return 23;
  if (strcmp(ch,"hlt")== 0) return 31;
  return 200;
}
/*-----------------------------------------*/
//fonction qui ajoute une ligne a code_a
void code_ajout_ligne(Code_a * c,int code_op, int rd, int rn, int imm, int s, int adr, char * etiq, char * jmp){
  //cette fonction permet de créer une ligne avec toutes les commandes (opérande,rn,rd...) et la place dans la liste chainée Code_a, dans laquelle on va stocker toutes les lignes du fichiers tansmis
  Ligne * l = creerLigne(code_op, rd, rn, imm, s, adr, etiq, jmp);
  Ligne * l2 = c->premier;
  if (c->premier == NULL){
    c->premier = l;
    l->suiv = NULL;
  }
  else{
    while(l2->suiv != NULL){
      l2 = l2->suiv;
    }
    l2->suiv = l;
    l->suiv = NULL;
  }
  return;
}
/*---------------------------------*/
//fonction qui recherche les sauts dans le code et vérifie s'il y
int rech_sauts(Code_a * c){
  Ligne * r = c-> premier;
  //cette boucle va rechercher les sauts dans chaque ligne du code assembleur en parcourant la structure Code_a, puis va placer dans S l'adresse (adr) du saut où aller.
  while(r != NULL){
    if (r->jmp != NULL){
      int tes = 0;
      Ligne * r2 = c -> premier;
      while (r2 != NULL){
        if ((r2->etiq != NULL) && (strcmp(r->jmp,r2->etiq)==0)){
          r->s = r2->adr;
          tes=1;
        }
        r2 = r2-> suiv;
      }
      if (tes == 0){
        return r->adr;
      }
    }
    r = r-> suiv;
  }
  return 0;
}
/*---------------------------------*/
//fonction qui traduit les codes opérandes, rd et rn en hexadécimal
char * code_rn_rd_binaire(int code_opbis){
  char * ligne_bin = calloc(O,sizeof(char));
  char * ligne_bin2 = calloc(O,sizeof(char));
  char * Valeur0 = "0";
  char * Valeur1 = "1";
  int i = 3;
  //écrit en binaire l'opérande, rn ou rd qui seront en arguments de fonction, puis qui va les traduire en binaire et placer le premier bit (vers la gauche) dans la chaine de caractère à renvoyer
  if (code_opbis > 15){
    ligne_bin = "1";
    code_opbis -= 16;
  }
  else{
    ligne_bin = "0";
  }
  //concatène le premier bit dans la chaîne à renvoyer
  strcat(ligne_bin2, ligne_bin);
  //boucle qui va placer le reste des bits à la suite de la chaîne de caractère
  while(i >= 0){
    int p = pow(2,i);
    if ( code_opbis >= p ){
      strcat(ligne_bin2,Valeur1);
      code_opbis -= p;
    }
    else{
      strcat(ligne_bin2,Valeur0);
    }
    i--;
  }
  //free(ligne_hexa); on ne peut pas free car cela cause problème pour la suite de notre programme
  return ligne_bin2;
}
/*---------------------------------*/
//fonction qui traduit Src2 en hexadécimal
char * code_S_binaire(int code_opbis, int bits){
  //elle prend en argument un entier en décimal puis retourne une chaine de caractère qui correspond au nombre en base 2
  char * ligne_bin = calloc(O,sizeof(char));
  char * ligne_bin2 = calloc(O,sizeof(char));
  char * Valeur0 = "0";
  char * Valeur1 = "1";
  int i = bits-2;
  int neg = 0;
  //on vérifie d'abord s'il est négatif
  if (code_opbis<0){
    code_opbis = code_opbis * (-1);
    neg = 1;
  }
  // procède de la même maniere que code_rn_rd_binaire(), mais cette fonction va l'éffectuer sur 16 bits
  if (code_opbis > pow(2,bits-1)-1){
    ligne_bin = "1";
    code_opbis -= pow(2,bits-1);
  }
  else{
    ligne_bin = "0";
  }
  //idem que code_rn_rd_binaire()
  strcat(ligne_bin2, ligne_bin);
  while(i >= 0){
    int p = pow(2,i);
    if ( code_opbis >= p){
      strcat(ligne_bin2,Valeur1);
      code_opbis -= p;
    }
    else{
      strcat(ligne_bin2,Valeur0);
    }
    i--;
  }
  if (neg == 1){
    int e = 0;
    while(ligne_bin2[e] != '\0'){
      if (ligne_bin2[e]=='1'){
        ligne_bin2[e]='0';
      }
      else{
        ligne_bin2[e]='1';
      }
      e++;
    }
    e--;
    while(e >= 0){
      if (ligne_bin2[e] == '0'){
        ligne_bin2[e] = '1';
        e = -1;
      }
      else{
        ligne_bin2[e] = '0';
        e -= 1;
      }
    }
  }
  return ligne_bin2;
}
/*---------------------------------*/
//fonction qui traduit une ligne de bianire en hexadecimal
char * trad_hex(char * ligne_bin){
  char * ligne_hex = calloc(O,sizeof(char));
  char ligne_bin2[32];
  int i = 0;
  //boucle qui permet de traduire un nombre binaire stockée dans la chaine de caractere donnée en argument en un nombre en hexadicmal
  while(i < 32){
    int som_hex=0;
    char * val = calloc(O,sizeof(char));
    //dans un premier temps nous récupérons les quatres premiers bits (toujours vers la gauche) et les placer dans un buffer temporaire
    for(int j = 0; j < 4; j++ ){
      ligne_bin2[j] = ligne_bin[j+i];
    }
    //suite de condtion qui vont permettre de traduire les quatres bits en un nombre en base 16
    if (ligne_bin2[0] == '1'){
      som_hex += 8;
    }
    if (ligne_bin2[1] == '1'){
      som_hex += 4;
    }
    if (ligne_bin2[2] == '1'){
      som_hex += 2;
    }
    if (ligne_bin2[3] == '1'){
      som_hex += 1;
    }
    if (som_hex>9){
      * val = 'A'+ som_hex-10;
    }
    else{
      * val ='0'+som_hex;
    }
    //concaténe chaque traduction dans la ligne à qui sera écrite dans le fichier hexa.txt
    strcat(ligne_hex,val);
    i += 4;
  }
  strcat(ligne_hex,"\n");
  return ligne_hex;
}
/*---------------------------------*/
// fonction qui traduit les lignes de code en en binaire
void trad_ecriture(Code_a * c,FILE * fic){
  //Cette fonction va utiliser les différentes fonctions définies juste avant afin de traduire une ligne d'assembleur en binaire pui en hexa décimal puis va enfin écrire chaque ligne dans le fichier texte hexa.txt qui sera en argument
  Ligne * l = c->premier;
  while (l != NULL ){
    //code opérande en hexadécial
    char * ligne_bin = calloc(O,sizeof(char));

    int code_opbis = l->code_op;
    int rd_bis = l->rd;
    int rn_bis = l->rn;
    char * imm_bin = calloc(1,sizeof(char));
    *imm_bin = '0' + l->imm;
    //printf("%s\n",imm_bin );
    int s_bis = l->s;

    //copie dans une chaine de caractères les valeurs en binaires des opérandes, rd, rn et S
    char * op_bin = code_rn_rd_binaire(code_opbis);
    char * rd_bin = code_rn_rd_binaire(rd_bis);
    char * rn_bin = code_rn_rd_binaire(rn_bis);
    char * s_bin = code_S_binaire(s_bis,16);

    //printf("\nop : %s -- rd : %s -- rn : %s imm : %s -- s : %s\n",op_bin,rd_bin,rn_bin,imm_bin,s_bin );

    //Concatène toutes les chaines crées dans la variable ligne_bin qui va contenir la ligne en binaire
    strcat(ligne_bin,strcat(op_bin,strcat(rd_bin,strcat(rn_bin,imm_bin))));
    strcat(ligne_bin,s_bin);

    //printf("Voici la ligne en binaire : %s\n", ligne_bin );

    //traduit la ligne binaire en hexadécimale
    trad_hex(ligne_bin);

    //printf("Voici la ligne en hexadécimal : %s\n", trad_hex(ligne_bin));

    //écrit la ligne traduite directement dans le fichier hexa.txt crée dans le main
    fputs(trad_hex(ligne_bin),fic);


    free(ligne_bin);
    free(code_rn_rd_binaire(code_opbis));
    free(code_rn_rd_binaire(rd_bis));
    free(code_rn_rd_binaire(rn_bis));
    free(code_S_binaire(s_bis,16));
    free(imm_bin);
    l = l->suiv;
  }
  return ;
}
/*---------------------------------*/
//fonction qui récupère les donnés lignes par ligne d'un fichier assembleur
int traduit_fichier_assembleur(char * ligne_fichier,int adr, Code_a * c1){
  //déclaration des variables nécessaires pour une ligne
  int imm, s, rd, rn, code_op;
  //variables qui vont permettre de parcourir la ligne
  int t = 0, r = 0, a = 0;

  //copie l'etiquette puis cherche l'indice du premier caractere apres l'étiquette
  char * etiq = NULL;
  if (strchr(ligne_fichier,':') != NULL){
    etiq = malloc(O*sizeof(char));
    while((ligne_fichier[a] != ':') && (ligne_fichier[a] != ' ')){
      etiq[a] = ligne_fichier[a];
      a++;
    }
    if (ligne_fichier[a]==' '){
      while(ligne_fichier[a] != ':'){
        if (ligne_fichier[a] != ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        a++;
      }
    }
  }
  char * jmp = NULL;
  //printf("%s\n", etiq );

  //copie l'opérande et cherche l'indice du premier caractere apres l'etiquette
  char * op = NULL;
  if (strchr(ligne_fichier,':') != NULL){
    op = malloc(O*sizeof(char));
    while(ligne_fichier[t] != ':'){
      t++;
    }
    t++;
    // boucle qui va passer tous les espaces (si nous sommes au milieu de la ligne)
    while(ligne_fichier[t] == ' '){
      t++;
    }
    //boucle qui va copier l'operande
    while((ligne_fichier[t]!= ' ') && (ligne_fichier[t]!='\0') && (ligne_fichier[t]!='\n')){
      op[r]=ligne_fichier[t];
      r++;
      t++;
    }
  }
  else{
    op = malloc(O*sizeof(char));
    while(ligne_fichier[t] == ' '){
      t++;
    }
    while((ligne_fichier[t]!= ' ') && (ligne_fichier[t]!='\0') && (ligne_fichier[t]!='\n')){
      op[r]=ligne_fichier[t];
      r++;
      t++;
    }
  }
  //printf("%s\n",op );
  code_op = op_en_decimal(op);
  if (code_op == 200){
    printf("Erreur à la ligne %d \n", (adr/4)+1);
    return 0;
  }
  free(op);
  //printf("%d \n", code_op);
  /*-------------------------------------------------------*/
  //condition qui considère que les instructions arithmétiques et logiques
  if (( (code_op >= 0) && ( code_op <= 7) ) || (code_op == 23)){
    // récupère rd
    char * c_rd = NULL;
    c_rd = malloc(O*sizeof(char));
    int i = 0;
    //boucle qui permet de sauter les espaces
    while(ligne_fichier[t] == ' '){
      t++;
    }
    if(ligne_fichier[t]!='r'){
      printf("Erreur à la ligne %d \n", (adr/4)+1);
      return 0;
    }
    t++;

    while((ligne_fichier[t]!=',') && (ligne_fichier[t] != ' ')){
      if((ligne_fichier[t]<'0')||(ligne_fichier[t]>'9')){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      c_rd[i]=ligne_fichier[t];
      i++;
      t++;
    }
    while(ligne_fichier[t] != ','){
      if (ligne_fichier[t] != ' '){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      t++;
    }

    //transforme une chaine en int
    rd = strtol(c_rd,NULL,10);
    free(c_rd);
    //printf("%d\n",rd );
    // recupere rn
    char * c_rn = NULL;
    c_rn = malloc(O*sizeof(char));
    int w = 0;
    t++;
    while(ligne_fichier[t] == ' '){
      t++;
    }
    if(ligne_fichier[t]!='r'){
      printf("Erreur à la ligne %d \n", (adr/4)+1);
      return 0;
    }
    t++;
    while((ligne_fichier[t]!=',') && (ligne_fichier[t] != ' ')){
      if((ligne_fichier[t]<'0')||(ligne_fichier[t]>'9')){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      c_rn[w]=ligne_fichier[t];
      w++;
      t++;
    }
    while(ligne_fichier[t] != ','){
      if (ligne_fichier[t] != ' '){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      t++;
    }
    //converti c_rn en rn
    rn = strtol(c_rn,NULL,10);
    free(c_rn);
    //printf("%d\n", rn);
    /*-------------------------------------------------------*/
    //recupere s
    char * c_s = NULL;
    c_s = malloc(O*sizeof(char));
    int y = 0;
    t++;
    while(ligne_fichier[t] == ' '){
      t++;
    }
    if (ligne_fichier[t]=='#'){
      imm = 1;
      t++;
      if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') ) ){
        while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-')){
          c_s[y]= ligne_fichier[t];
          y++;
          t++;
        }
        while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
          if (ligne_fichier[t] != ' '){
            printf("Erreur à la ligne %d \n", (adr/4)+1);
            return 0;
          }
          t++;
        }
        s = strtol(c_s,NULL,10);
        //printf("%d\n", s);
      }
      else if(ligne_fichier[t]=='h'){
        t++;
        if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f')) ) ){
          while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f'))){
            c_s[y]= ligne_fichier[t];
            y++;
            t++;
          }
        }
        while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
          if (ligne_fichier[t] != ' '){
            printf("Erreur à la ligne %d \n", (adr/4)+1);
            return 0;
          }
          t++;
        }
        //transforme une chaine en int
        s = strtol(c_s,NULL,16);
        //printf("%d\n", s);

      }
      else{
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
    }
    else if (ligne_fichier[t] == 'r'){
      imm=0;
      t++;
      while((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')){
        c_s[y]= ligne_fichier[t];
        y++;
        t++;
      }
      while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
        if (ligne_fichier[t] != ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        t++;
      }
      s = strtol(c_s,NULL,10);
      free(c_s);
      //printf("%d\n", s);
    }
    else{
      printf("Erreur à la ligne %d \n", (adr/4)+1);
      return 0;
    }
  }
  /*-------------------------------------------------------*/
  // condition qui considère que les instructions de transferts
  if((code_op > 7) && (code_op <= 13)){

    //condition qui consiere les de db à dw
    if (code_op <= 10){
      // récupère rd
      char * c_rd = NULL;
      c_rd = malloc(O*sizeof(char));
      int i = 0;
      //boucle qui permet de sauter les espaces
      while(ligne_fichier[t] == ' '){
        t++;
      }

      if(ligne_fichier[t]!='r'){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      t++;
      while((ligne_fichier[t]!=',') && (ligne_fichier[t] != ' ')){
        if((ligne_fichier[t]<'0')||(ligne_fichier[t]>'9')){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        c_rd[i]=ligne_fichier[t];
        i++;
        t++;
      }

      while(ligne_fichier[t] != ','){
        if (ligne_fichier[t] != ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        t++;
      }

      //transforme une chaine en int
      rd = strtol(c_rd,NULL,10);
      free(c_rd);
      //printf("%d\n",rd );

      //recupere rn
      char * c_rn = NULL;
      c_rn = malloc(O*sizeof(char));
      int w = 0;
      t++;
      while(ligne_fichier[t] != '('){
        if( ligne_fichier[t] != ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        t++;
      }

      t++;
      while(ligne_fichier[t] == ' '){
        t++;
      }
      if(ligne_fichier[t]!='r'){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }

      t++;
      while((ligne_fichier[t]!=')') && (ligne_fichier[t] != ' ')){
        if((ligne_fichier[t]<'0')||(ligne_fichier[t]>'9')){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        c_rn[w]=ligne_fichier[t];
        w++;
        t++;
      }
      while(ligne_fichier[t] != ')'){
        if (ligne_fichier[t] != ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        t++;
      }

      //printf("%s\n",c_rn );
      //converti c_rn en rn
      rn = strtol(c_rn,NULL,10);
      //printf("%d\n", rn);
      /*-------------------------------------------------------*/
      //recupere s
      char * c_s = NULL;
      c_s = malloc(O*sizeof(char));
      int y = 0;
      t++;
      while(ligne_fichier[t] == ' '){
        t++;
      }
      if((ligne_fichier[t]!='r') && (ligne_fichier[t] != '#')){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      if (ligne_fichier[t]=='#'){
        imm = 1;
        t++;
        if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') ) ){
          while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-')){
            c_s[y]= ligne_fichier[t];
            y++;
            t++;
          }
          while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
            if (ligne_fichier[t] != ' '){
              printf("Erreur à la ligne %d \n", (adr/4)+1);
              return 0;
            }
            t++;
          }
          s = strtol(c_s,NULL,10);
          //printf("%d\n", s);
        }
        else if(ligne_fichier[t]=='h'){
          t++;
          if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f')) ) ){
            while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f'))){
              c_s[y]= ligne_fichier[t];
              y++;
              t++;
            }
          }
          while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
            if (ligne_fichier[t] != ' '){
              printf("Erreur à la ligne %d \n", (adr/4)+1);
              return 0;
            }
            t++;
          }
          //transforme une chaine en int
          s = strtol(c_s,NULL,16);
          //printf("%d\n", s);

        }
        else{
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
      }
      else if (ligne_fichier[t] == 'r'){
        imm=0;
        t++;
        while((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')){
          c_s[y]= ligne_fichier[t];
          y++;
          t++;
        }
        while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
          if (ligne_fichier[t] != ' '){
            printf("Erreur à la ligne %d \n", (adr/4)+1);
            return 0;
          }
          t++;
        }
        s = strtol(c_s,NULL,10);
        free(c_s);
        //printf("%d\n", s);
      }
      else{
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
    }
    /*-------------------------------------------------------*/
    //condition qui consiere les de stb à stw
    if (code_op>=11){
      //recupere rd
      char * c_rd = NULL;
      c_rd = malloc(O*sizeof(char));
      int i=0;
      while(ligne_fichier[t] != '('){
        if( ligne_fichier[t] != ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        t++;
      }
      t++;
      while(ligne_fichier[t] == ' '){
        t++;
      }
      if(ligne_fichier[t]!='r'){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }

      t++;
      while((ligne_fichier[t]!=')') && (ligne_fichier[t] != ' ')){
        if((ligne_fichier[t]<'0')||(ligne_fichier[t]>'9')){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        c_rd[i]=ligne_fichier[t];
        i++;
        t++;
      }
      while(ligne_fichier[t] != ')'){
        if (ligne_fichier[t] != ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        t++;
      }
      //converti c_rd en rd
      rd = strtol(c_rd,NULL,10);
      //printf("%d\n", rd);
      /*-------------------------------------------------------*/
      //recupere s
      char * c_s = NULL;
      c_s = malloc(O*sizeof(char));
      int y = 0;
      t++;
      while(ligne_fichier[t] == ' '){
        t++;
      }
      if((ligne_fichier[t]!='r') && (ligne_fichier[t] != '#')){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      if (ligne_fichier[t]=='#'){
        imm = 1;
        t++;
        if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') ) ){
          while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-')){
            c_s[y]= ligne_fichier[t];
            y++;
            t++;
          }
          s = strtol(c_s,NULL,10);
          //printf("%d\n", s);
        }
        else if(ligne_fichier[t]=='h'){
          t++;
          if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f')) ) ){
            while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f'))){
              c_s[y]= ligne_fichier[t];
              y++;
              t++;
            }
          }
          //transforme une chaine en int
          s = strtol(c_s,NULL,16);
          //printf("%d\n", s);

        }
        else{
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
      }
      else if (ligne_fichier[t] == 'r'){
        imm=0;
        t++;
        while((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')){
          c_s[y]= ligne_fichier[t];
          y++;
          t++;
        }
        s = strtol(c_s,NULL,10);
        free(c_s);
        //printf("%d\n", s);
      }
      else{
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      //printf("%d\n", s);
      /*-------------------------------------------------------*/
      //recupere rn
      char * c_rn = NULL;
      c_rn = malloc(O*sizeof(char));
      int w =0;
      //boucle qui permet de sauter les espaces
      while(ligne_fichier[t] !=','){
        if (ligne_fichier[t]!= ' '){
          printf("Erreur à la ligne %d \n", (adr/4)+1);
          return 0;
        }
        t++;
      }
      t++;
      while(ligne_fichier[t]==' '){
        t++;
      }
      if(ligne_fichier[t]!='r'){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      t++;
      while((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')){
        c_rn[w]=ligne_fichier[t];
        w++;
        t++;
      }
      //transforme une chaine en int
      rn = strtol(c_rn,NULL,10);
      //printf("%d\n", rn );
    }
    while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
      if (ligne_fichier[t] != ' '){
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      t++;
    }
  }

  /*-------------------------------------------------------*/
  //condition qui considère que les instructions de sauts
  if((code_op > 13) && (code_op <= 20)){
    rd = 0;
    rn = 0;
    char * c_s = malloc(O*sizeof(char));
    int y = 0;
    t++;
    while(ligne_fichier[t] == ' '){
      t++;
    }

    if (ligne_fichier[t] == '#'){
      imm = 1;
      t++;
      if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') ) ){
        while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-')){
          c_s[y]= ligne_fichier[t];
          y++;
          t++;
        }
        s = strtol(c_s,NULL,10);
        //printf("%d\n", s);
      }
      else if(ligne_fichier[t]=='h'){
        t++;
        if ( (( (ligne_fichier[t]>='0')&&(ligne_fichier[t]<='9') ) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f')) ) ){
          while(((ligne_fichier[t]!=' ')&&(ligne_fichier[t]!='\0')&& (ligne_fichier[t]>='0') && (ligne_fichier[t]<='9')) || (ligne_fichier[t]=='-') || ((ligne_fichier[t]>='a')&&(ligne_fichier[t]<='f'))){
            c_s[y]= ligne_fichier[t];
            y++;
            t++;
          }
        }
        //transforme une chaine en int
        s = strtol(c_s,NULL,16);
        //printf("%d\n", s);

      }
      else{
        printf("Erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
    }
    else{
      if (ligne_fichier[t] == 'r' && (ligne_fichier[t+1] < '9')){
        imm = 0;
        t++;
        while((ligne_fichier[t]!=' ') && (ligne_fichier[t]!='\0')){
          c_s[y] = ligne_fichier[t];
          y++;
          t++;
        }
        s = strtol(c_s,NULL,10);
      }
      else{
        jmp = malloc(sizeof(char));
        imm = 1;
        while((ligne_fichier[t]!= ' ') && (ligne_fichier[t]!='\0') && (ligne_fichier[t]!='\n')){
          c_s[y]= ligne_fichier[t];
          y++;
          t++;
        }
        s = 0;
        strcpy(jmp,c_s);
      }
    }
    free(c_s);
  }
  /*-------------------------------------------------------*/
  //condition qui considere que les entrée et sortie
  if((code_op > 20) && (code_op <= 22)){
    int i = 0;
    imm = 0;
    char * c_rd = NULL;
    c_rd = malloc(sizeof(char));
    while(ligne_fichier[t] == ' '){
      t++;
    }
    t++;
    while((ligne_fichier[t]!= ' ') && (ligne_fichier[t]!='\0') && (ligne_fichier[t]!='\n')){
      c_rd[i]=ligne_fichier[t];
      i++;
      t++;
    }
    rd = strtol(c_rd,NULL,10);
    free(c_rd);
    rn = 0;
    s = 0;
  }
  /*-------------------------------------------------------*/
  //condition pour terminer le programme
  if(code_op == 31){
    rn = 0;
    rd = 0;
    s = 0;
    imm = 0;
    while((ligne_fichier[t]!='\n')&&(ligne_fichier[t]!='\0')){
      if (ligne_fichier[t] != ' '){
        printf("erreur à la ligne %d \n", (adr/4)+1);
        return 0;
      }
      t++;
    }
  }
  /*-------------------------------------------------------*/
  //permet d'ajouter une ligne à la structure du code
  if ((rd<0)||(rd>31) || (rn<0)||(rn>31) || ( (imm==1) && ( (s<-32768) || (s>32767) ) ) ||( (imm==0) && ( (s<0) || (s>31) ) )  ){
    printf("Erreur à la ligne %d \n", (adr/4)+1 );
    return 0;
  }
  code_ajout_ligne(c1,code_op,rd,rn,imm,s,adr,etiq,jmp);

  return 1;
}
/*---Fonctions Pour la partie Exécution---*/
/*---------------------------------*/
//fonction qui traduit d'hexa en binaire
int * hexa_a_bin (char * ligne_fichier){
  int * binaire = malloc(O*sizeof(int));

  int i = 0;
  //cette boucle va permettre de traduire ligne par ligne le fichier en hexa.txt en étudiant chaque caractère puis en ajoutant dans un tableau d'entier
  while(i<9){
    if (ligne_fichier[i]=='F'){
      for(int j = 0; j < 4; j++){
        binaire[j+i*4] = 1;
      }
    }
    if (ligne_fichier[i] == 'E' ){
      for(int j = 0; j < 3; j++){
        binaire[j+i*4] = 1;
      }
      binaire[i*4+3] =  0;
    }
    if (ligne_fichier[i] == 'D' ){
      binaire[i*4] = 1;
      binaire[i*4+1] = 1;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 1;
    }
    if (ligne_fichier[i] == 'C' ){
      binaire[i*4] = 1;
      binaire[i*4+1] = 1;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 0;
    }
    if (ligne_fichier[i] == 'B' ){
      binaire[i*4] = 1;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 1;
      binaire [i*4+3] = 1;
    }
    if (ligne_fichier[i] == 'A' ){
      binaire[i*4] = 1;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 1;
      binaire [i*4+3] = 0;
    }
    if (ligne_fichier[i] == '9' ){
      binaire[i*4] = 1;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 1;
    }
    if (ligne_fichier[i] == '8' ){
      binaire[i*4] = 1;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 0;
    }
    if (ligne_fichier[i] == '7' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 1;
      binaire [i*4+2] = 1;
      binaire [i*4+3] = 1;
    }
    if (ligne_fichier[i] == '6' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 1;
      binaire [i*4+2] = 1;
      binaire [i*4+3] = 0;
    }
    if (ligne_fichier[i] == '5' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 1;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 1;
    }
    if (ligne_fichier[i] == '4' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 1;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 0;
    }
    if (ligne_fichier[i] == '3' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 1;
      binaire [i*4+3] = 1;
    }
    if (ligne_fichier[i] == '2' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 1;
      binaire [i*4+3] = 0;
    }
    if (ligne_fichier[i] == '1' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 1;
    }
    if (ligne_fichier[i] == '0' ){
      binaire[i*4] = 0;
      binaire[i*4+1] = 0;
      binaire [i*4+2] = 0;
      binaire [i*4+3] = 0;
    }
    i++;
  }
  return binaire;
}
/*---------------------------------*/
//fonction qui récupère les opérandes d'une ligne en binaire
int * binaire_a_decimal(int * binaire){
  int  * tab_op_rn_rd_imm_S = calloc(O,sizeof(int));
  int p = 4, p2 = 15, p3 = 0;
  //cette boucle va découper un tableau d'un nombre en binaire afin d'y extraire les codes opérandes, rd, rn, Imm et S. Elle traite aussi le cas où S est négatif...
  for (int i = 0; i < 32; i++ ){
    if(i < 5){
      tab_op_rn_rd_imm_S[0] += (binaire [i] * pow(2,p));
      p--;
    }
    if(i == 5) p = 4;
    if((i >= 5) && (i < 10) ){
      tab_op_rn_rd_imm_S[1] += (binaire [i]*pow(2,p));
      p--;
    }
    if(i == 10) p = 4;
    if((i >= 10) && (i < 15) ){
      tab_op_rn_rd_imm_S[2] += (binaire [i]*pow(2,p));
      p--;
    }
    if (i == 15) tab_op_rn_rd_imm_S[3] = binaire[i];
    if (i > 15){
      if ((binaire[16] == 1) && (i == 16)){
        p3 = 1;
        for (int m = 0; m < 16; m++){
          if (binaire[m+i] == 1){
            binaire [m+i] = 0;
          }
          else {
            binaire[m+i] = 1;
          }
        }
        int m2 = 31;
        while( binaire[m2] != 0){
          binaire[m2] = 0;
          m2--;
        }
        binaire[m2] = 1;
      }
      tab_op_rn_rd_imm_S[4] += (binaire [i]*pow(2,p2));
      p2--;
    }
  }
  //vérifie si S est négatif et multiplie à la fin le nombre pour le rendre négagtif
  if(p3 == 1 ) tab_op_rn_rd_imm_S[4] *= (-1);

  return tab_op_rn_rd_imm_S;
}
/*---------------------------------*/
//fonction qui traduit du décimale au binaire
int * code_S_binaire2 (int s){
  //Prend en argument un nombre en décimal puis retourne un pointeur sur tableau contenant en base 2 ce nombre
  int * s_tab = malloc(O*sizeof(int));
  int temp = 0;
  for(int j = 0; j<32; j++){
    s_tab[j]=0;
  }
  if(s < 0){
    s = s*(-1);
    temp = 1;
  }
  int i = 31;
  while(s > 0 && i >= 0 ){
    s_tab[i] = s%2;
    s = s/2;
    i--;
  }
  if (temp == 0){
    return s_tab;
  }
  else{
    for(int x = 0; x<32 ;x++ ){
      if(s_tab[x] == 1){
        s_tab[x] = 0;
      }
      else{
        s_tab[x] = 1;
      }
    }

    int e = 31;
    while(e >= 0){
      if (s_tab[e] == 0){
        s_tab[e] = 1;
        e = -1;
      }
      else{
        s_tab[e] = 0;
        e -= 1;
      }
    }
    return s_tab;
  }

}
/*---------------------------------*/
//fonction qui recherche l'adresse dans le fichier hexa et retourne la ligne en binaire correspondant à l'adresse
char * rech_adr(Code_a * c, int adr3){
  Ligne * r = c-> premier;
  //cette boucle va rechercher les adresses dans chaque ligne du code hexa.txt en parcourant la structure Code_a.
  while(r != NULL){
    if (r->adr == adr3){
      //procède de la même manière que la fonction trad_hex()
      char * ligne_bin = calloc(O,sizeof(char));
      char * op_bin = code_rn_rd_binaire(r->code_op);
      char * rd_bin = code_rn_rd_binaire(r->rd);
      char * rn_bin = code_rn_rd_binaire(r->rn);
      char * imm_bin = calloc(O,sizeof(char));
      *imm_bin = '0' + r->imm;
      char * s_bin = code_S_binaire(r->s,16);
      strcat(ligne_bin,strcat(op_bin,strcat(rd_bin,strcat(rn_bin,imm_bin))));
      strcat(ligne_bin,s_bin);


      free(code_rn_rd_binaire(r->code_op));
      free(code_rn_rd_binaire(r->rd));
      free(code_rn_rd_binaire(r->rn));
      free(code_S_binaire(r->s,16));
      return ligne_bin;
    }
    r = r-> suiv;
  }
  return NULL;
}
/*---------------------------------*/
//fonction qui traduit du binaire au décimale sur n bits
int binaire_decimale32(int * int_rn_bin, int bits){
  //Traduit un tableau contenant un nombre biniare sur n bits et renvoie ce nombre en décimal
  int bin = 0, p3 = 0;
  if (int_rn_bin[0] == 1){
    p3 = 1;
    for (int m = 0; m < bits; m++){
      if (int_rn_bin[m] == 1){
        int_rn_bin [m] = 0;
      }
      else {
        int_rn_bin[m] = 1;
      }
    }
    int m2 = bits-1;
    while( int_rn_bin[m2] != 0){
      int_rn_bin[m2] = 0;
      m2--;
    }
    int_rn_bin[m2] = 1;
  }
  int e=0;
  for(int g = bits-1; g >= 0; g--){
    bin += (int_rn_bin[g] * pow(2,e));

    e++;
  }
  //vérifie si S est négatif et multiplie à la fin le nombre pour le rendre négagtif
  if(p3 == 1 ) bin *= (-1);
  printf("%d\n",bin );
  return bin;
}
/*---------------------------------*/
//fonction qui va chercher l'adresse où sauter qui est est entrée en arguement.
Ligne * recherche_sauts_ex(Code_a * c2, int adr){
  Ligne * l = c2->premier;
  while(l != NULL ){
    if ( adr == l->adr){
      return l;
    }
    l = l->suiv;
  }
  printf("\nLe fichier ne contient pas cette adressse.\n" );
  return NULL;
}
/*---------------------------------*/
//libere les code assembleur
void libere_code(Code_a *c){
  Ligne * l = c->premier;
  while(l != NULL){
    Ligne * temp = l;
    l = l->suiv;
    free(temp);
    temp = NULL;
  }
  free(l);
}
/*---------------------------------*/
//Fonction qui exécute les instructions une à une
int execute(Code_a * c2){
  //Initialisation des registres
  int  tab_registre [33];
  for (int h = 0; h < 33; h++){
    tab_registre[h] = 0;
  }
  //On initialise le registre PC à 4 la premiere adresse
  tab_registre[32] = 0;
  int z = 0, c = 0, n = 0;

  Ligne * l = c2->premier;
  //Cette boucle permet d'exécuter ligne par ligne lee fichier hexa.txt
  while(l != NULL) {
    /*--- Exécution Des Instruction Arithmétiques Et Logiques*/
    //exécution opérande 0(and)
    if (l->code_op == 0){
      //Vérifie d'abord si le Src2 est un registre ou une valeur puis éxecute l'opérande and en comparant les valeurs directement en décimal
      if (l->imm == 0){
        if(tab_registre[l->s] == tab_registre[l->rn]){
          tab_registre[l->rd] = 1;
          z = 0;
          c = 0;
          n = 0;
        }
        else{
          tab_registre[l->rd] = 0;
          z = 1;
          c = 0;
          n = 0;
        }
      }
      else{
        if(l->s == tab_registre[l->rn]){
          tab_registre[l->rd] = 1;
          z = 0;
          c = 0;
          n = 0;
        }
        else{
          tab_registre[l->rd] = 0;
          z = 1;
          c = 0;
          n = 0;
        }
      }
    }
    //exécution opérande 1(or)
    if (l->code_op == 1){
      //Vérifie d'abord si Src2 est un registre ou une valeur puis exécute l'operande or en comparant les nombres en décimal
      if (l->imm == 0){
        if((tab_registre[l->s] == 0 ) && (tab_registre[l->rn] == 0)){
          tab_registre[l->rd] = 0;
          z = 1;
          c = 0;
          n = 0;
        }
        else{
          tab_registre[l->rd] = 1;
          z = 0;
          c = 0;
          n = 0;
        }
      }
      else{
        if ((tab_registre[l->rn] == 0) && (l->s==0)){
          tab_registre[l->rd] = 0;
          z = 1;
          c = 0;
          n = 0;
        }
        else{
          tab_registre[l->rd] = 1;
          z = 0;
          c = 0;
          n = 0;
        }
      }
    }
    //exécution opérande 2(xor)
    if (l->code_op == 2){
      //Vérifie d'abord si Src2 est un registre ou une valeur puis exécute l'operande xor en comparant directement les nombres en décimal
      if (l->imm == 0){
        if(tab_registre[l->s] == tab_registre[l->rn]){
          tab_registre[l->rd] = 0;
          z = 1;
          c = 0;
          n = 0;
        }
        else{
          tab_registre[l->rd] = 1;
          z = 0;
          c = 0;
          n = 0;
        }
      }
      else{
        if ( tab_registre[l->rn] == l->s ){
          tab_registre[l->rd] = 0;
          z = 1;
          c = 0;
          n = 0;
        }
        else{
          tab_registre[l->rd] = 1;
          z = 0;
          c = 0;
          n = 0;
        }
      }
    }
    //exécution opérande 3(mul)
    if (l->code_op == 3){
      //Cette premiere étape va recuperer les 16 bits de poids faible de rn puis va le traduire en base 10
      int *tab_temp = malloc(sizeof(int));
      int * tab_temp2;
      tab_temp2 = code_S_binaire2(tab_registre[l->rn]);
      for(int gh = 31; gh>=16; gh-- ){
        tab_temp[gh-16] = tab_temp2[gh];
      }
      int val = binaire_decimale32(tab_temp,16);
      // Effectue la multiplication des valeurs en décimal des resgistres et des valeurs dans les instructions
      if (l->imm == 0){
        if (tab_registre[l->s]>32768){
          int y = 15;
          while (tab_registre[l->s] > pow(2,y)){
            y++;
          }
          while(y>14){
            tab_registre[l->s] -= pow(2,y);
            y--;
          }
        }
        tab_registre[l->rd] = val * tab_registre[l->s];
        //pour tester le resultat de la multiplication
        //printf("%d\n",tab_registre[l->rd] );
      }
      else{
        tab_registre[l->rd] = val * l->s ;
        //pour tester le resultat de la multiplication
        //printf("%d\n",tab_registre[l->rd] );
      }
      //On actualise les registres d'état.
      if (tab_registre[l->rd] == 0){
        z = 1;
      }
      else{
        z = 0;
      }
      c = 0;
    }
    //exécution opérande 4(div)
    if (l->code_op == 4){
      if (l->imm == 0){
        //On vérifie d'abord qu'on divie pas par 0, le cas écheant on affiche une erreur. Puis on effecute la division avec les nombres en décimal
        if (tab_registre[l->rn] == 0 && tab_registre[l->s] == 0){
          printf("Impossible de diviser 0 par 0.\n");
          return 0;
        }
        tab_registre[l->rd] = tab_registre[l->rn] / tab_registre[l->s];
      }
      else{
        if (tab_registre[l->rn] ==0 && l->s == 0){
          printf("Impossible de diviser 0 par 0.\n");
          return 0;
        }
        tab_registre[l->rd] = tab_registre[l->rn] / l->s ;
      }
      if (tab_registre[l->rd] == 0){
        z = 1;
      }
      else{
        z = 0;
      }
      c = 0;
      if(tab_registre[l->rd] < 0){
        n = 1;
      }
      else{
        n = 0;
      }
    }
    //exécution opérande 5(add)
    if (l->code_op == 5){
      //On efecute l'addition classique entre deux nombres décimaux, on prend en compte bien sur s'il il peut y avoir de retenue.
      c = 0;
      if (l->imm == 0){
        tab_registre[l->rd] = tab_registre[l->rn] + tab_registre[l->s];
      }
      else{
        tab_registre[l->rd] = tab_registre[l->rn] + l->s ;
      }
      while (tab_registre[l->rd]>= pow(2,32)){
        c = 1;
        tab_registre[l->rd]-= pow(2,33);
      }
      if (tab_registre[l->rd]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rd]<0){
        n = 1;
      }
      else{
        n = 0;
      }
    }
    //exécution opérande 6 (sub)
    if(l->code_op == 6){
      c = 0;
      if (l->imm == 0){
        //on compare les deux nombres pour voir s'il y a une retenue lors de la soustraction des deux nombres, puis on met c à 1 si c'est le cas
        if((tab_registre[l->rn]) < (tab_registre[l->s])){
          c = 1;
        }
        tab_registre[l->rd] = tab_registre[l->rn] - tab_registre[l->s];
      }
      else{
        if((tab_registre[l->rn]) < (l->s)){
          c = 1;
        }
        tab_registre[l->rd] = tab_registre[l->rn] - l->s ;
      }
      if (tab_registre[l->rd] == 0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rd] < 0){
        n = 1;
      }
      else{
        n = 0;
      }
    }
    //exécution opérande 7 (shr)
    if(l->code_op == 7){
      //on récupere et traduit en binaire le contenu du registre rn, puis on le place dans un tableau d'entier. Grace à des boucles for on effectue le décalage des bits
      int * int_rn_bin = malloc(O * sizeof(int));
      int_rn_bin = code_S_binaire2(tab_registre[l->rn]);
      int cmp = 0;
      if (l->imm == 0){
        cmp =  (-1)* tab_registre[l->s];
      }
      else{
        cmp = (-1)* l->s;
      }
      if (cmp >= 0){
        while(cmp > 0){
          int cmp2 = 0;
          int cmp3 = 0;
          for (int e = 31; e >= 0; e--){
            cmp3 = int_rn_bin[e];
            int_rn_bin[e] = cmp2;
            cmp2 = cmp3;
            if(e == 0) c = int_rn_bin[0];
          }
          cmp--;
        }
      }
      //le cas où Src2 est négatif
      else{
        while(cmp < 0){
          int cmp2 = 0;
          int cmp3 = 0;
          for (int e = 0; e < 32; e++){
            cmp3 = int_rn_bin[e];
            int_rn_bin[e] = cmp2;
            cmp2 = cmp3;
            if(e == 31) c = int_rn_bin[31];
          }
          cmp++;
        }
      }
      //pour vérifier que le décalage est bien effectué
      for(int ds = 0; ds < 32; ds++){
        printf("%d",int_rn_bin[ds] );
      }
      printf("\n" );
      tab_registre[l->rd] = binaire_decimale32(int_rn_bin,32);
      if (tab_registre[l->rd]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rd]<0){
        n = 1;
      }
      else{
        n = 0;
      }
      free(int_rn_bin);
    }

    /*---Exécution Des Instructios De Transferts---*/
    //exécution opérande 8 (ldb)
    //Ces opérandes vont d'abord calculer l'adresse, puis vont copier la somme des octets nécessaires et les placer dans le regsitre destinataire. Si la ligne n'existe pas dans le fichier, on prend arbitrairement une ligne de 0 en binare
    if(l->code_op == 8){
      int adr3 = 0;
      int adr4 = 0;
      if (l->imm == 0){
        adr3 = tab_registre[l->rn] + tab_registre[l->s];
      }
      else{
        adr3 = tab_registre[l->rn] + l->s;
      }
      if (adr3 >= pow(2,32)){
        printf("adresse trop grande \n");
        return 0;
      }
      if (adr3 < 0){
        printf("adresse negative \n");
        return 0;
      }
      adr4 = adr3;
      while (adr4 % 4 != 0){
        adr4 -= 1;
      }
      char * ligne_binaire = calloc(O,sizeof(char));
      char * ligne_binaire14 = calloc(O,sizeof(char));
      ligne_binaire14 = rech_adr(c2,adr4);
      //printf("%s\n",ligne_binaire );
      if ( ligne_binaire14 == NULL){
        for(int j = 0; j < 32; j++){
          ligne_binaire[j] = '0';
        }
      }
      else{
        for(int j = 0; j < 32; j++){
          ligne_binaire[j] = ligne_binaire14[j];
        }
      }
      int * tab_binaire = malloc(O*sizeof(int));
      for(int j = 0; j < 8; j++){
        tab_binaire[j] = 0;
      }
      int chen = 0;
      if(adr3==adr4){
        for(int o = 31; o >= 24; o--){
          if(ligne_binaire[o] == '1'){
            tab_binaire[chen] = 1;
          }
          else{
            tab_binaire[chen] = 0;
          }
          chen++;
        }
      }
      else if (adr3==adr4+1){
        for(int o = 23; o >= 16; o--){
          if(ligne_binaire[o] == '1'){
            tab_binaire[chen] = 1;
          }
          else{
            tab_binaire[chen] = 0;
          }
          chen++;
        }
      }
      else if(adr3==adr4+2){
        for(int o = 15; o >= 8; o--){
          if(ligne_binaire[o] == '1'){
            tab_binaire[chen] = 1;
          }
          else{
            tab_binaire[chen] = 0;
          }
          chen++;
        }
      }
      else{
        for(int o = 7; o >= 0; o--){
          if(ligne_binaire[o] == '1'){
            tab_binaire[chen] = 1;
          }
          else{
            tab_binaire[chen] = 0;
          }
          chen++;
        }
      }

      tab_registre[l->rd] = binaire_decimale32(tab_binaire,8);
      //printf("%d\n",tab_registre[l->rd] );
      if (tab_registre[l->rd]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rd]<0){
        n = 1;
      }
      else{
        n = 0;
      }
      c = 0;
      //printf(" res : %d \n", tab_registre[4]);
      free(ligne_binaire);
      free(tab_binaire);
    }
    //exécution opérande 9 (ldh)
    if(l->code_op == 9){
      int adr3 = 0;
      int adr4 = 0;
      if (l->imm == 0){
        adr3 = tab_registre[l->rn] + tab_registre[l->s];
      }
      else{
        adr3 = tab_registre[l->rn] + l->s;
      }
      if (adr3>=pow(2,32)){
        printf("adresse trop grande \n");
        return 0;
      }
      if (adr3<0){
        printf("adresse negative \n");
        return 0;
      }
      adr4=adr3;
      while (adr4 % 4 != 0){
        adr4-=1;
      }
      if (adr4<=adr3+2){
        char *ligne_binaire = calloc(O,sizeof(char));
        char * ligne_binaire13 = calloc(O,sizeof(char));
        ligne_binaire13 = rech_adr(c2,adr4);

        //printf("%s\n",ligne_binaire );

        if ( ligne_binaire13 == NULL){
          for(int j = 0; j < 32; j++){
            ligne_binaire[j] = '0';
          }
        }
        else{
          for(int j = 0; j < 32; j++){
            ligne_binaire[j] = ligne_binaire13[j];
          }
        }
        int * tab_binaire = malloc(O*sizeof(int));
        for(int j = 0; j < 16; j++){
          tab_binaire[j] = 0;
        }
        int chen = 0;
        if(adr3==adr4){
          for(int o = 31; o >= 16; o--){
            if(ligne_binaire[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
        }
        else if (adr3==adr4+1){
          for(int o = 23; o >= 8; o--){
            if(ligne_binaire[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
        }
        else {
          for(int o = 15; o >= 0; o--){
            if(ligne_binaire[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
        }
        tab_registre[l->rd] = binaire_decimale32(tab_binaire,16);
      }
      else{
        char * ligne_binaire2_b = calloc(O,sizeof(char));
        char * ligne_binaire_b = calloc(O,sizeof(char));
        char * ligne_binaire = calloc(O,sizeof(char));
        ligne_binaire_b = rech_adr(c2,adr4);
        char * ligne_binaire2 = calloc(O,sizeof(char));
        ligne_binaire2_b = rech_adr(c2,adr4+4);
        //printf("%s\n",ligne_binaire );

        if ( ligne_binaire_b == NULL){
          for(int j = 0; j < 32; j++){
            ligne_binaire[j] = '0';
          }
        }
        else{
          for(int j = 0; j < 32; j++){
            ligne_binaire[j] = ligne_binaire_b[j];
          }
        }
        if (ligne_binaire2_b == NULL){
          for(int j = 0; j < 32; j++){
            ligne_binaire2[j] = '0';
          }
        }
        else{
          for(int j = 0; j < 32; j++){
            ligne_binaire2[j] = ligne_binaire2_b[j];
          }
        }
        int * tab_binaire = malloc(O*sizeof(int));
        for(int j = 0; j < 16; j++){
          tab_binaire[j] = 0;
        }
        int chen = 0;
        for(int o = 7; o >= 0; o--){
          if(ligne_binaire[o] == '1'){
            tab_binaire[chen] = 1;
          }
          else{
            tab_binaire[chen] = 0;
          }
          chen++;
        }
        for(int o = 31; o >= 24; o--){
          if(ligne_binaire2[o] == '1'){
            tab_binaire[chen] = 1;
          }
          else{
            tab_binaire[chen] = 0;
          }
          chen++;
        }
        tab_registre[l->rd] = binaire_decimale32(tab_binaire,16);
      }
      if (tab_registre[l->rd]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rd]<0){
        n = 1;
      }
      else{
        n = 0;
      }
      c = 0;
      //printf("%d \n", tab_registre[4]);
    }
    //exécution opérande 10 (ldw)
    if(l->code_op == 10){
      int * tab_binaire = malloc(O*sizeof(int));
      for(int j = 0; j < 32; j++){
        tab_binaire[j] = 0;
      }
      int adr3 = 0;
      int adr4 = 0;
      if (l->imm == 0){
        adr3 = tab_registre[l->rn] + tab_registre[l->s];
      }
      else{
        adr3 = tab_registre[l->rn] + l->s;
      }
      if (adr3>=pow(2,32)){
        printf("adresse trop grande \n");
        return 0;
      }
      if (adr3<0){
        printf("adresse negative \n");
        return 0;
      }
      adr4 = adr3;
      while (adr4 % 4 != 0){
        adr4 -= 1;
      }
      if (adr4 == adr3){
        char * ligne_binaire_B = calloc(O,sizeof(char));
        char * ligne_binaire = calloc(O,sizeof(char));
        ligne_binaire_B = rech_adr(c2,adr3);

        //printf("%s\n",ligne_binaire );

        if ( ligne_binaire_B == NULL){
          for(int j = 0; j < 32; j++){
            ligne_binaire[j] = '0';
          }
        }
        int chen = 0;
        for(int o = 31; o >= 0; o--){
          if(ligne_binaire[o] == '1'){
            tab_binaire[chen] = 1;
          }
          else{
            tab_binaire[chen] = 0;
          }
          chen++;
        }
        tab_registre[l->rd] = binaire_decimale32(tab_binaire,32);
      }
      else{
        char * ligne_binaire2_B = calloc(O,sizeof(char));
        char * ligne_binaire_B = calloc(O,sizeof(char));
        char * ligne_binaire = calloc(O,sizeof(char));
        ligne_binaire_B = rech_adr(c2,adr4);
        char * ligne_binaire2 = calloc(O,sizeof(char));
        ligne_binaire2_B = rech_adr(c2,adr4+4);
        //printf("%s\n",ligne_binaire );

        if ( ligne_binaire_B == NULL){
          for(int j = 0; j < 32; j++){
            ligne_binaire[j] = '0';
          }
        }
        else{
          for(int j = 0; j < 32; j++){
            ligne_binaire[j] = ligne_binaire_B[j];
          }
        }
        if ( ligne_binaire2_B == NULL){
          for(int j = 0; j < 32; j++){
            ligne_binaire2[j] = '0';
          }
        }
        else{
          for(int j = 0; j < 32; j++){
            ligne_binaire2[j] = ligne_binaire2_B[j];
          }
        }
        if(adr3 == adr4+1){
          int chen = 0;
          for(int o = 23; o >= 0; o--){
            if(ligne_binaire[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
          for(int o = 31; o >= 24 ; o--){
            if(ligne_binaire2[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
        }
        else if(adr3 == adr4+2){
          int chen = 0;
          for(int o = 15; o >= 0; o--){
            if(ligne_binaire[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
          for(int o = 31; o >= 16 ; o--){
            if(ligne_binaire2[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
        }
        else{
          int chen = 0;
          for(int o = 7; o >= 0; o--){
            if(ligne_binaire[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
          for(int o = 31; o >= 8 ; o--){
            if(ligne_binaire2[o] == '1'){
              tab_binaire[chen] = 1;
            }
            else{
              tab_binaire[chen] = 0;
            }
            chen++;
          }
        }
      }
      tab_registre[l->rd] = binaire_decimale32(tab_binaire,32);
      //printf("%d\n",tab_registre[l->rd] );
      if (tab_registre[l->rd]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rd]<0){
        n = 1;
      }
      else{
        n = 0;
      }
      c = 0;
    }
    //Ces opérandes vont calculer l'adresse à changer. Si cette adresse dans le fichier texte, elle est remplacée par le contenu du registre rn. Sinon, on créé une ligne de 0 en binaire à l'adresse calculée qui sera modifiée et stockée dans le code.
    //exécution opérande 11 (stb)
    if (l->code_op == 11){
      int adr5 = 0;
      int adr6 = 0;
      int tee = 0;
      if (l->imm == 0){
        adr5 = tab_registre[l->rd] + tab_registre[l->s];
      }
      else{
        adr5 = tab_registre[l->rd] + l->s;
      }
      if (adr5>=pow(2,32)){
        printf("adresse trop grande \n");
        return 0;
      }
      if (adr5<0){
        printf("adresse negative \n");
        return 0;
      }
      adr6 = adr5;
      while (adr6 % 4 != 0){
        adr6 -= 1;
      }
      char * ligne_binaire_B= calloc(O,sizeof(char));
      char * ligne_binaire = calloc(O,sizeof(char));
      ligne_binaire_B = rech_adr(c2,adr6);
      if ( ligne_binaire_B == NULL){
        tee=1;
        code_ajout_ligne(c2,0,0,0,0,0,adr6,NULL,NULL);
        ligne_binaire=rech_adr(c2,adr6);
      }
      else{
        for(int j = 0; j < 32; j++){
          ligne_binaire[j] = ligne_binaire_B[j];
        }
      }
      char * ligne_binaire_rn = calloc(O,sizeof(char));
      ligne_binaire_rn = code_S_binaire(tab_registre[l->rn],32);
      if (adr5 == adr6){
        for(int o = 31; o >= 24; o--){
          ligne_binaire[o]=ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6>0) && (tee==0) ){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }

      }
      else if(adr5 == adr6+1){
        for(int o = 31; o >= 24; o--){
          ligne_binaire[o-8] =ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f] =='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6>0) && (tee==0) ){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }
      }
      else if(adr5 == adr6+2){
        for(int o = 31; o >= 24; o--){
          ligne_binaire[o-16] =ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f] =='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6>0) && (tee==0) ){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }
      }
      else{
        for(int o = 31; o >= 24; o--){
          ligne_binaire[o-24] =ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f] =='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6>0) && (tee==0) ){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }
      }
      if (tab_registre[l->rn]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rn]<0){
        n = 1;
      }
      else{
        n = 0;
      }
      c = 0;
    }
    //exécution de l'opérande 12 (sth)
    if (l->code_op == 12){
      int adr5 = 0;
      int adr6 = 0;
      int tee=0;
      if(l->imm == 0){
        adr5 = tab_registre[l->rd] + tab_registre[l->s];
      }
      else{
        adr5 = tab_registre[l->rd] + l->s;
      }
      if (adr5>=pow(2,32)){
        printf("adresse trop grande \n");
        return 0;
      }
      if (adr5<0){
        printf("adresse negative \n");
        return 0;
      }
      adr6 = adr5;
      while (adr6 % 4 != 0){
        adr6 -= 1;
      }
      char * ligne_binaire_B= calloc(O,sizeof(char));
      char * ligne_binaire = calloc(O,sizeof(char));
      ligne_binaire_B = rech_adr(c2,adr6);
      if ( ligne_binaire_B == NULL){
        tee=1;
        code_ajout_ligne(c2,0,0,0,0,0,adr6,NULL,NULL);
        ligne_binaire=rech_adr(c2,adr6);
      }
      else{
        for(int j = 0; j < 32; j++){
          ligne_binaire[j] = ligne_binaire_B[j];
        }
      }
      char * ligne_binaire_rn = calloc(O,sizeof(char));
      ligne_binaire_rn = code_S_binaire(tab_registre[l->rn],32);
      if (adr5 == adr6){
        for(int o = 31; o >= 16 ; o--){
          ligne_binaire[o]=ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6 > 0) && (tee==0)){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }
      }
      else if (adr5 == adr6+1){
        for(int o = 31; o >= 16 ; o--){
          ligne_binaire[o-8]=ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6 > 0) && (tee==0)){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }
      }
      else if (adr5 == adr6+2){
        for(int o = 31; o >= 16 ; o--){
          ligne_binaire[o-16] = ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6 > 0) && (tee==0)){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }
      }
      else{
        char * ligne_binaire2_B= calloc(O,sizeof(char));
        char * ligne_binaire2 = calloc(O,sizeof(char));
        ligne_binaire2_B = rech_adr(c2,adr6+4);
        if ( ligne_binaire2_B == NULL){
          tee=1;
          code_ajout_ligne(c2,0,0,0,0,0,adr6+4,NULL,NULL);
          ligne_binaire=rech_adr(c2,adr6+4);
        }
        else{
          for(int j = 0; j < 32; j++){
            ligne_binaire2[j] = ligne_binaire2_B[j];
          }
        }
        for(int o = 31; o >= 24 ; o--){
          ligne_binaire[o-24]=ligne_binaire_rn[o];
        }
        for(int o = 31; o >= 24 ; o--){
          ligne_binaire2[o] = ligne_binaire_rn[o-8];
        }
        int * tab_binaire = malloc(sizeof(int));
        int * tab_binaire2 = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        for(int f = 0; f < 32; f++){
          if (ligne_binaire2[f]=='1'){
            tab_binaire2[f] = 1;
          }
          else{
            tab_binaire2[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        int * tab_instruction2 = binaire_a_decimal(tab_binaire2);
        if((adr6 > 0) && (tee==0)){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          l15->suiv=l14;
          l14->suiv=l18;
          l18->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          l18->suiv=NULL;
          l14->suiv=l18;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          c2->premier = l14;
          l14->suiv=l18;
          l18->suiv = l16->suiv;
        }
      }
      if (tab_registre[l->rn]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rn]<0){
        n = 1;
      }
      else{
        n = 0;
      }
      c = 0;
    }
    //exécution de l'opérande 13 (stw)
    if (l->code_op == 13){
      int adr5 = 0;
      int adr6 = 0;
      int tee=0;
      if(l->imm == 0){
        adr5 = tab_registre[l->rd] + tab_registre[l->s];
      }
      else{
        adr5 = tab_registre[l->rd] + l->s;
      }
      if (adr5>=pow(2,32)){
        printf("adresse trop grande \n");
        return 0;
      }
      if (adr5<0){
        printf("adresse negative \n");
        return 0;
      }
      adr6 = adr5;
      while (adr6 % 4 != 0){
        adr6 -= 1;
      }
      char * ligne_binaire_B= calloc(O,sizeof(char));
      char * ligne_binaire = calloc(O,sizeof(char));
      ligne_binaire_B = rech_adr(c2,adr6);
      if ( ligne_binaire_B == NULL){
        tee=1;
        code_ajout_ligne(c2,0,0,0,0,0,adr6,NULL,NULL);
        ligne_binaire=rech_adr(c2,adr6);
      }
      else{
        for(int j = 0; j < 32; j++){
          ligne_binaire[j] = ligne_binaire_B[j];
        }
      }
      char * ligne_binaire_rn = calloc(O,sizeof(char));
      ligne_binaire_rn = code_S_binaire(tab_registre[l->rn],32);
      if (adr5 == adr6){
        for(int o = 31; o >= 0 ; o--){
          ligne_binaire[o]=ligne_binaire_rn[o];
        }
        int * tab_binaire = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        if((adr6 > 0) && (tee==0)){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          l15->suiv=l14;
          l14->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          l14->suiv=NULL;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6);
          c2->premier = l14;
          l14->suiv = l16->suiv;
        }
      }
      else if(adr5 == adr6+1){
        char * ligne_binaire2_B= calloc(O,sizeof(char));
        char * ligne_binaire2 = calloc(O,sizeof(char));
        ligne_binaire2_B = rech_adr(c2,adr6+4);
        if ( ligne_binaire2_B == NULL){
          tee=1;
          code_ajout_ligne(c2,0,0,0,0,0,adr6+4,NULL,NULL);
          ligne_binaire=rech_adr(c2,adr6+4);
        }
        else{
          for(int j = 0; j < 32; j++){
            ligne_binaire2[j] = ligne_binaire2_B[j];
          }
        }
        for(int o = 31; o >= 8 ; o--){
          ligne_binaire[o-8] = ligne_binaire_rn[o];
        }
        for(int o = 31; o >= 24 ; o--){
          ligne_binaire2[o] = ligne_binaire_rn[o-24];
        }
        int * tab_binaire = malloc(sizeof(int));
        int * tab_binaire2 = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        for(int f = 0; f < 32; f++){
          if (ligne_binaire2[f]=='1'){
            tab_binaire2[f] = 1;
          }
          else{
            tab_binaire2[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        int * tab_instruction2 = binaire_a_decimal(tab_binaire2);
        if((adr6 > 0) && (tee==0)){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          l15->suiv=l14;
          l14->suiv=l18;
          l18->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          l18->suiv=NULL;
          l14->suiv=l18;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          c2->premier = l14;
          l14->suiv=l18;
          l18->suiv = l16->suiv;
        }
      }
      else if(adr5 == adr6+2){
        char * ligne_binaire2 = calloc(O,sizeof(char));
        ligne_binaire2 = rech_adr(c2,adr6+4);
        if ( ligne_binaire2 == NULL){
          printf("\nErreur, cette adresse n'existe pas.\n");
          return 0;
        }
        for(int o = 31; o >= 16 ; o--){
          ligne_binaire[o-16]=ligne_binaire_rn[o];
        }
        for(int o = 31; o >= 16 ; o--){
          ligne_binaire2[o] = ligne_binaire_rn[o-16];
        }
        int * tab_binaire = malloc(sizeof(int));
        int * tab_binaire2 = malloc(sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        for(int f = 0; f < 32; f++){
          if (ligne_binaire2[f]=='1'){
            tab_binaire2[f] = 1;
          }
          else{
            tab_binaire2[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        int * tab_instruction2 = binaire_a_decimal(tab_binaire2);
        if((adr6 > 0) && (tee==0)){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          l15->suiv=l14;
          l14->suiv=l18;
          l18->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          l18->suiv=NULL;
          l14->suiv=l18;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          c2->premier = l14;
          l14->suiv=l18;
          l18->suiv = l16->suiv;
        }
      }
      else{
        char * ligne_binaire2 = calloc(O,sizeof(char));
        ligne_binaire2 = rech_adr(c2,adr6+4);
        if ( ligne_binaire2 == NULL){
          printf("\nErreur, cette adresse n'existe pas.\n");
          return 0;
        }
        for(int o = 31; o >= 24 ; o--){
          ligne_binaire[o-24]=ligne_binaire_rn[o];
        }
        for(int o = 31; o >= 8 ; o--){
          ligne_binaire2[o] = ligne_binaire_rn[o-8];
        }
        int * tab_binaire = malloc(O*sizeof(int));
        int * tab_binaire2 = malloc(O*sizeof(int));
        for(int f = 0; f < 32; f++){
          if (ligne_binaire[f]=='1'){
            tab_binaire[f] = 1;
          }
          else{
            tab_binaire[f] = 0;
          }
        }
        for(int f = 0; f < 32; f++){
          if (ligne_binaire2[f]=='1'){
            tab_binaire2[f] = 1;
          }
          else{
            tab_binaire2[f] = 0;
          }
        }
        int * tab_instruction = binaire_a_decimal(tab_binaire);
        int * tab_instruction2 = binaire_a_decimal(tab_binaire2);
        if(adr6 > 0){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l15 = recherche_sauts_ex(c2,adr6-4);
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          l15->suiv=l14;
          l14->suiv=l18;
          l18->suiv=l16->suiv;
        }
        else if(tee==1){
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          l18->suiv=NULL;
          l14->suiv=l18;
          Ligne * parcoureur = c2->premier;
          while(parcoureur->suiv->suiv != NULL){
            parcoureur=parcoureur->suiv;
          }
          parcoureur->suiv=l14;
        }
        else{
          Ligne * l14 = creerLigne( tab_instruction[0],tab_instruction[1],tab_instruction[2],tab_instruction[3],tab_instruction[4],adr6, NULL,NULL );
          Ligne * l18 = creerLigne( tab_instruction2[0],tab_instruction2[1],tab_instruction2[2],tab_instruction2[3],tab_instruction2[4],adr6+4, NULL,NULL );
          Ligne * l16 = recherche_sauts_ex(c2,adr6+4);
          c2->premier = l14;
          l14->suiv=l18;
          l18->suiv = l16->suiv;
        }
      }
      if (tab_registre[l->rn]==0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(tab_registre[l->rn]<0){
        n = 1;
      }
      else{
        n = 0;
      }
      c = 0;
    }

    /*---Exécution Des Instructios De Sauts---*/
    //On cherche l'adresse où le saut va être effectué puis on remplace la ligne courante par la ligne où nous devons sauter
    //exécute opérande 14 (jmp)
    if (l->code_op == 14){
      if(l->imm == 0){
        tab_registre[32] = tab_registre[l->s];
        //Cette fonction cherche l'adresse dans la structure contenant les lignes et renvoie le pointeur sur la ligne
        Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
        l_jmp->suiv = recherche_sauts_ex(c2,tab_registre[l->s]);
        l = l_jmp;
      }
      else{
        tab_registre[32] = l->s;
        Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
        l_jmp->suiv= recherche_sauts_ex(c2,l->s);
        l = l_jmp;
      }
    }
    //exéctue opérande 15 (jzs)
    if (l->code_op == 15){
      if(z == 1) {
        if(l->imm == 0){
          tab_registre[32] = tab_registre[l->s];
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv = recherche_sauts_ex(c2,tab_registre[l->s]);
          l = l_jmp;
        }
        else{
          tab_registre[32] = l->s;
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv= recherche_sauts_ex(c2,l->s);
          l = l_jmp;
        }
      }
    }
    //exécute opérande 16 (jzc)
    if (l->code_op == 16){
      if(z == 0) {
        if(l->imm == 0){
          tab_registre[32] = tab_registre[l->s];
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv = recherche_sauts_ex(c2,tab_registre[l->s]);
          l = l_jmp;

        }
        else{
          tab_registre[32] = l->s;
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv= recherche_sauts_ex(c2,l->s);
          l = l_jmp;
        }
      }
    }
    //exécute opérande 17 (jcs)
    if (l->code_op == 17){
      if(c == 1) {
        if(l->imm == 0){
          tab_registre[32] = tab_registre[l->s];
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv = recherche_sauts_ex(c2,tab_registre[l->s]);
          l = l_jmp;
        }
        else{
          tab_registre[32] = l->s;
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv= recherche_sauts_ex(c2,l->s);
          l = l_jmp;
        }
      }

    }
    //exécute opérande 18 (jcc)
    if (l->code_op == 18){
      if(c == 0) {
        if(l->imm == 0){
          tab_registre[32] = tab_registre[l->s];
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv = recherche_sauts_ex(c2,tab_registre[l->s]);
          l = l_jmp;

        }
        else{
          tab_registre[32] = l->s;
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv= recherche_sauts_ex(c2,l->s);
          l = l_jmp;
        }
      }

    }
    //exécute opérande 19 (jns)
    if (l->code_op == 19){
      if(n == 1) {
        if(l->imm == 0){
          tab_registre[32] = tab_registre[l->s];
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv = recherche_sauts_ex(c2,tab_registre[l->s]);
          l = l_jmp;

        }
        else{
          tab_registre[32] = l->s;
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv= recherche_sauts_ex(c2,l->s);
          l = l_jmp;
        }
      }
    }
    //exécute opérande 20 (jns)
    if (l->code_op == 20){
      if(n == 0) {
        if(l->imm == 0){
          tab_registre[32] = tab_registre[l->s];
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv = recherche_sauts_ex(c2,tab_registre[l->s]);
          l = l_jmp;
        }
        else{
          tab_registre[32] = l->s;
          Ligne * l_jmp = creerLigne(0,0,0,0,0,0,NULL,NULL);
          l_jmp->suiv= recherche_sauts_ex(c2,l->s);
          l = l_jmp;
        }
      }
    }

    /*---Exécution Des Instructios D'Entrée-Sortie---*/
    //exécute opérande 21 (in)
    if (l->code_op == 21){
      printf("\nEntrer une valeur pour le registre numéro %d :\n", l->rd );
      scanf("%d", &tab_registre[l->rd] );
      //vérifie si le nombre entrée est dans la plage des nombres
      if(tab_registre[l->rd]>= -pow(2,31) && tab_registre[l->rd]<= pow(2,31)-1){
        if(tab_registre[l->rd] == 0){
          z = 1;
        }
        else{
          z = 0;
        }
        if(tab_registre[l->rd] < 0){
          n = 1;
        }
        else{
          n = 0;
        }
        c = 0;
      }
      else{
        printf("\nErreur, le nombre entré n'est pas entier pris en charge. " );
        return 0;
      }

    }
    //exécute opérande 22 (out)
    if(l->code_op == 22){
      printf("\nVoici le contenu du registre %d : %d \n", l->rd, tab_registre[l->rd] );
      if(l->rd == 0){
        z = 1;
      }
      else{
        z = 0;
      }
      if(l->rd < 0){
        n = 1;
      }
      else{
        n = 0;
      }
      c = 0;
    }
    /*---Exécution Diverses---*/
    //exécute opérande 23 (rnd)
    if(l->code_op == 23){
      srand(time(NULL));
      if(l->imm == 0){
        double x1 = tab_registre[l->s]-1 , x2 = tab_registre[l->rd];
        printf("%f %f\n",x2, x1 );
        int p1 = fmin(x2,x1), p2 = fmax(x2,x1);
        printf("%d %d\n",p1 ,p2 );
        tab_registre[l->rd] =  p1 + rand() % (p2 + 1 - p1);

        printf("%d\n", tab_registre[l->rd] );

        if(tab_registre[l->rd] == 0){
          z = 1;
        }
        else{
          z = 0;
        }
        if(tab_registre[l->rd] < 0){
          n = 1;
        }
        else{
          n = 0;
        }
        c = 0;
      }
      else{
        double x1 = l->s-1, x2 = tab_registre[l->rd];
        int p1 = fmin(x2,x1), p2 = fmax(x2,x1);
        tab_registre[l->rd] =  p1 + rand() % (p2 + 1 - p1);
        if(tab_registre[l->rd] == 0){
          z = 1;
        }
        else{
          z = 0;
        }
        if(tab_registre[l->rd] < 0){
          n = 1;
        }
        else{
          n = 0;
        }
        c = 0;
      }

    }
    //exécute opérande 31 (hlt)
    if(l->code_op == 31){
      printf("\nFin de l'exécution du programme.\n");
      return 0;
      z = 0;
      c = 0;
      n = 0;
    }

    tab_registre[32] += 4;
    tab_registre[0] = 0;
    l = l->suiv;

    //On vérifie les registres d'état après chaque instruction
    //printf("z  : %d - c : %d - n : %d - pc : %d\n",z,c,n,tab_registre[32] );
  }
  return 1;
}

/*---MAIN---*/
int main(int argc, char * argv[] ){

  // test pour voir si l'utilisateur rentre bien le nom du fihcier texte
  if (argc < 2){
    printf("\nEntrer un nom de fichier d'assembleur \n");
    return 0;
  }
  //Représente l'adresse de chaque ligne du code assembleur
  int adr = 0;
  //Chaine qui va stocker une ligne (donc une instruction)
  char * ligne_fichier = malloc(O * sizeof(char));
  char * ligne_fichier1 = malloc(O * sizeof(char));

  //On ouvre le ficher texte puis on vérifie si celui-ci s'est ouvert sans problème
  FILE * fic_ass = fopen(argv[1],"r");
  if (fic_ass == NULL){
    printf("\nNe nous arrivons pas à lire le fichier assembleur.\n");
    return 0;
  }
  //Cette boucle permet de vérifier si le fichier ne dépasse pas la mémoire de notre machine qui est de 64kilos-octets
  int compte_ligne = 0;
  while(fgets(ligne_fichier1,O,fic_ass) != NULL){
    compte_ligne ++;
  }
  //Vérifie si le fichier atteint la limite de notre machine virtuelle
  if (compte_ligne > O) {
    printf("\nErreur de segmentation, le fichier est trop volumineux.\n" );
    return 0;
  }

  /*--------Partie Traduction Et Écriture--------*/

  Code_a * c1 = creerCode_a();
  Code_a * c2 = creerCode_a();

  rewind(fic_ass);

  //Variable qui va tester s'il y a des erreurs
  int test;
  //boucle qui lit ligne par ligne le code d'un fichier assembleur
  while(fgets(ligne_fichier,N,fic_ass) != NULL){


    test = traduit_fichier_assembleur(ligne_fichier,adr,c1);
    if (test == 0 ) return 0;
    // adresse de chaque ligne
    adr += 4;
  }

  //recherche les fautes au niveau des sauts
  int faute_jmp;
  faute_jmp = rech_sauts(c1);
  if (faute_jmp != 0){
    printf("Erreur à la ligne %d \n", (faute_jmp/4)+1);
    return 0;
  }

  //affiche la structure code_as pour pouvoir vérifier les erreurs
  //afficher_code(c1);
  fclose(fic_ass);
  //Créer le fichier hexa.txt et y écrit la traduction en hexadécimal
  FILE * fic_hexa = fopen("hexa.txt", "w");
  trad_ecriture(c1,fic_hexa);
  fclose(fic_hexa);
  free(ligne_fichier);
  free(ligne_fichier1);
  libere_code(c1);


  /*--------Partie simulation--------*/

  FILE * fic_hexa_simulation = fopen("hexa.txt","r");
  if(fic_hexa_simulation == NULL){
    printf("\nErreur d'ouverture.\n");
    return 0;
  }
  char * ligne_ficher_hexa = malloc(O * sizeof(char));

  int adr2 = 0;
  // crée le nouveau code avec le fichier texte hexa.txt
  while(fgets(ligne_ficher_hexa,O,fic_hexa_simulation) != NULL){
    int * tab = calloc(1,sizeof(int));
    tab = hexa_a_bin(ligne_ficher_hexa);
    int * tab1 = calloc(1,sizeof(int));
    tab1 = binaire_a_decimal(tab);
    code_ajout_ligne(c2,tab1[0],tab1[1],tab1[2],tab1[3],tab1[4],adr2,NULL,NULL);
    adr2 += 4;
    free(tab);
    free(tab1);
    free(binaire_a_decimal(tab));
    free(binaire_a_decimal(tab));
  }
    int test2 = 1;
    //Exécute les instructions une à un
    test2 = execute(c2);
    if(test2 == 0){
      return 0;
    }
    //affiche le deuxième code traduit directement depuis le fichier hexa.txt avec les instructions exécutées
    //afficher_code(c2);

    libere_code(c2);
    free(ligne_ficher_hexa);
    fclose(fic_hexa_simulation);
    return 0;
  }
