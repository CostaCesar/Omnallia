#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

void copy_Matrix(Matrix *source, Matrix *destiny)
{
    if(source == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Argumento de entrada invalido \n");
        return;
    }
    if(destiny == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Argumento de saida invalido \n");
        return;
    }
    if(source->Xsize != destiny->Xsize || source->Ysize != destiny->Ysize)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Tamanhos diferentes nao sao permitidos \n");
        return;
    }

    for(int i = 0; i < source->Ysize; i++)
    {
        for(int j = 0; j < source->Xsize; j++)
            destiny->matrix[i][j] = source->matrix[i][j];
    }
    return;
}

void show_Matrix(Matrix* show)
{
    if(show == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Argumentos invalidos! \n");
        return;
    }
    if(show->Xsize < 1 || show->Ysize < 1)
    {
        printf("ERROR: %s", __FUNCTION__);
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
        printf("# Nao foi possivel extrair lado esquerdo \n");
        return;
    }
    *right = extract_Matrix(origin, 0, column+1, origin->Ysize-1, origin->Xsize-1);
    if(*right == NULL)
    {
        printf("ERROR: %s", __FUNCTION__);
        printf("# Nao foi possivel extrair lado direito \n");
        return;
    }

    return;
}

int find_MatrixRow_Element_AtCol(Matrix *A, int col, int biggest)
{
    if(A == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Matriz invalido \n");
        return -1;
    }
    if(col < 0 || col >= A->Xsize)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Coluna invalida \n");
        return -1;
    }
    int resultN, resultI = -1;
    if(biggest == 0)
        resultN = INT_MAX;
    else resultN = INT_MIN;

    for(int i = 0; i < A->Ysize; i++)
    {
        if((biggest == 0 && A->matrix[i][col] < resultN) || (A->matrix[i][col] > resultN))
            resultI = i, resultN = A->matrix[i][col];
    }   

    return resultI;
}

void multiply_MatrixRow(Matrix *A, int row, double num)
{
    if(A == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Matriz invalido \n");
        return;
    }
    if(row < 0 || row > A->Ysize)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Tamanho invalido \n");
        return;
    }

    for(int i = 0; i < A->Xsize; i++)
        A->matrix[row][i] *= num;
    return;
}

Matrix *add_Matrix_Part(Matrix *main, Matrix *add, int atRow, int atCol, int untilRow, int untilCol)
{
    if(main == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Argumento base invalido \n");
        return NULL;
    }
    if(add == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Argumento de soma invalido \n");
        return NULL;
    }
    if(atRow < 0 || atRow >= main->Ysize || atCol < 0 || atCol >= main->Xsize)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Posicao inicial invalida \n");
        return NULL;
    }
    if( untilRow < 0 || untilRow >= add->Ysize || untilRow > main->Ysize || untilRow < atRow ||
        untilCol < 0 || untilCol >= add->Xsize || untilCol > main->Xsize || untilCol < atCol)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Posicao final invalida \n");
        return NULL;
    }

    Matrix *res = (Matrix *) malloc(sizeof(Matrix));
    if(res == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Incapaz de incializar matriz! \n");
        return NULL;
    }
    res->Xsize = main->Xsize, res->Ysize = main->Ysize;
    
    res->matrix = alloc_Matrix(res->Xsize, res->Ysize);
    if(res->matrix == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Incapaz de incializar valores matriz! \n");
        return NULL;
    }

    int fromRow = 0, fromCol = 0;
    for(int i = 0; i < res->Ysize; i++)
    {
        for(int j = 0; j < res->Xsize; j++)
        {
            res->matrix[i][j] = main->matrix[i][j];
            if((i >= atRow && j >= atCol) && (i <= untilRow && j <= untilCol))
                res->matrix[i][j] += add->matrix[i-atRow][j-atCol];
        }
    }
    
    return res;
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
    
    Matrix* identity = (Matrix *) malloc(sizeof(Matrix));
    identity->matrix = alloc_Matrix(A->Xsize, A->Ysize);
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

    Matrix *result = (Matrix *) malloc(sizeof(Matrix));
    result->matrix = alloc_Matrix(A->Xsize, A->Ysize);
    if(result->matrix == NULL)
    {
        printf("# Incapaz de incializar matriz identidade! \n");
        return NULL;
    }

    for(int i = 0; i < result->Xsize; i++)
    {
        for(int j = 0; j < result->Xsize; j++)
        {
        }
    }

}