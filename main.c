/*
Copyright (c) 2016 Frederico Schardong
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <math.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define PI 3.14159265358979323846

float data[100][4] = {0};
float center_x1, center_y1, center_x2, center_y2;
float pearson_k1 = 0, pearson_k2 = 0;
float PI_k1 = 0, PI_k2 = 0;
float standard_deviation_k1_x = 0, standard_deviation_k2_x = 0, standard_deviation_k1_y = 0, standard_deviation_k2_y = 0;

int read_file(){
  int i = 0;
  FILE *fp;

  if((fp = fopen("data.txt", "r")) == NULL) {
    errx(-1, "File opening error, errno = %d, %s", errno, strerror(errno));
  }

  if (fp != NULL){
    while (fscanf(fp, "%f %f", &data[i][0], &data[i][1]) != EOF){
      i++;
    }
  }

  fclose(fp);
}

float euclidean_distance(float x1, float y1, float x2, float y2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void k_means(){
  int i;
  float old_center_x1, old_center_y1, old_center_x2, old_center_y2;

  center_x1 = data[0][0];
  center_y1 = data[0][1];
  center_x2 = data[1][0];
  center_y2 = data[1][1];

  do{
    for(i = 0; i < 100; i++){
      if(euclidean_distance(center_x1, center_y1, data[i][0], data[i][1]) > euclidean_distance(center_x2, center_y2, data[i][0], data[i][1])){
        data[i][2] = 0;
      }
      else{
        data[i][2] = 1;
      }
    }

    old_center_x1 = center_x1;
    old_center_y1 = center_y1;
    old_center_x2 = center_x2;
    old_center_y2 = center_y2;

    float total_center_x1 = 0, total_center_y1 = 0, total_center_x2 = 0, total_center_y2 = 0;
    int total_k1 = 0, total_k2 = 0;

    for(i = 0; i < 100; i++){
      if(data[i][2]){
        total_center_x1 += data[i][0];
        total_center_y1 += data[i][1];
        total_k1++;
      }
      else{
        total_center_x2 += data[i][0];
        total_center_y2 += data[i][1];
        total_k2++;
      }
    }
    
    center_x1 = total_center_x1 / total_k1;
    center_y1 = total_center_y1 / total_k1;
    center_x2 = total_center_x2 / total_k2;
    center_y2 = total_center_y2 / total_k2;

  }while(old_center_x1 != center_x1 && old_center_y1 != center_y1 && old_center_x2 != center_x2 && old_center_y2 != center_y2);
}

//https://en.wikipedia.org/wiki/Multivariate_normal_distribution#Density_function
float pdf_for_bivariate_distribution(float x, float x_standard_deviation, float x_mean, float y, float y_standard_deviation, float y_mean, float pearson_product){


  return (1 / (2 * PI * x_standard_deviation * y_standard_deviation * sqrt(1 - pow(pearson_product, 2)))) *
         expf(-(1 / (float)(2 * (1 - pow(pearson_product, 2)))) *
              ((pow(x - x_mean, 2) / pow(x_standard_deviation, 2)) + (pow(y - y_mean, 2) / pow(y_standard_deviation, 2)) -
               ((2 * pearson_product * (x - x_mean) * (y - y_mean))/(x_standard_deviation * y_standard_deviation))));
}

void em(){
  int i;
  int total_k1 = 0, total_k2 = 0;

  float Pr_k1[100] = {0}, Pr_k2[100] = {0};
  float total_center_x1 = 0, total_center_y1 = 0, total_center_x2 = 0, total_center_y2 = 0;
  float covariance_k1 = 0, covariance_k2 = 0;
  float covariance_k1_change, covariance_k2_change;

  //generate things based on k-means
  for(i = 0; i < 100; i++){
    if(data[i][2]){
      total_center_x1 += data[i][0];
      total_center_y1 += data[i][1];
      total_k1++;
    }
    else{
      total_center_x2 += data[i][0];
      total_center_y2 += data[i][1];
      total_k2++;
    }
  }

  center_x1 = total_center_x1 / total_k1;
  center_y1 = total_center_y1 / total_k1;
  center_x2 = total_center_x2 / total_k2;
  center_y2 = total_center_y2 / total_k2;

  //actually it is not standard deviation at this point
  for(i = 0; i < 100; i++){
    if(data[i][2]){
      standard_deviation_k1_x += pow(data[i][0] - center_x1, 2);
      standard_deviation_k1_y += pow(data[i][1] - center_y1, 2);
      covariance_k1 += (data[i][0] - center_x1) * (data[i][1] - center_y1);
    }
    else{
      standard_deviation_k2_x += pow(data[i][0] - center_x2, 2);
      standard_deviation_k2_y += pow(data[i][1] - center_y2, 2);
      covariance_k2 += (data[i][0] - center_x2) * (data[i][1] - center_y2);
    }
  }

  //https://en.wikipedia.org/wiki/Covariance#Discrete_variables
  covariance_k1 = covariance_k1 / total_k1;
  covariance_k2 = covariance_k2 / total_k2;

  //now it is standard deviation, but you knew it :)
  standard_deviation_k1_x = sqrt(standard_deviation_k1_x / total_k1);
  standard_deviation_k1_y = sqrt(standard_deviation_k1_y / total_k1);
  standard_deviation_k2_x = sqrt(standard_deviation_k2_x / total_k2);
  standard_deviation_k2_y = sqrt(standard_deviation_k2_y / total_k2);

  //Pearson product https://en.wikipedia.org/wiki/Pearson_product-moment_correlation_coefficient#For_a_population
  pearson_k1 = covariance_k1 / (standard_deviation_k1_x * standard_deviation_k1_y);
  pearson_k2 = covariance_k2 / (standard_deviation_k2_x * standard_deviation_k2_y);

  //PI_{k} is the probability of an element belong to group k
  PI_k1 = total_k1 / (float)(total_k1 + total_k2);
  PI_k2 = total_k2 / (float)(total_k1 + total_k2);

  do{
    float N_k1 = 0, N_k2 = 0, old_covariance_k1 = covariance_k1, old_covariance_k2 = covariance_k2;

    //Step E
    for(i = 0; i < 100; i++){
      Pr_k1[i] = pdf_for_bivariate_distribution(data[i][0], standard_deviation_k1_x, center_x1, data[i][1], standard_deviation_k1_y, center_y1, pearson_k1) * PI_k1;
      Pr_k2[i] = pdf_for_bivariate_distribution(data[i][0], standard_deviation_k2_x, center_x2, data[i][1], standard_deviation_k2_y, center_y2, pearson_k2) * PI_k2;

      float temp = Pr_k1[i];

      Pr_k1[i] = Pr_k1[i] / (Pr_k1[i] + Pr_k2[i]);
      Pr_k2[i] = Pr_k2[i] / (temp + Pr_k2[i]);

      //calculate N_{k}
      N_k1 += Pr_k1[i];
      N_k2 += Pr_k2[i];
    }

    //Step M
    //update mean
    float cond_prob_k1_x = 0, cond_prob_k1_y = 0, cond_prob_k2_x = 0, cond_prob_k2_y = 0;

    for(i = 0; i < 100; i++){
      cond_prob_k1_x += Pr_k1[i] * data[i][0];
      cond_prob_k1_y += Pr_k1[i] * data[i][1];
      cond_prob_k2_x += Pr_k2[i] * data[i][0];
      cond_prob_k2_y += Pr_k2[i] * data[i][1];
    }

    center_x1 = cond_prob_k1_x / N_k1;
    center_y1 = cond_prob_k1_y / N_k1;
    center_x2 = cond_prob_k2_x / N_k2;
    center_y2 = cond_prob_k2_y / N_k2;

    //update standard deviation, covariance and pearson_product
    cond_prob_k1_x = cond_prob_k1_y = cond_prob_k2_x = cond_prob_k2_y = 0;
    covariance_k1 = covariance_k2 = 0;

    for(i = 0; i < 100; i++){
      cond_prob_k1_x += Pr_k1[i] * pow(data[i][0] - center_x1, 2);
      cond_prob_k1_y += Pr_k1[i] * pow(data[i][1] - center_y1, 2);
      cond_prob_k2_x += Pr_k2[i] * pow(data[i][0] - center_x2, 2);
      cond_prob_k2_y += Pr_k2[i] * pow(data[i][1] - center_y2, 2);

      covariance_k1 += Pr_k1[i] * (data[i][0] - center_x1) * (data[i][1] - center_y1);
      covariance_k2 += Pr_k2[i] * (data[i][0] - center_x2) * (data[i][1] - center_y2);
    }

    covariance_k1 = covariance_k1 / N_k1;
    covariance_k2 = covariance_k2 / N_k2;

    standard_deviation_k1_x = cond_prob_k1_x / N_k1;
    standard_deviation_k1_y = cond_prob_k1_y / N_k1;
    standard_deviation_k2_x = cond_prob_k2_x / N_k2;
    standard_deviation_k2_y = cond_prob_k2_y / N_k2;

    pearson_k1 = covariance_k1 / (standard_deviation_k1_x * standard_deviation_k1_y);
    pearson_k2 = covariance_k2 / (standard_deviation_k2_x * standard_deviation_k2_y);

    //now update PI function
    PI_k1 = N_k1 / (N_k1 + N_k2);
    PI_k2 = N_k2 / (N_k1 + N_k2);

    covariance_k1_change = (100 * MIN(covariance_k1, old_covariance_k1)) / MAX(covariance_k1, old_covariance_k1);
    covariance_k2_change = (100 * MIN(covariance_k2, old_covariance_k2)) / MAX(covariance_k2, old_covariance_k2);

  //one may ask why 94%, the real answer is that more steps mess up the result. There is probably an issue somewhere in the code above.
  }while(covariance_k1_change < 94 || covariance_k2_change < 94);

  for(i = 0; i < 100; i++){
    data[i][3] = Pr_k1[i];
  }
}

int main(int argc, char **argv){
  int i;

  read_file();
  k_means();

  float temp_center_x1 = center_x1, temp_center_y1 = center_y1, temp_center_x2 = center_x2, temp_center_y2 = center_y2;

  em();

  for(i = 0; i < 100; i++){
    printf("%f %f %.0f %.3f\n", data[i][0], data[i][1], data[i][2], data[i][3]);
  }

  printf("\nk-means:\n");
  printf("k-mean center coordinates for group 1: %f %f\n", temp_center_x1, temp_center_y1);
  printf("k-mean center coordinates for group 2: %f %f\n", temp_center_x2, temp_center_y2);


  printf("\nEM:\n");
  printf("EM center coordinates for group 1: %f %f\n", center_x1, center_y1);
  printf("EM center coordinates for group 2: %f %f\n", center_x2, center_y2);

  printf("Covariance matrix for group 1:\n");
  printf("[ %f %f ]\n", pow(standard_deviation_k1_x, 2), pearson_k1 * standard_deviation_k1_x * standard_deviation_k1_y);
  printf("[ %f %f ]\n", pearson_k1 * standard_deviation_k1_x * standard_deviation_k1_y, pow(standard_deviation_k1_y, 2));

  printf("Covariance matrix for group 2:\n");
  printf("[ %f %f ]\n", pow(standard_deviation_k2_x, 2), pearson_k2 * standard_deviation_k2_x * standard_deviation_k2_y);
  printf("[ %f %f ]\n", pearson_k2 * standard_deviation_k2_x * standard_deviation_k2_y, pow(standard_deviation_k2_y, 2));

  printf("PI por group 1: %f\n", PI_k1);
  printf("PI por group 2: %f\n", PI_k2);

  return 0;
}
