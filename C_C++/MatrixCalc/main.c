#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define COMMAND_SIZE 128

typedef struct P_Matrix
{
    double **matrix;
    int Xsize;
    int Ysize;
} Matrix;

void free_Matrix(Matrix* freed)
{
    if(freed != NULL)
    {
        if(freed->matrix != NULL)
        {
            for(int i = 0; i < freed->Ysize; i++)
            {
                if(freed->matrix[i] != NULL)
                    free(freed->matrix[i]);
            }
            free(freed->matrix);
            }
        free(freed);
    }
    return;
}

double** alloc_Matrix(int Xsize, int Ysize)
{
    double **out = (double **) malloc(Ysize * sizeof(double*));
    if(out == NULL)
        return NULL;
    for(int i = 0; i < Ysize; i++)
    {
        out[i] = (double *) malloc(Xsize * sizeof(double));
        if(out[i] == NULL)
        {
            for(int ii = 0; i < i; ii++)
                free(out[ii]);
            free(out);
            return NULL;
        }
    }
    return out;
}

void show_Matrix(Matrix* show)
{
    if(show == NULL)
    {
        printf("# Argumentos invalidos! \n");
        return;
    }
    if(show->Xsize < 1 || show->Ysize < 1)
    {
        printf("# Tamanhos fora de escala! \n");
        return;
    }

    for(int i = 0; i < show->Ysize; i++)
    {
        printf("<<<Lin %d>>> \n", i);
        for(int ii = 0; ii < show->Xsize; ii++)
            printf("<Col %d>: %.4lf ", ii, show->matrix[i][ii]);
        putchar('\n');
    }
    return;
}

Matrix* get_Matrix()
{
    Matrix *out = (Matrix *) malloc(sizeof(Matrix));
    if(out == NULL)
    {
        printf("# Incapaz de incializar matriz! \n");
        return NULL;
    }
    printf("> Tamanho da matriz [Y X]: ");
    scanf("%d %d", &out->Ysize, &out->Xsize);

    if(out->Xsize < 0 || out->Ysize < 0)
    {
        printf("# Tamanho invalido! \n");
        return NULL;
    }

    out->matrix = alloc_Matrix(out->Xsize, out->Ysize);
    if(out->matrix == NULL)
    {
        printf("# Incapaz de incializar valores da matriz! \n");
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

Matrix* multiply_MatrixByN(Matrix* A)
{
    if(A == NULL)
    {
        printf("# Argumentos invalidos! \n");
        return NULL;
    }
    if(A->Xsize < 1)
    {
        printf("# Tamanhos fora de escala! \n");
        return NULL;
    }

    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    if(res == NULL)
    {
        printf("# Incapaz de incializar matriz! \n");
        return NULL;
    }
    res->Xsize = A->Xsize, res->Ysize = A->Ysize;
    
    res->matrix = alloc_Matrix(res->Xsize, res->Ysize);
    if(res->matrix == NULL)
    {
        printf("# Incapaz de incializar valores matriz! \n");
        return NULL;
    }

    double N = 1.0;
    printf("> Digite o numero pelo  qual multiplicar: ");
    scanf("%lf", &N);

    for(int i = 0; i < res->Ysize; i++)
    {
        for(int ii = 0; ii < res->Xsize; ii++)
            res->matrix[i][ii] = A->matrix[i][ii] * N;
    }
    printf("$ Multiplicacao concluida com sucesso! \n");
    return res;
}

Matrix* multiply_Matrixes(Matrix *A, Matrix *B)
{
    if(A == NULL || B == NULL)
    {
        printf("# Argumentos invalidos! \n");
        return NULL;
    }
    if(A->Xsize != B->Ysize)
    {
        printf("# Tamanhos fora de escala! \n");
        return NULL;
    }

    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    if(res == NULL)
    {
        printf("# Incapaz de incializar matriz! \n");
        return NULL;
    }

    res->Xsize = B->Xsize, res->Ysize = A->Ysize;
    res->matrix = alloc_Matrix(res->Xsize, res->Ysize);
    if(res->matrix == NULL)
    {
        printf("# Incapaz de incializar valores matriz! \n");
        return NULL;
    }

    for(int i = 0; i < res->Ysize; i++)
    {
        for(int ii = 0; ii < res->Xsize; ii++)
        {
            res->matrix[i][ii] = 0;
            for(int xA = 0; xA < A->Xsize; xA++)
            {
                res->matrix[i][ii] += A->matrix[i][xA] * B->matrix[xA][ii];
            }
        }
    }
    printf("$ Multiplicacao concluida com sucesso! \n");
    return res;
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
                free_Matrix(RES_Matrix);
                RES_Matrix = multiply_Matrixes(A_Matrix, B_Matrix);
                show_Matrix(RES_Matrix);
                break;
            case 'd':
            case 'D':
                free_Matrix(RES_Matrix);
                RES_Matrix = multiply_Matrixes(A_Matrix, B_Matrix);
                show_Matrix(RES_Matrix);
                break;
            case 'c':
            case 'C':
                free_Matrix(RES_Matrix);
                RES_Matrix = multiply_MatrixByN(A_Matrix);
                show_Matrix(RES_Matrix);
                break;
            case 'x':
            case 'X':
                free_Matrix(A_Matrix);
                free_Matrix(B_Matrix);
                free_Matrix(RES_Matrix);
                printf("$ Saindo... \n");
                return 0;
            default:
                printf("$ Comando Invalido! \n");
                break;
        }
    } while (1);

    return 1;
}