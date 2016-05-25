/*
Copyright (c) 2016 Frederico Schardong
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <sys/stat.h>
#include <stdint.h>
#include <float.h>

float data[100][3] = {0};

float center_x1, center_y1, center_x2, center_y2;

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
    int total_k0 = 0, total_k1 = 0;

    for(i = 0; i < 100; i++){
      if(data[i][2]){
        total_center_x1 += data[i][0];
        total_center_y1 += data[i][1];
        total_k0++;
      }
      else{
        total_center_x2 += data[i][0];
        total_center_y2 += data[i][1];
        total_k1++;
      }
    }
    
    center_x1 = total_center_x1 / total_k0;
    center_y1 = total_center_y1 / total_k0;
    center_x2 = total_center_x2 / total_k1;
    center_y2 = total_center_y2 / total_k1;

    //printf("Changing center x1 from %f to %f\n", old_center_x1, center_x1);
  }while(old_center_x1 != center_x1 && old_center_y1 != center_y1 && old_center_x2 != center_x2 && old_center_y2 != center_y2);
}

int main(int argc, char **argv){
  int i;

  read_file();
  k_means();

  for(i = 0; i < 100; i++){
    printf("%f %f %.0f\n", data[i][0], data[i][1], data[i][2]);
  }

  printf("k-mean center coordinates for group 1: %f %f\n", center_x1, center_y1);
  printf("k-mean center coordinates for group 2: %f %f\n", center_x2, center_y2);

  return 0;
}
