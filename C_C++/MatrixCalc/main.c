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

typedef struct P_reMatrix
{
    double ***matrix;
    double ***coefic;
    int coefic_Size;
    int Xsize;
    int Ysize;
} VariableMatrix;


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

void free_VarMatrix(VariableMatrix* freed)
{
    if(freed != NULL)
    {
        if(freed->matrix != NULL)
        {
            for(int i = 0; i < freed->Ysize; i++)
            {
                if(freed->matrix[i] != NULL)
                {
                    for(int j = 0; j < freed->Xsize; j++)
                    {
                        if(freed->matrix[i][j] != NULL)
                            free(freed->matrix[i][j]);
                    }
                    free(freed->matrix[i]);
                }
            }
            free(freed->matrix);
        }
        if(freed->coefic != NULL)
        {
            for(int i = 0; i < freed->Ysize; i++)
            {
                if(freed->coefic[i] != NULL)
                {
                    for(int j = 0; j < freed->Xsize; j++)
                    {
                        if(freed->coefic[i][j] != NULL)
                            free(freed->coefic[i][j]);
                    }
                    free(freed->coefic[i]);
                }
            }
            free(freed->coefic);
        }
        free(freed);
    }
    return;
}

VariableMatrix* alloc_VarMatrix(int Xsize, int Ysize, int Csize)
{
    VariableMatrix *output = (VariableMatrix *) malloc(sizeof(VariableMatrix));
    output->Xsize = Xsize, output->Ysize = Ysize, output->coefic_Size = Csize;

    output->matrix = (double ***) malloc(Ysize * sizeof(double**));
    if(output->matrix == NULL)
        return NULL;
    output->coefic = (double ***) malloc(Ysize * sizeof(double**));
    if(output->coefic == NULL)
    { free_VarMatrix(output); return NULL; }

    for(int i = 0; i < Ysize; i++)
    {
        output->matrix[i] = (double **) malloc(Xsize * sizeof(double *));
        if(output->matrix[i] == NULL)
        { free_VarMatrix(output); return NULL; }
        for(int j = 0; j < Xsize; i++)
        {
            output->matrix[i][j] = (double *) malloc(Csize * sizeof(double));
            if(output->matrix[i][j] == NULL)
            { free_VarMatrix(output); return NULL; }
        }
    }
    return output->matrix ;
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

Matrix* getInverse_Matrix(Matrix *A)
{
    if(A->Xsize != A->Ysize)
    {
        printf("# A matriz nao possui um tamanho compativel! \n");
        return NULL;
    }
    
    Matrix* identity = alloc_Matrix(A->Xsize, A->Ysize);
    if(identity == NULL)
    {
        printf("# Incapaz de incializar matriz identidade! \n");
        return NULL;
    }
    for(int i = 0; i < identity->Xsize; i++)
    {
        for(int j = 0; j < identity->Xsize; j++)
        {
            if(i == j)
                identity->matrix[i][j] = 1;
            else
                identity->matrix[i][j] = 0;
        }
    }

    VariableMatrix* result = alloc_VarMatrix(A->Xsize, A->Ysize, A->Xsize);
    if(result == NULL)
    {
        printf("# Incapaz de incializar matriz inversa! \n");
        return NULL;
    }
    for(int i = 0; i < result->Xsize; i++)
    {
        for(int j = 0; j < result->Xsize; j++)
        {
            for(int k = 0; k < result->coefic_Size; k++)
                result
        }
    }

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
        printf("<[Z] Obter Matriz Inversa Da Matriz 1 > \n");
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