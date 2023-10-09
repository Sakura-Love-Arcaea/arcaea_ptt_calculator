#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

int count = 0;

//定義資料結構
struct SongRecord {
    char short_name[128];
    int difficulty;
    int score;
    int playPTT;
};


void quickSort(struct SongRecord songRecords[], int start, int end) {
    int i = start;
    int j = end;
    if (i >= j) { return; }
    int pivot = songRecords[(start + end) / 2].playPTT;
    while (i < j) {
        while (songRecords[i].playPTT > pivot) { i++; }
        while (songRecords[j].playPTT < pivot) { j--; }
        if (i < j) {
            struct SongRecord temp = songRecords[i];
            songRecords[i] = songRecords[j];
            songRecords[j] = temp;
            i++;
            j--; 
        } else if (i == j) {
            i++;
        }
    }
    quickSort(songRecords, start, j);
    quickSort(songRecords, i, end);
}

void sortData(struct SongRecord songRecords[]) {
    quickSort(songRecords, 0, sizeof(struct SongRecord));
}



/*載入資料*/
void loadData(struct SongRecord songRecords[]) {
    FILE *fp = fopen("best30.csv", "r");
    if (fp == NULL) { puts("no file\n"); return; }
    char header[256];
    fgets(header, sizeof(header), fp);//丟掉

    int n = 0;
    //計數器
    while (fgets(header, sizeof(header), fp) != NULL) {
        char short_name[128];
        sscanf(header, "%[^,],%d,%d,%d,",
        short_name,
        &songRecords[n].difficulty,
        &songRecords[n].score,
        &songRecords[n].playPTT);
        strcpy(songRecords[n].short_name, short_name);
        n++;
    }
    
    fclose(fp);
    count = n;
}
/*印出資料(排序)*/
void printAll(struct SongRecord songRecords[], int numberOfRecord) {
    sortData(songRecords);
    printf("%-4s%-25s%5s%9s%11s\n\n", "#", "歌名", "難度", "分數", "PlayPTT");
    int i = 0;
    while (songRecords[i].score != 0) {
        if (i == numberOfRecord) { return; }
        printf("%-4d%-25s%4.1f%9d%9.4f\n",
        i + 1,
        songRecords[i].short_name,
        (float) songRecords[i].difficulty / 10,
        songRecords[i].score,
        (float) songRecords[i].playPTT / 10000);
        i++;
    }
}
void b30(struct SongRecord songRecords[]) {
    int b30 = 0;
    int r10 = 0;
    for (int i = 0; i < 30; i++) {
        b30 += songRecords[i].playPTT;
        if (i == 9) {
            r10 = b30;
        }
    }

    printf("b30: %.4f\tr10: %.4f\nPTT: %.4f\n", b30 / 30 / 10000.0, r10 / 10 / 10000.0, (b30 + r10) / 40 / 10000.0);

}
/*結果寫回*/
int writeBack(struct SongRecord songRecords[]) {
    FILE *fp = fopen("../best30.csv", "w");

    fprintf(fp, "%s\n", "歌名,難度,分數,PlayPTT");
    int i = 0;
    while (songRecords[i].score != 0) {
        fprintf(fp, "%s,%d,%d,%d\n",
        songRecords[i].short_name,
        songRecords[i].difficulty,
        songRecords[i].score,
        songRecords[i].playPTT);
        i++;
    }
    fclose(fp);
    return i;
}
/*功能1——插入新成績*/
void insert(struct SongRecord songRecords[]) {
    char short_name[128];
    puts("請輸入歌名、難度、分數(忽略小數點,並用逗號分隔)");
    fscanf(stdin, "%[^,],%d,%d",
        short_name,
        &songRecords[count].difficulty,
        &songRecords[count].score
    );
    strcpy(songRecords[count].short_name, short_name);
    
    if (songRecords[count].score > 10000000) {
        songRecords[count].playPTT = (songRecords[count].difficulty + 20) * 1000;
    } else if (songRecords[count].score > 9800000) {
        songRecords[count].playPTT = (songRecords[count].score - 9800000) / 20 + (songRecords[count].difficulty + 10) * 1000;
    } else {
        songRecords[count].playPTT = (songRecords[count].score - 9500000) / 30 + (songRecords[count].difficulty) * 1000;
    }
    count++;
}



int main(void) {
    struct SongRecord songRecords[300];
    loadData(songRecords);
    printAll(songRecords, 30);
    //insert(songRecords);
    //printAll(songRecords);
    b30(songRecords);
    writeBack(songRecords);

    return 0;
}