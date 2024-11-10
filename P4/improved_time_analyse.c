#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h> 

#define max_size 501



int main() {
    int N; // number of the internal nodes;
    printf("please input the number of internal nodes:\n");
    scanf("%d", &N);

    //output the answer for the small size input
    if (N == 1 || N == 2) {
        printf("The number of different RBT with %d internal nodes is %d", N, N);
        return 0;
    }

    unsigned  long  int black_root_dp[max_size][max_size]; // dp[i][j] record RBT numbers when its internal nodes = i and black height = j
    unsigned  long  int red_root_dp[max_size][max_size]; //  same as the former, prepared for the former's calculation



    unsigned long int  RB_black_root, BR_black_root, BB_black_root, RR_black_root, count;
    int test_times ;
    int time_range[5] = {200,400,600,800,1000};

    clock_t start_time = clock();
    for (int i = 0; i < 5; i++) {
        test_times = time_range[i];
        for(int i = 0 ; i < test_times ;i ++){
            //initialize dp
            for (int i = 0; i <= N; i++) {
            for (int j = 0; j <= N; j++) {
                black_root_dp[i][j] = 0;
                red_root_dp[i][j] = 0;
                }
            }
            black_root_dp[0][1] = 1; // Null leaf's BH = 1
            black_root_dp[1][1] = 1; // black root's BH = 1
            black_root_dp[2][1] = 2; //when it has two internal nodes and  black root 's BH = 1, there are two cases 
            black_root_dp[2][2] = 0; // no situation for this case

            red_root_dp[0][1] = 1; // NULL leaf's BH = 1
            red_root_dp[1][1] = 1; // red root has its null leaf, so its BH = 1 and number of situaion = 1
            red_root_dp[2][1] = 0; // no consisent red nodes for RBT, so when BH = 1, number = 0
            red_root_dp[2][2] = 0; // same as the former

            //dynamical programming
            //BH : black height
            for (int internal_nodes = 3; internal_nodes <= N; internal_nodes++) {
                // BH's range is calculated by RBT's property(please check out the document for more information)
                for (int BH = (int)ceil(log(internal_nodes + 1) / log(4)); 1 << BH - 1 <= internal_nodes; BH++)  {
                    // compute the subproblems: add all situaition 's left_number * right_number 
                    // Left_size's range is calculated by RBT's property(please check document for more information)
                    for (int left_size = pow(2, BH - 1) - 1; left_size < 1 << 2*BH - 1 && left_size < internal_nodes - 1; left_size++) {
                        int right_size = internal_nodes - 1 - left_size;

                        // for red root, sub_tree must be black_root, add them up
                        red_root_dp[internal_nodes][BH] += black_root_dp[left_size][BH - 1] * black_root_dp[right_size][BH - 1];

                        //when the subtree's root is black, its BH is actually its black_height +1 ,so we use BH-1 for calculating BH
                        RB_black_root = red_root_dp[left_size][BH] * black_root_dp[right_size][BH - 1]; // RB situation for black root case
                        BR_black_root = black_root_dp[left_size][BH - 1] * red_root_dp[right_size][BH]; // BR situtation for black root case
                        BB_black_root = black_root_dp[left_size][BH - 1] * black_root_dp[right_size][BH - 1]; // BB situation for black root case
                        RR_black_root = red_root_dp[left_size][BH] * red_root_dp[right_size][BH]; // RR situation for black root case

                        //add up all situation numbers for black_root 
                        black_root_dp[internal_nodes][BH] += RB_black_root + BR_black_root + BB_black_root + RR_black_root;

                    }
                }
            }

            //sum up all the RBT with N internal nodes
            count = 0;
            // i's range is calculated by RBT's property(please check out the document for more information)
            for (int i = (int)ceil(log(N + 1) / log(4)); 1 << i - 1 <= N; i++) {
                count += black_root_dp[N][i];

                //printf("black_root_dp of BH:%d is %lu\n\n", i, black_root_dp[N][i]);
                //printf("red_root_dp of BH:%d is %lu\n\n", i, red_root_dp[N][i]);
            }

        count = count % 1000000007;
    }

    clock_t end_time = clock(); // stop time record

    // calculate taken time
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Time taken for the computation of %d times: %f seconds\n", test_times,time_taken);
    }

    printf("\n--Result-- \nThe number of different RBT with %d internal nodes mod by 1000000007 is %lu", N, count);

}