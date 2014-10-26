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
void dicdfs(char *, int, int);
void dump();    //dump infomation to output file
void loadAnsHash(char *);
long long int stPT[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long long int edPT[] = {9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999, 9999999999, 99999999999, 999999999999};
#ifdef USE_STRING
bool testString(string str);
#else
bool testString(char str[], int len);
#endif
long long int guessedNumber = 0;
long long int hitNumber = 0;
long long int totalNumber = 100;
bool optFlag;
set<string> dicSet;
set<string> dicSetSimple;
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
        cout << input << endl;
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

    char tmpstr[1000];
    while(~fscanf(dic, "%s", tmpstr)) {
        int len = strlen(tmpstr);
        if (len >= 12 || len < 3) continue;
        for (int i = 0; i < len; i++) tmpstr[i] = toupper(tmpstr[i]);
        optFlag = false;
        dicdfs(tmpstr, 0, len);
    }
    fclose(dic);
}

#ifdef USE_STRING
bool testString(string str) {
    char rangeStr[20];
    string currentStr, appendStr;
    int len = str.length();
    int ind = 12 - len - 1;
    bool ishit = false;
    for (long long int range = 0; range <= 999; range++) {
        sprintf(rangeStr, "%lld", range);
        appendStr = rangeStr;
        currentStr = appendStr + str;
        ishit |= md5Test(currentStr.c_str(), currentStr.length());
        currentStr = str + appendStr;
        ishit |= md5Test(currentStr.c_str(), currentStr.length());
    }
    return ishit;
}
#else
bool testString(char str[], int len) {
    char rangeStr[20];
    bool ishit = false;
    for (int range = 0; range <= 999; range++) {
        sprintf(rangeStr, "%s%d", str, range);
        ishit |= md5Test(rangeStr, strlen(rangeStr));
        sprintf(rangeStr, "%d%s", range, str);
        ishit |= md5Test(rangeStr, strlen(rangeStr));
    }
    return ishit;
}
#endif
// make upper and lower choice
void dicdfs(char *str, int lv, int len) {
    if (totalNumber <= 0 || optFlag) {
        return ;
    }
    if (lv == len) {
#ifdef USE_STRING
        optFlag = testString((string)str);
#else
        optFlag = testString(str, len);
#endif
        return;
    } else {
        char tmpc = str[lv];
        str[lv] = toupper(str[lv]);
        dicdfs(str, lv+1, len);
        str[lv] = tolower(str[lv]);
        dicdfs(str, lv+1, len);
        str[lv] = tmpc;
    }
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
