#include <stdio.h>
#include <stdlib.h>
#include "matrixLib.h"

#define NUM_MATRICES 10

void display_MatrixStates(Matrix** list)
{
    for(int i = 0; i < NUM_MATRICES; i++)
    {
        printf("[%s] Matriz %d \n", list[i] != NULL ? "USADA" : "LIVRE", i);
    }
    return;
}

int confirm_Matrix(char* question)
{
    int matrixId = -1;
    printf("> %s: ", question);
    scanf("%d", &matrixId);

    if(matrixId < 0 || matrixId >= NUM_MATRICES)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Escolha invalida! \n");
        return -1;
    }
    return matrixId;
}

Matrix *obtain_Matrix(Matrix **list)
{
    int matrixId = -1;
    
    display_MatrixStates(list);
    matrixId = confirm_Matrix("Escolha a matriz a ser usada: ");
    if(matrixId < 0) return NULL;

    return list[matrixId];
}

Matrix* userFill_Matrix()
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

void print_Matrix(Matrix *matrix)
{
    // Load Matrix
    if(isEmpty_Matrix(matrix)) return;
    double *elements = get_MatrixElements(matrix);
    
    // Separate print
    putchar('\n');

    // Print cols number
    printf("             |");
    for(int i = 0; i < matrix->Xsize; i++)
    {
        printf(" %10d |", i+1);
    }
    putchar('\n');
    putchar('\n');

    // Print rows number & values
    // Number
    for(int i = 0; i < matrix->Ysize; i++)
    {
        printf("| %10d |", i+1);

        // Values
        for(int j = 0; j < matrix->Xsize; j++)
        {
            printf("  %10.4lf ", elements[i*matrix->Xsize+j]);
        }
        putchar('\n');
        putchar('\n');
    }
    
    putchar('\n');
    return;
}

int promt_Matrix(Matrix* source, char* message)
{
    char choice = ' ';
    if(source != NULL)
    {
        print_Matrix(source);
        printf("> %s [S/N] ", message);
        
        // Loop to ignore '\n' left in stdin, breaks when user inputs a valid char 
        do choice = getchar();
        while(choice == '\n');

        if(choice == 's' || choice == 'S') return 1;
    }
    return 0;
}
void get_Matrix(Matrix ***list)
{
    int matrixId = -1;
    
    display_MatrixStates(*list);
    matrixId = confirm_Matrix("Escolha qual das matrizes ocupar: ");
    if(matrixId < 0) return;

    if((*list)[matrixId] != NULL)
    {
        if(promt_Matrix((*list)[matrixId], "Deseja realmente substituir a matriz?") == 0) return;
    }
    (*list)[matrixId] = free_Matrix((*list)[matrixId]);
    (*list)[matrixId] = userFill_Matrix();
    return;
}

void view_Matrix(Matrix **list)
{
    int matrixId = -1;
    
    display_MatrixStates(list);
    matrixId = confirm_Matrix("Escolha a matriz a ser visualizada: ");
    if(matrixId < 0) return;

    if(list[matrixId] != NULL)
    {
        print_Matrix(list[matrixId]);
    }
    else
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        printf("# Matriz vazia! \n");
    }
    return;
}

void insert_Matrix(Matrix ***list, Matrix *insert)
{
    if(insert == NULL) return;

    int matrixId = -1;
    
    display_MatrixStates(*list);
    matrixId = confirm_Matrix("Escolha a matriz a ser ocupada: ");
    if(matrixId < 0) return;

    if((*list)[matrixId] != NULL)
    {
        if(promt_Matrix((*list)[matrixId], "Deseja realmente substituir a matriz?") == 0) return;
    }
    (*list)[matrixId] = free_Matrix((*list)[matrixId]);
    (*list)[matrixId] = insert;
    printf("$ Matriz inserida com sucesso! \n");
    return;
}

Matrix *oprt_MultiplyMatrices(Matrix **list)
{
    int confirm = 0;
    Matrix *A, *B; 
    do
    {
        A = obtain_Matrix(list);
        if(A == NULL)
        {
            printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
            printf("# Matriz invalida! \n");
            return NULL;
        }
        confirm = promt_Matrix(A, "Tem certeza dessa escolha?");
    } while (confirm != 1);
    do
    {
        B = obtain_Matrix(list);
        if(B == NULL)
        {
            printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
            printf("# Matriz invalida! \n");
            return NULL;
        }
        confirm = promt_Matrix(B, "Tem certeza dessa escolha?");
    } while (confirm != 1);
    
    Matrix *res = multiply_Matrixes(A, B);
    if(res == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        return NULL;
    }
    print_Matrix(res);
    return res;
}

Matrix *oprt_MultiplyMatrixByN(Matrix **list)
{
    int confirm = 0;
    Matrix *A;
    do
    {
        A = obtain_Matrix(list);
        if(A == NULL)
        {
            printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
            printf("# Matriz invalida! \n");
            return NULL;
        }
        confirm = promt_Matrix(A, "Tem certeza dessa escolha?");
    } while (confirm != 1);

    double n = 0;
    printf("> Digite o numero para multiplicar a matriz por: ");
    scanf("%lf", &n);

    Matrix *res = multiply_MatrixByN(A, n);
    if(res == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        return NULL;
    }
    print_Matrix(res);
    return res;
}

Matrix *oprt_Inverse_Matrix(Matrix **list)
{
    int confirm = 0;
    Matrix *A;
    do
    {
        A = obtain_Matrix(list);
        if(A == NULL)
        {
            printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
            printf("# Matriz invalida! \n");
            return NULL;
        }
        confirm = promt_Matrix(A, "Tem certeza dessa escolha?");
    } while (confirm != 1);

    Matrix *res = get_Inverse_Matrix(A);
    if(res == NULL)
    {
        printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
        return NULL;
    }
    print_Matrix(res);
    return res;
}

void oprt_Determinant(Matrix **list)
{
    int confirm = 0;
    Matrix *A;
    do
    {
        A = obtain_Matrix(list);
        if(A == NULL)
        {
            printf("ERROR: %s, %d", __FUNCTION__, __LINE__);
            printf("# Matriz invalida! \n");
            return;
        }
        confirm = promt_Matrix(A, "Tem certeza dessa escolha?");
    } while (confirm != 1);

    double res = get_Determinant(A);
    printf("$ Determinante da matriz: %lf", res);

    return;
}

int main(int argc, char **argv)
{
    char action, ignore;
    Matrix *RES_Matrix = NULL, *LastRes_Matrix = NULL;
    Matrix **Matrices = (Matrix**) calloc(NUM_MATRICES, sizeof(Matrix*));

    do
    {
        putchar('\n');
        printf("<[Q] Inserir Matriz \n");
        printf("<[W] Ver Matriz \n");
        printf("<[A] Inserir Ultimo Resultado> \n");
        printf("<[S] Ver Ultimo Resultado> \n");
        printf("<[C] Multiplicar Matriz x Numero> \n");
        printf("<[E] Multiplicar Matriz x Matriz> \n");
        printf("<[R] Obter Determinante> \n");
        printf("<[T] Obter Matriz Inversa> \n");
        printf("<[P] Sair> \n");
        fflush(stdin);
        
        // Loop to ignore '\n' left in stdin, breaks when user inputs a valid char 
        do action = getchar();
        while(action == '\n');

        switch (action)
        {
            case 'q': case 'Q':
                get_Matrix(&Matrices);
                break;
            case 'w': case 'W':
                view_Matrix(Matrices);
                break;
            case 'a': case 'A':
                insert_Matrix(&Matrices, LastRes_Matrix);
                break;
            case 's': case 'S':
                print_Matrix(LastRes_Matrix);
                break;
            case 'c': case 'C':
                RES_Matrix = oprt_MultiplyMatrixByN(Matrices);
                break;
            case 'e': case 'E':
                RES_Matrix = oprt_MultiplyMatrices(Matrices);
                break;
            case 't': case 'T':
                RES_Matrix = oprt_Inverse_Matrix(Matrices);
                break;
            case 'r':
            case 'R':
                oprt_Determinant(Matrices);
                break;
            case 'p':
            case 'P':
                for(int i = 0; i < NUM_MATRICES; i++)
                    (void) free_Matrix(Matrices[i]);
                (void) free_Matrix(RES_Matrix);
                (void) free_Matrix(LastRes_Matrix);
                printf("$ Saindo... \n");
                return 0;
            default:
                printf("$ Comando Invalido! \n");
                break;
        }
        if((!isEmpty_Matrix(RES_Matrix)) && !comp_Matrix(RES_Matrix, LastRes_Matrix))
        {
            LastRes_Matrix = free_Matrix(LastRes_Matrix);
            LastRes_Matrix = clone_Matrix(RES_Matrix);
        }
        RES_Matrix = free_Matrix(RES_Matrix);
    } while (1);

    return 1;
}