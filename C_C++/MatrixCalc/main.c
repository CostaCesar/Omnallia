#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "matrixLib.h"

#define COMMAND_SIZE 128

Matrix* get_Matrix()
{
    int x = 0, y = 0;
    printf("> Tamanho da matriz [Y X]: ");
    scanf("%d %d", &x, &y);

    if(x < 1 || y < 1)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Tamanho invalido! \n");
        return NULL;
    }

    Matrix * out = alloc_Matrix(x, y);
    if(out == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        return NULL;
    }
    for(int i = 0; i < out->Ysize; i++)
    {
        printf(">> Lin %d \n", i);
        for(int ii = 0; ii < out->Xsize; ii++)
        {
            printf("> Col %d: ", ii);
            scanf("%lf", &out->matrix[i][ii]);
        }
    }
    
    printf("$ Matriz preenchida com sucesso! \n");
    return out;
}

int main(int argc, char **argv)
{
    char command[COMMAND_SIZE], action;
    Matrix *A_Matrix = NULL, *B_Matrix = NULL, *RES_Matrix = NULL;

    do
    {
        putchar('\n');
        printf("<[Q] Inserir Matriz 1> \n");
        printf("<[A] Inserir Matriz 2> \n");
        printf("<[W] Ver Matriz 1> \n");
        printf("<[S] Ver Matriz 2> \n");
        printf("<[C] Multiplicar Matriz 1 X Numero N> \n");
        printf("<[E] Multiplicar Matriz 1 x Matriz 2> \n");
        printf("<[D] Multiplicar Matriz 2 x Matriz 1> \n");
        printf("<[Z] Obter Matriz Inversa Da Matriz 1> \n");
        printf("<[.] Testar uma funcao nova> \n");
        printf("<[X] Sair> \n");
        fflush(stdin);
        scanf("%c", &action);

        switch (action)
        {
            case 'q':
            case 'Q':
                free_Matrix(A_Matrix);
                A_Matrix = get_Matrix();
                break;
            case 'a':
            case 'A':
                free_Matrix(B_Matrix);
                B_Matrix = get_Matrix();
                break;
            case 'w':
            case 'W':
                show_Matrix(A_Matrix);
                break;
            case 's':
            case 'S':
                show_Matrix(B_Matrix);
                break;
            case 'e':
            case 'E':
                RES_Matrix = multiply_Matrixes(A_Matrix, B_Matrix);
                show_Matrix(RES_Matrix);
                break;
            case 'd':
            case 'D':
                RES_Matrix = multiply_Matrixes(A_Matrix, B_Matrix);
                show_Matrix(RES_Matrix);
                break;
            case 'c':
            case 'C':
                RES_Matrix = multiply_MatrixByN(A_Matrix);
                show_Matrix(RES_Matrix);
                break;
            case 'z':
            case 'Z':
                RES_Matrix = getInverse_Matrix(A_Matrix);
                show_Matrix(RES_Matrix);
                break;
            case '.':
                show_Matrix(A_Matrix);
                printf("> POS: %d", find_MatrixRow_Element_AtCol(A_Matrix, 2, true));
                break;
            case 'x':
            case 'X':
                if(RES_Matrix != NULL)
                    free_Matrix(RES_Matrix);
                free_Matrix(A_Matrix);
                free_Matrix(B_Matrix);
                printf("$ Saindo... \n");
                return 0;
            default:
                printf("$ Comando Invalido! \n");
                break;
        }
        if(RES_Matrix != NULL)
            free_Matrix(RES_Matrix);
    } while (1);

    return 1;
}