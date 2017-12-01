/*************************************************************************
	> File Name: lufenjiefa.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Tue 28 Nov 2017 03:04:43 AM CST
 ************************************************************************/

#include<stdio.h>
#include<math.h>
#define JIESHUMAX 1000

void display2(int jieshu, void * pA){
    double (*A)[jieshu];
    A = (double (*)[])pA;
    for(int i = 1; i <= jieshu ; i++){
        for(int j = 1; j <= jieshu; j++){
            printf("%lf\t", A[i - 1][j - 1]);
        }
        printf("\n");
    }
    printf("----------------\n");
}

/*
 * 对于方程 Ax=b
 *
 * A = 
 *
 *  a1,1 a1,2 ... a1,j ... a1,n
 *
 *  a2,1 a2,2 ... a2,j ... a2,n
 *  
 *   :    :        :        :
 *
 *  ai,1 ai,2 ... ai,j ... ai,n
 *
 *   :    :        :        :
 *
 *  an,1 an,2 ... an,j ... an,n
 *
 * x = 
 *
 *  x1
 *
 *  x2
 *
 *   :
 *
 *  xn
 *
 * b =
 *
 *  b1
 *
 *  b2
 *
 *   :
 *
 *  bn
* */

int lufenjiefa(const int jieshu, void * pA, double * x, double * b){
    double (*A)[jieshu];
    A = (double (*)[])pA;
    double L[jieshu][jieshu];
    double U[jieshu][jieshu];
    // U第一行 1st row of U
    for(int j = 1; j <= jieshu; j++){
        U[1 - 1][j - 1] = A[1 - 1][j - 1];
        if(U[1 - 1][1 - 1] == 0) return 1;
    }
    //display2(jieshu, (void *)L);
    //display2(jieshu, (void *)U);
    // L第一列 1st column of L
    for(int i = 1; i <= jieshu; i++){
        L[i - 1][1 - 1] = A[i - 1][1 - 1] / U[1 - 1][1 - 1];
    }
    puts("+++++++++U++++++++");
    display2(jieshu, (void *)U);
    puts("+++++++++L++++++++");
    display2(jieshu, (void *)L);
    // U 第r行 rth row of U 和 L 第r列 rth column of L
    for(int r = 2; r <= jieshu; r++){
        // U 第r行第j列 rth row, jth column of U
        for(int j = r; j <= jieshu; j++){
            double sum = 0;
            for(int k = 1; k <= r - 1; k++){
                sum += L[r - 1][k - 1] * U[k - 1][j - 1];
            }
            U[r - 1][j - 1] = A[r - 1][j - 1] - sum;
            if(U[r - 1][r - 1] == 0) return 1;
        }
        // L 第r列第i行 rth column, ith row of L
        for(int i = r + 1; i <= jieshu; i++){
            double sum = 0;
            for(int k = 1; k <= r - 1; k++){
                sum += L[i - 1][k - 1] * U[k - 1][r - 1];
            }
            L[i - 1][r - 1] = (A[i - 1][r - 1] - sum) / U[r - 1][r - 1];
        }
    }
    for(int r = 1; r <= jieshu - 1; r++){
    }
    // 解y
    double y[jieshu];
    y[1 - 1] = b[1 - 1];
    // y 第r行
    for(int r = 2; r <= jieshu; r++){
        double sum = 0;
        for(int j = 1; j <= r - 1; j++){
            sum += L[r - 1][j - 1] * y[j - 1];
        }
        y[r - 1] = b[r - 1] - sum;
    }
    // 解x
    x[jieshu - 1] = y[jieshu - 1] / U[jieshu - 1][jieshu - 1];
    // x 第r行
    for(int r = jieshu - 1; r >= 1; r--){
        double sum = 0;
        for(int j = r + 1; j <= jieshu; j++){
            sum += U[r - 1][j - 1] * x[j - 1];
        }
        x[r - 1] = (y[r - 1] - sum) / U[r - 1][r - 1];
    }
    return 0;
}

int main(void){
    /*double A[4][4] = {1, 2, 3, 4, 
                      -1, -2, 3, 4, 
                      0, 2, 4, 6, 
                      1, 3, 5, 7};
                      */
    /*double A[5][5] = {2, 4, 3, 9, -2, 
                      3, 4, 15, -9, 0, 
                      4, 5, 11, 12, 16, 
                      -3, -2, -1, 10, 7,
                      0, 2, 2, 5, 9};
                      */
    /*double A[3][3] = {2, 5, -6,
                      4, 13, -19,
                      -6, -3, 4};
                      */
    /*double A[3][3] = {1 , 2, 3,
                      -1, -2, 3,
                      0, 2, 4};
                      */
    double A[3][3] = {4, 4, -2,
                      3, 4, -3,
                      -2, -3, 1};
    double x[3];
    double b[3] = { -1, 
                    3, 
                    1};
    if(lufenjiefa(3, (void *)A, x, b)){
        puts("Error");
    }else{
        for(int i = 1; i <= 3; i++){
            printf("%lf\t", x[i - 1]);
        }
    }
    puts("");

    return 0;
}

/*
int ttt(double (*A)[3]){
    printf("%lf\n", A[2][2] );
    return 0;
}

int main(void){
    double A[3][3] = {1, 2, 3, 
                   4, 5, 6, 
                   7, 8, 9};
    ttt(A);

    return 0;
}
*/
