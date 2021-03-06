#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <pthread.h>
using namespace std;
#define THREAD_NUM 8
#define uint8_t unsigned char
/* ========== Function declaration ========== */
extern void md5(const uint8_t *, size_t, uint8_t *);
bool md5Test(const char *input, int length);
void init(char *, char *);
void loadDic(char *);
void dicdfs(char *, int, bool);
void dump();    //dump infomation to output file
void loadAnsHash(char *);
bool testString(char str[], int len);
bool testStringV2(char str[], int len);
/* ========== Variable declaration ========== */
long long int chanceNumber = 4000000000;
long long int guessedNumber = 0;
long long int hitNumber = 0;
long long int totalNumber = 100;
int *dfsMapping[11];
bool optFlag;
set<string> ansSet;
set<string> dicSet;
map<long long int, string> hitsMap;
/* ========== Pthread  declaration ========== */
pthread_mutex_t mutexInputString;
pthread_mutex_t mutexNumberChange;
void *pthreadTest(void *);
int main(int argc, char *argv[])
{
    clock_t start, end;
    printf("Start loading\n");
    start = clock();
    if (argc < 2)
        init(NULL, NULL);
    else
        init(argv[1], argv[2]);
    printf("Loading done\n");
    end = clock();
    printf("Load dictionary consumes %lf ms!\n", (double)(end-start));
    printf("Load dictionary consumes %lf s!\n", (double)(end-start)/CLOCKS_PER_SEC);
    dump();
    return 0;
}

bool md5Test(const char *input, int length) {
    string retString = "";
    uint8_t result[16];
    md5((uint8_t *)input, length, result);
    for (int i = 0; i < 16; i++) {
        char ss[10];
        sprintf(ss, "%2.2x", result[i]);
        retString += ss;
    }
    chanceNumber--;
    guessedNumber++;
    if (ansSet.find(retString) != ansSet.end()) {
        printf("%s\n", input);
        pthread_mutex_lock(&mutexNumberChange);
        totalNumber--;
        hitNumber++;
        hitsMap[guessedNumber] = input;
        pthread_mutex_unlock(&mutexNumberChange);
        return true;
    }
    return false;
}

// initialize
void init(char *hashPath, char *dicPath) {
    loadAnsHash(hashPath);
    loadDic(dicPath);
}

// loading hashed password
void loadAnsHash(char *path) {
    FILE *ans;
    if (path == NULL)
        ans = fopen("HashedPassword.txt", "r");
    else
        ans = fopen(path, "r");

    char tmpstr[1000];
    while(~fscanf(ans, "%s", tmpstr)) {
        ansSet.insert((string)tmpstr);
    }
    totalNumber = ansSet.size();
    fclose(ans);
}

// loading dictionary files
void loadDic(char *path) {
    FILE *dic;
    if (path == NULL)
        dic = fopen("dictionary.txt", "r");
    else
        dic = fopen(path, "r");

    for (int i = 0; i < 11; i++) {
        int size = 2 << i;
        dfsMapping[i] = (int *) malloc ((size)*sizeof(int));
    }
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < (2<<i); j++) {
            dfsMapping[i][j] = j;
        }
    }
    pthread_t pthreads[THREAD_NUM];
    pthread_t pthreadsRe[THREAD_NUM];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutexInputString, NULL);
    pthread_mutex_init(&mutexNumberChange, NULL);

    for (int i = 0; i < THREAD_NUM; i++) {
        int ret = pthread_create(&pthreads[i], &attr, pthreadTest, (void *)dic);
        if (ret) {
            fprintf(stderr, "ERROR! pthread_create FAILED!\n");
            exit(-1);
        }
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        int status;
        pthread_join(pthreads[i], (void **)&status);
    }
    fclose(dic);

    // Remainer test
    char tmpstr[20];
    set<string>::iterator iter = dicSet.begin();
    for (; iter != dicSet.end() && chanceNumber > 0; iter++) {
        strcpy(tmpstr, iter->c_str());
        dicdfs(tmpstr, iter->length(), true);
    }

    for (int i = 0; i < 11; i++) {
        free(dfsMapping[i]);
    }
    pthread_attr_destroy(&attr);
}

void *pthreadTest(void *dicPtr) {
    char tmpstr[1000];
    FILE *dic = (FILE *)dicPtr;
    bool isDone = false;
    while(1) {
        pthread_mutex_lock(&mutexNumberChange);
        if (chanceNumber <= 0) {
            isDone = true;
        }
        pthread_mutex_unlock(&mutexNumberChange);
        if (isDone) {
            break;
        }

        pthread_mutex_lock(&mutexInputString);
        int ret = fscanf(dic, "%s", tmpstr);
        pthread_mutex_unlock(&mutexInputString);

        if (ret != 1) break;
        printf("PUSHEEN %s\n", tmpstr);
        int len = strlen(tmpstr);
        if (len >= 12) continue;
        bool isContainNotAlpha = false;
        for (int i = 0; i < len; i++) {
            if (isalpha(tmpstr[i]) == 0) {isContainNotAlpha = true; break;}
        }
        if (isContainNotAlpha) {continue;}
        for (int i = 0; i < len; i++) tmpstr[i] = toupper(tmpstr[i]);
        dicSet.insert((string)tmpstr);
        optFlag = false;
        dicdfs(tmpstr, len, false);
    }
    pthread_exit(NULL);
}
int stPT[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int edPT[] = {9, 99, 999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999};
bool testString(char str[], int len) {
    char rangeStr[20];
    int ind = 12 - len - 1;
    bool ishit = false;
    for (int range = stPT[ind]; range <= edPT[ind]; range++) {
        sprintf(rangeStr, "%s%d", str, range);
        ishit |= md5Test(rangeStr, strlen(rangeStr));
        sprintf(rangeStr, "%d%s", range, str);
        ishit |= md5Test(rangeStr, strlen(rangeStr));
    }
    return ishit;
}
bool testStringV2(char str[], int len) {
    char rangeStr[20];
    bool ishit = false;
    for (int front = 0; front <= 99; front++) {
        for (int back = 0; back <= 99; back++) {
            sprintf(rangeStr, "%d%s%d", front, str, back);
            if (strlen(rangeStr) > 12) break;
            ishit |= md5Test(rangeStr, strlen(rangeStr));
        }
    }
    return ishit;
}
// make upper and lower choice
void dicdfs(char *str, int len, bool isRemain) {
    if (totalNumber <= 0 || chanceNumber <= 0) {
        return ;
    }
    char ttstr[20] = {0};
    for (int i = 0; i < (2 << (len-1)); i++) {
        int tmpt = dfsMapping[len-1][i];
        for (int j = len-1; j >= 0; j--) {
            if (tmpt & 1) {
                ttstr[j] = toupper(str[j]);
            } else {
                ttstr[j] = tolower(str[j]);
            }
            tmpt = tmpt >> 1;
        }
        if (!isRemain) {
            optFlag = testString(ttstr, len);
        } else {
            optFlag = testStringV2(ttstr, len);
        }
        if (optFlag || chanceNumber <= 0)
            break;
    }
    return;
}
// dump to output file
void dump() {
    FILE *outputFile = fopen("OutputFile.txt", "w");
    fprintf(outputFile, "------------------------------------------\n");
    map<long long int, string>::iterator iter = hitsMap.begin();
    for (; iter != hitsMap.end(); iter++) {
        fprintf(outputFile, "%s\t在第 %lld 個sequence 被猜中\n", iter->second.c_str(), iter->first);
    }
    fprintf(outputFile, "------------------------------------------\n");
    fprintf(outputFile, "猜了 %lld 個 sequence\n", guessedNumber);
    fprintf(outputFile, "猜中了 %lld 個 password\n", hitNumber);
    fprintf(outputFile, "breaking rate : %lf %%\n", (double)hitNumber/(double)ansSet.size()*100.0);
    fclose(outputFile);
}
/* test 3->8 */
