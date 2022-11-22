#include <stdio.h>
#include <stdlib.h>
#include "matrixLib.h"

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

void swap_MatrixRows(Matrix *A, int rowA, int rowB)
{
    if(rowA < 0 || rowB < 0 || rowA >= A->Ysize || rowB >= A->Ysize)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Tamanhos invavidos! \n");
        return;
    }

    double *buffer = (double *) malloc(A->Xsize * sizeof(double));
    for(int i = 0; i < A->Xsize; i++)
    {
        buffer[i] = A->matrix[rowA][i];
        A->matrix[rowA][i] = A->matrix[rowB][i];
        A->matrix[rowB][i] = buffer[i];
    }
    free(buffer);
    return;
}

Matrix* extract_Matrix(Matrix *extractFrom, int rowStart, int colStart, int rowEnd, int colEnd)
{
    if(rowStart < 0 || rowEnd < 0 || rowStart >= extractFrom->Ysize || rowEnd >= extractFrom->Ysize || rowStart > rowEnd)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Tamanhos de linha invavidos! \n");
        return NULL;
    }
    if(colStart < 0 || colEnd < 0 || colStart >= extractFrom->Xsize || colEnd >= extractFrom->Xsize || colStart > colEnd)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Tamanhos de coluna invavidos! \n");
        return NULL;
    }
    int rowDif = rowEnd - rowStart + 1, colDif = colEnd - colStart + 1;

    Matrix* result = (Matrix *) malloc(sizeof(Matrix));
    if(result == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Incapaz de inicializar resultado! \n");
        return NULL;
    } 
    result->Xsize = colDif, result->Ysize = rowDif;
    result->matrix = alloc_Matrix(result->Xsize, result->Ysize);
    if(result->matrix == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Incapaz de inicializar matriz! \n");
        free_Matrix(result);
        return NULL;
    }

    for(int i = rowStart; i <= rowEnd; i++)
    {
        for(int j = colStart; j <= colEnd; j++)
            result->matrix[i-rowStart][j-colStart] = extractFrom->matrix[i][j];
    }

    return result;
}

Matrix *extract_Row(Matrix* A, int row)
{
    if(row < 0 || row > A->Ysize)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Linha invalida! \n");
        return NULL;
    }

    return extract_Matrix(A, row, 0, row, A->Xsize-1);
}

Matrix *join_Matrix_Left(Matrix *Base, Matrix *Add)
{
    if(Base == NULL || Add == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Argumentos invalidos! \n");
        return NULL;
    }
    if(Base->Ysize != Add->Ysize)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Tamanhos diferentes nao sao permitidos! \n");
        return NULL;
    }


    Matrix* result = (Matrix *) malloc(sizeof(Matrix));
    if(result == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Incapaz de inicializar resultado! \n");
        return NULL;
    }
    
    result->Xsize = Base->Xsize + Add->Xsize, result->Ysize = Base->Ysize;
    result->matrix = alloc_Matrix(result->Xsize, result->Ysize);
    if(result->matrix == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Incapaz de inicializar matriz! \n");
        free_Matrix(result);
        return NULL;
    }

    for(int i = 0; i < Base->Ysize; i++)
    {
        int resultPos = 0;
        for(int j = 0; j < Base->Xsize; j++)
        {
            result->matrix[i][j] = Base->matrix[i][j];
            resultPos = j;
        }
        resultPos++;
        for(int j = 0; j < Base->Xsize; j++)
        {
            result->matrix[i][resultPos] = Add->matrix[i][j];
            resultPos++;
        }
    }

    return result;
}

void split_Matrix_AtCol(Matrix *origin, Matrix **left, Matrix **right, int column)
{
    if(origin == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Argumento invalido! \n");
        return;
    }
    if(column < 0|| column > origin->Xsize)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Coluna invalida! \n");
        return;
    }

    if(*left != NULL)
        free_Matrix(*left);
    if(*right != NULL)
        free_Matrix(*right);

    
    *left = extract_Matrix(origin, 0, 0, origin->Ysize-1, column);
    if(*left == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        return;
    }
    *right = extract_Matrix(origin, 0, column+1, origin->Ysize-1, origin->Xsize-1);
    if(*right == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        return;
    }

    return;
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