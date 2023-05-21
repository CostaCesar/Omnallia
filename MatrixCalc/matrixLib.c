#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "matrixLib.h"

#define MATRIXLIB_INVERSE_ITERATIONS 100
#define MATRIXLIB_COMP_TOLERANCE 0.00001

static int _Matrix_Error_ = 0;

Matrix* free_Matrix(Matrix* freed)
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
        freed = NULL;
    }
    return freed;
}

int isEmpty_Matrix(Matrix* source)
{
    return ((source == NULL) || source->matrix == NULL || source->Xsize * source->Ysize <= 0);
}

int get_MatrixSize(Matrix* source)
{
    if(!isEmpty_Matrix(source))
    {
        return source->Xsize * source->Ysize;
    }
    else return 0;
}

double* get_MatrixElements(Matrix* source)
{
    if(isEmpty_Matrix(source))
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Incapaz de alocar matriz \n");
        return NULL;
    }

    int matrixSize = get_MatrixSize(source);
    double* elements = (double*) malloc(matrixSize * sizeof(double));
    if(elements == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Incapaz de alocar resultado \n");
        return NULL;
    }
    for(int i = 0; i < matrixSize; i++)
    {
        elements[i] = source->matrix[i / source->Xsize][i % source->Xsize];
    }
    return elements;
}

Matrix* alloc_Matrix(int Xsize, int Ysize)
{
    Matrix *output = (Matrix *) malloc(sizeof(Matrix));
    if(output == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Incapaz de alocar matriz \n");
        return NULL;
    }

    output->Xsize = Xsize, output->Ysize = Ysize;
    output->matrix = (double **) malloc(Ysize * sizeof(double*));
    if(output->matrix == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Incapaz de inicializar matriz \n");
        return NULL;
    }
    for(int i = 0; i < Ysize; i++)
    {
        output->matrix[i] = (double *) malloc(Xsize * sizeof(double));
        if(output->matrix[i] == NULL)
        {
            printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
            printf("# Incapaz de inicializar posicao %d\n", i);
            return NULL;
        }
    }
    return output;
}

int comp_Matrix(const Matrix *A, const Matrix *B)
{
    if(A == NULL || B == NULL) return 0;

    if(A->Xsize != B->Xsize || A->Ysize != B->Ysize) return 0;

    for(int i = 0; i < A->Ysize; i++)
    {
        for(int j = 0; j < A->Xsize; j++)
        {
            if( A->matrix[i][j] > B->matrix[i][j] + MATRIXLIB_COMP_TOLERANCE ||
                A->matrix[i][j] < B->matrix[i][j] - MATRIXLIB_COMP_TOLERANCE)
                return 0;

        }
    }
    return 1;
}

void copy_Matrix(Matrix *source, Matrix *destiny)
{
    if(source == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento de entrada invalido \n");
        return;
    }
    if(destiny == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento de saida invalido \n");
        return;
    }
    if(source->Xsize != destiny->Xsize || source->Ysize != destiny->Ysize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
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

Matrix *clone_Matrix(Matrix *source)
{
    if(source == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento de entrada invalido \n");
        return NULL;
    }

    Matrix *res = alloc_Matrix(source->Xsize, source->Ysize);
    if(res == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        return NULL;
    }

    copy_Matrix(source, res);
    return res;
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

    putchar('\n');
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
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Tamanhos de linha invalidos! \n");
        return NULL;
    }
    if(colStart < 0 || colEnd < 0 || colStart >= extractFrom->Xsize || colEnd >= extractFrom->Xsize || colStart > colEnd)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Tamanhos de coluna invalidos! \n");
        return NULL;
    }
    int rowDif = rowEnd - rowStart + 1, colDif = colEnd - colStart + 1;

    Matrix *result = alloc_Matrix(colDif, rowDif);
    if(result == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
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


    Matrix* result = alloc_Matrix(Base->Xsize + Add->Xsize, Base->Ysize);
    if(result == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        return NULL;
    }

    result->Ysize = Base->Ysize, result->Xsize = Base->Xsize + Add->Xsize;
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

int find_MatrixRowNum_AtCol(Matrix *A, int col)
{
    if(A == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Matriz invalido \n");
        return -1;
    }
    if(col < 0 || col >= A->Xsize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Coluna invalida \n");
        return -1;
    }
    double resultN = -DBL_MAX;
    int resultI = 0;

    for(int i = 0; i < A->Ysize; i++)
    {
        if(fabs(A->matrix[i][col]) > resultN)
            resultI = i, resultN = fabs(A->matrix[i][col]);
    }   

    return resultI;
}

void multiply_MatrixRow(Matrix *A, int row, double num)
{
    if(A == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Matriz invalido \n");
        return;
    }
    if(row < 0 || row > A->Ysize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
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
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento base invalido \n");
        return NULL;
    }
    if(add == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento de soma invalido \n");
        return NULL;
    }
    if(atRow < 0 || atRow >= main->Ysize || atCol < 0 || atCol >= main->Xsize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Posicao inicial invalida \n");
        return NULL;
    }
    if( untilRow < 0 || untilRow >= add->Ysize || untilRow > main->Ysize ||
        untilCol < 0 || untilCol >= add->Xsize || untilCol > main->Xsize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Posicao final invalida \n");
        return NULL;
    }

    Matrix * res = alloc_Matrix(main->Xsize, main->Ysize);
    if(res == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        return NULL;
    }

    for(int i = 0; i < res->Ysize; i++)
    {
        for(int j = 0; j < res->Xsize; j++)
        {
            res->matrix[i][j] = main->matrix[i][j];
            if((i >= atRow && j >= atCol) && (i-atRow <= untilRow && j-atCol <= untilCol))
                res->matrix[i][j] += add->matrix[i-atRow][j-atCol];
        }
    }
    
    return res;
}

Matrix* multiply_MatrixByN(Matrix* A, double N)
{
    if(A == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumentos invalidos! \n");
        return NULL;
    }
    if(A->Xsize < 1)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Tamanhos fora de escala! \n");
        return NULL;
    }
    
    Matrix * res = alloc_Matrix(A->Xsize, A->Ysize);
    if(res == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        return NULL;
    }

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

    Matrix *res = alloc_Matrix(B->Xsize, A->Ysize);
    if(res == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
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

void multiplyRow_AddMatrix(Matrix *A, int multipRow, double num, int addRow)
{
    if(A == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento matriz invalido \n");
        return;
    }
    if(multipRow < 0 || multipRow >= A->Ysize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento linha multiplicada invalido \n");
        return;
    }
    if(addRow < 0 || addRow >= A->Ysize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumento linha adicionada invalido \n");
        return;
    }

    Matrix *intermed = extract_Row(A, multipRow);
    if(intermed == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        return;
    }
    multiply_MatrixRow(intermed, 0, num);

    Matrix *res = add_Matrix_Part(A, intermed, addRow, 0, 0, intermed->Xsize-1);
    if(intermed == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        free_Matrix(intermed);
        return;
    }
    
    copy_Matrix(res, A);
    free_Matrix(intermed);
    free(res);
    return;
}

Matrix* create_IdentityMatrix(Matrix *A)
{
    if((A == NULL) || A->Xsize < 1 || A->Ysize < 1)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Argumerto matriz invalido! ");
        return NULL;
    }

    
    Matrix *out = alloc_Matrix(A->Xsize, A->Ysize);
    if(out == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        return NULL;
    }

    for(int i = 0; i < out->Ysize; i++)
    {
        for(int j = 0; j < out->Xsize; j++)
        {
            if(i == j) out->matrix[i][j] = 1;
            else out->matrix[i][j] = 0;
        }
    }

    return out;
}

Matrix* get_Inverse_Matrix(Matrix *A)
{
    if(A->Xsize != A->Ysize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# A matriz nao possui um tamanho compativel! \n");
        return NULL;
    }
    
    Matrix* identity = create_IdentityMatrix(A);
    if(identity == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Incapaz de incializar matriz identidade! \n");
        return NULL;
    }

    Matrix *mid = join_Matrix_Left(A, identity);
    if(mid == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Incapaz de incializar matriz identidade! \n");
        free(identity);
        return NULL;
    }

    int cRow, cCol, completed = 0;
    for(int count = 0; completed == 0 && count < MATRIXLIB_INVERSE_ITERATIONS; count++)
    {
        for(int diagI = 0; diagI < mid->Ysize; diagI++)
        {
            cRow = diagI, cCol = diagI;
            int maxI = find_MatrixRowNum_AtCol(mid, cCol);
            if(maxI != cRow)
                swap_MatrixRows(mid, cRow, maxI);
            
            if(mid->matrix[cRow][cCol] != 1)
            {
                double factor = 1.0/mid->matrix[cRow][cCol];
                multiply_MatrixRow(mid, cRow, factor);
            }

            for(int rowI = cRow+1; rowI < mid->Ysize; rowI++)
            {
                if(mid->matrix[rowI][cCol] != 0)
                {
                    int baseRow = cCol;
                    double cValue = mid->matrix[rowI][cCol], baseValue = mid->matrix[baseRow][cCol];
                    if(baseValue != 0)
                    {
                        double factor = -(cValue/baseValue);
                        multiplyRow_AddMatrix(mid, baseRow, factor, rowI);
                    }
                }
            }

            for(int colI = cRow+1; colI < A->Xsize; colI++)
            {
                if(mid->matrix[cRow][colI] != 0)
                {
                    int baseRow = colI;
                    double cValue = mid->matrix[cRow][colI], baseValue = mid->matrix[baseRow][colI];
                    if(baseValue != 0)
                    {
                        double factor = -(cValue/baseValue);
                        multiplyRow_AddMatrix(mid, baseRow, factor, cRow);
                    }
                } 
            }
        }

        Matrix *left = extract_Matrix(mid, 0, 0, A->Ysize-1, A->Xsize-1);
        if(left == NULL)
        {
            printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
            return NULL;
        }
        
        if(comp_Matrix(left, identity) == 1)
        {
            completed = 1;
            Matrix *result = extract_Matrix(mid, 0, A->Xsize, mid->Ysize-1, mid->Xsize-1);
            free(identity);
            free(mid);
            return result;
        }
        free(left);
    }
    
    printf("WARNING: %s, %d \n", __FUNCTION__, __LINE__);
    printf("# Incapaz de encontrar inversa! \n");
    free(identity);
    free(mid);
    return NULL;
}

Matrix* extract_Matrix_Except(Matrix* A, int rowExcep, int colExcep)
{
    if(A == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Matriz invalida! \n");
        return NULL;
    }

    if(rowExcep < 0 || rowExcep >= A->Ysize || colExcep < 0 || colExcep >= A->Xsize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Tamanho da matriz invalido \n");
        return NULL;
    }

    Matrix *res = alloc_Matrix(A->Xsize - 1, A->Ysize - 1);
    if(res == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        return NULL;
    }

    int rowOffset = 0, colOffset = 0;
    for(int i = 0; i < A->Ysize -1; i++)
    {
        colOffset = 0;
        if(i >= rowExcep)
            rowOffset = 1;
          
        for(int j = 0; j < A->Xsize -1; j++)
        {
            if(j >= colExcep)
                colOffset = 1;

            res->matrix[i][j] = A->matrix[i + rowOffset][j + colOffset];
        }
    }
    return res;
}

double get_Determinant(Matrix *A)
{
    int res = 0, buffer = 0;
    _Matrix_Error_ = 0;
    
    if(A == NULL)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Matriz invalida! \n");
        return 0;
    }
    if(A->Xsize < 1 || A->Ysize < 1 || A->Xsize != A->Ysize)
    {
        printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
        printf("# Tamanho da matriz nao e valido ou quadrado! \n");
        return 0; 
    }

    if(A->Xsize == 1)
        return A->matrix[0][0];
    if(A->Xsize == 2)
        return (A->matrix[0][0] * A->matrix[1][1]) - (A->matrix[1][0] * A->matrix[0][1]);
    
    for(int i = 0; i < A->Xsize; i++)
    {
        Matrix *subMatrix = extract_Matrix_Except(A, 0, i);
        if(subMatrix == NULL)
        {
            _Matrix_Error_ = 1;
            printf("ERROR: %s, %d \n", __FUNCTION__, __LINE__);
            return 0;
        }
        
        buffer = get_Determinant(subMatrix);
        if(_Matrix_Error_ == 1)
        {
            free_Matrix(subMatrix);
            return 0;
        }

        // A[0][n] (nada otimizado) X Cofator (determinante * -pra impar & +pra par)
        res += A->matrix[0][i] * (buffer * (i % 2 == 0 ? 1 : -1));
        free_Matrix(subMatrix);
    }

    return res;
}