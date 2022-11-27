typedef struct P_Matrix
{
    double **matrix;
    int Xsize;
    int Ysize;
} Matrix;

void free_Matrix(Matrix* freed);
void show_Matrix(Matrix* show);
void copy_Matrix(Matrix *source, Matrix *destiny);
void swap_MatrixRows(Matrix *A, int rowA, int rowB);
void multiply_MatrixRow(Matrix *A, int row, double num);
void split_Matrix_AtCol(Matrix *origin, Matrix **left, Matrix **right, int column);
int find_MatrixRow_Element_AtCol(Matrix *A, int col, int biggest);
Matrix* clone_Matrix(Matrix *source);
Matrix* getInverse_Matrix(Matrix *A);
Matrix* multiply_MatrixByN(Matrix* A);
Matrix* extract_Row(Matrix* A, int row);
Matrix* alloc_Matrix(int Xsize, int Ysize);
Matrix* multiply_Matrixes(Matrix *A, Matrix *B);
Matrix* join_Matrix_Left(Matrix *Base, Matrix *Add);
Matrix* multiplyRow_AddMatrix(Matrix *A, int multipRow, double num, int addRow);
Matrix* extract_Matrix(Matrix *extractFrom, int rowStart, int colStart, int rowEnd, int colEnd);
Matrix* add_Matrix_Part(Matrix *main, Matrix *add, int atRow, int atCol, int untilRow, int untilCol);