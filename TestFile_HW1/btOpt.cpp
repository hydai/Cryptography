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
using namespace std;
#define uint8_t unsigned char
extern void md5(const uint8_t *, size_t, uint8_t *);
bool md5Test(const char *input, int length);
void init(char *, char *);
void loadDic(char *);
void dicdfs(char *, int);
void dump();    //dump infomation to output file
void loadAnsHash(char *);
bool testString(char str[], int len);
long long int guessedNumber = 0;
long long int hitNumber = 0;
long long int totalNumber = 100;
int *dfsMapping[11];
bool optFlag;
set<string> ansSet;
map<long long int, string> hitsMap;
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
    guessedNumber++;
    for (int i = 0; i < 16; i++) {
        char ss[10];
        sprintf(ss, "%2.2x", result[i]);
        retString += ss;
    }
    if (ansSet.find(retString) != ansSet.end()) {
        printf("%s\n", input);
        totalNumber--;
        hitNumber++;
        hitsMap[guessedNumber] = input;
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
    char tmpstr[1000];
    while(~fscanf(dic, "%s", tmpstr)) {
        int len = strlen(tmpstr);
        if (len >= 12 || len < 3) continue;
        bool isContainNotAlpha = false;
        for (int i = 0; i < len; i++) {
            if (isalpha(tmpstr[i]) == 0) {isContainNotAlpha = true; break;}
        }
        if (isContainNotAlpha) {continue;}
        for (int i = 0; i < len; i++) tmpstr[i] = toupper(tmpstr[i]);
        optFlag = false;
        dicdfs(tmpstr, len);
    }
    fclose(dic);
}

int stPT[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int edPT[] = {9, 99, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999};
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
// make upper and lower choice
void dicdfs(char *str, int len) {
    if (totalNumber <= 0 || optFlag) {
        return ;
    }
    char ttstr[100];
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
        optFlag = testString(ttstr, len);
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
    fprintf(outputFile, "breaking rate : %lf %%\n", (double)hitNumber/(double)guessedNumber);
    fclose(outputFile);
}
/* test 3->8 */
