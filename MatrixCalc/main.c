#include <stdio.h>
#include "matrixLib.h"

#define COMMAND_SIZE 128

typedef enum E_BOOL
{
    True = 1,
    False = 0
} Bool;

Matrix* get_Matrix()
{
    int x = 0, y = 0;
    printf("> Tamanho da matriz [Y X]: ");
    scanf("%d %d", &y, &x);

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
    Matrix *A_Matrix = NULL, *B_Matrix = NULL, *RES_Matrix = NULL, *LastRes_Matrix = NULL, *Buffer = NULL;
    Bool saveRes;

    do
    {
        putchar('\n');
        printf("<[Q] Inserir Matriz 1> \n");
        printf("<[A] Inserir Matriz 2> \n");
        printf("<[Z] Inserir Matriz Resultado Em> \n");
        printf("<[W] Ver Matriz 1> \n");
        printf("<[S] Ver Matriz 2> \n");
        printf("<[X] Ver Matriz Resultado> \n");
        printf("<[O] Trocar Matriz 1 Por Matriz 2> \n");
        printf("<[C] Multiplicar Matriz 1 X Numero N> \n");
        printf("<[E] Multiplicar Matriz 1 x Matriz 2> \n");
        printf("<[D] Multiplicar Matriz 2 x Matriz 1> \n");
        printf("<[R] Obter Determinante Da Matriz 1> \n");
        printf("<[T] Obter Matriz Inversa Da Matriz 1> \n");
        printf("<[P] Sair> \n");
        fflush(stdin);
        scanf("%c", &action);

        switch (action)
        {
            case 'q':
            case 'Q':
                A_Matrix = free_Matrix(A_Matrix);
                A_Matrix = get_Matrix();
                break;
            case 'a':
            case 'A':
                B_Matrix = free_Matrix(B_Matrix);
                B_Matrix = get_Matrix();
                break;
            case 'z':
            case 'Z':
                printf(">Inserir em [1] ou [2]: ");
                scanf("%d", &action);
                if(action == 1)
                {
                    A_Matrix = free_Matrix(A_Matrix);
                    A_Matrix = clone_Matrix(LastRes_Matrix);
                }
                else if(action == 2)
                {
                    B_Matrix = free_Matrix(B_Matrix);
                    B_Matrix = clone_Matrix(LastRes_Matrix);
                }
                else
                {
                    printf("# Numero da Matriz invalido! \n");
                }
                break;
            case 'w':
            case 'W':
                show_Matrix(A_Matrix);
                break;
            case 's':
            case 'S':
                show_Matrix(B_Matrix);
                break;
            case 'x':
            case 'X':
                show_Matrix(LastRes_Matrix);
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
            case 't':
            case 'T':
                RES_Matrix = getInverse_Matrix(A_Matrix); 
                show_Matrix(RES_Matrix);
                break;
            case 'r':
            case 'R':
                show_Matrix(A_Matrix);
                printf("\n$ Determinante da matriz: %.5lf $ \n", getDeterminant(A_Matrix));
                break;
            case 'o':
            case 'O':
                Buffer = A_Matrix; A_Matrix = B_Matrix; B_Matrix = Buffer;
                printf("\n$ Matrizes trocadas! $ \n");
                break;
            case 'p':
            case 'P':
                RES_Matrix = free_Matrix(RES_Matrix);
                A_Matrix = free_Matrix(A_Matrix);
                B_Matrix = free_Matrix(B_Matrix);
                printf("$ Saindo... \n");
                return 0;
            default:
                printf("$ Comando Invalido! \n");
                break;
        }
        if(isEmpty_Matrix(RES_Matrix))
        {
            LastRes_Matrix = free_Matrix(LastRes_Matrix);
            LastRes_Matrix = clone_Matrix(RES_Matrix);
        }
        RES_Matrix = free_Matrix(RES_Matrix);
    } while (1);

    return 1;
}