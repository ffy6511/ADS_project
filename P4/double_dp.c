#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define max_size 100

int main() {
    int N; // number of the internal nodes;
    printf("please input the number of internal nodes:\n");
    scanf("%d", &N);

    if (N == 1 || N == 2) {
        printf("The number of different RBT with %d internal nodes is %d", N, N);
        return 0;
    }

    int black_root_dp[max_size][max_size]; // dp[i][j] record RBT numbers when its internal nodes = i and black height = j
    int red_root_dp[max_size][max_size]; //  same as the former, prepared for the former's calculation

    //initialize dp
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            black_root_dp[i][j] = 0;
            red_root_dp[i][j] = 0;
        }
    }
    black_root_dp[0][1] = 1;
    black_root_dp[1][1] = 1;
    black_root_dp[2][1] = 2;
    black_root_dp[2][2] = 0;

    red_root_dp[0][1] = 1;
    red_root_dp[1][1] = 1;
    red_root_dp[2][1] = 0;
    red_root_dp[2][2] = 0;

    int RB_black_root, BR_black_root, BB_black_root, RR_black_root;

    //dynamical programming
    //BH : black height
    for (int internal_nodes = 3; internal_nodes <= N; internal_nodes++) {
        for (int BH = 1; pow(2, BH) - 1 <= internal_nodes; BH++) {
            //compute the subproblems
            for (int left_size = 1; left_size < internal_nodes - 1; left_size++) {
                int right_size = internal_nodes - 1 - left_size;

                red_root_dp[internal_nodes][BH] += black_root_dp[left_size][BH - 1] * black_root_dp[right_size][BH - 1];

                //when the subtree's root is black, its BH is actually its black_height +1 ,so we use BH-2 for calculating BH
                RB_black_root = red_root_dp[left_size][BH] * black_root_dp[right_size][BH - 1];
                BR_black_root = black_root_dp[left_size][BH - 1] * red_root_dp[right_size][BH];
                BB_black_root = black_root_dp[left_size][BH - 1] * black_root_dp[right_size][BH - 1];
                RR_black_root = red_root_dp[left_size][BH] * red_root_dp[right_size][BH];

                black_root_dp[internal_nodes][BH] += RB_black_root + BR_black_root + BB_black_root + RR_black_root;


            }
        }
    }
    
    //sum up all the RBT with N internal nodes
    long int  count = 0;
    for (int i = 1; pow(i,2)-1 <= N; i++) {
        count += black_root_dp[N][i];
        printf("black_root_dp of BH:%d is %d\n", i, black_root_dp[N][i]);
        printf("red_root_dp of BH:%d is %d\n", i, red_root_dp[N][i]);
    }

    count = count % 1000000007;

    printf("\nThe number of different RBT with %d internal nodes is %ld", N, count);

}