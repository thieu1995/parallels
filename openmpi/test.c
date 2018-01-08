//
// Created by thieunv on 08/01/2018.
//

#include <stdio.h>

struct Date {
    int d;
    int m;
    int y;
} DATE;

struct Date m_date = {16, 9, 1989};
struct Date date;

int main() {
    FILE *fp = NULL;
    int n_byte;


    //openning file to writting
    fp = fopen("data.bin","wb");
    if(fp == NULL)
        printf("\nError in openning file\n");
    else
    {
        //ghi struct date xuong file data.dat
        if(fwrite(&m_date, sizeof(DATE), 1, fp) != 1)
            printf("Error in writting\n");
        fclose(fp);
    }

    //openning file to read
    fp = fopen("data.bin","rb");

    if(fp == NULL)
        printf("\nError in openning file\n");
    else
    {
//        if(fread(&date, sizeof(DATE), 1, fp) != 1)
//            printf("\nError in reading \n");
//        else
//        {
//            printf("\ndd/mm/yyyy = %.2d/%.2d/%.4d\n", date.d, date.m, date.y);
//        }
//        fclose(fp);

        int d, m, y;
        fread(&d, sizeof(int), 1, fp);
        fread(&m, sizeof(int), 1, fp);
        //fread(&y, sizeof(int), 1, fp);

        printf("\ndd/mm/yyyy = %.2d/%.2d/%.4d\n", d, m, y);
        fclose(fp);
    }
    return 0;
}