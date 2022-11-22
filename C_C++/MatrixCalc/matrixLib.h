typedef struct P_Matrix
{
    double **matrix;
    int Xsize;
    int Ysize;
} Matrix;

void free_Matrix(Matrix* freed);
double** alloc_Matrix(int Xsize, int Ysize);
void show_Matrix(Matrix* show);
void swap_MatrixRows(Matrix *A, int rowA, int rowB);
Matrix* extract_Matrix(Matrix *extractFrom, int rowStart, int colStart, int rowEnd, int colEnd);
Matrix *extract_Row(Matrix* A, int row);
Matrix *join_Matrix_Left(Matrix *Base, Matrix *Add);
void split_Matrix_AtCol(Matrix *origin, Matrix **left, Matrix **right, int column);
Matrix* multiply_MatrixByN(Matrix* A);
Matrix* multiply_Matrixes(Matrix *A, Matrix *B);