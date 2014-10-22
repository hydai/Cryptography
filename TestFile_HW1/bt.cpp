#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <vector>
//#define DUMPINFO
//#define StrFirst
using namespace std;
#define uint8_t unsigned char
extern void md5(const uint8_t *, size_t, uint8_t *);
void init(char *);
void loadDic();
void dicdfs(char *, int, int);
void dump();    //dump infomation to output file
void loadAnsHash(char *);
void btSimple(); // only test dictionary string
void btExtend(int); // test with numbers but string first
void btNumber(int); // test with numbers first
long long int guessedNumber = 0;
long long int hitNumber = 0;
long long int totalNumber = 100;
set<string> dicSet;
set<string> dicSetSimple;
set<string> ansSet;
map<long long int, string> hitsMap;
vector<string> tmpDicV;
#ifdef DUMPINFO
void dumpDicSet() {
    set<string>::iterator iter = dicSet.begin();
    FILE *dS = fopen("dicSet.txt", "w");
    for (; iter != dicSet.end(); iter++) {
        fprintf(dS, "%s\n", iter->c_str());
    }
    fclose(dS);
}
void dumpAnsHash() {
    set<string>::iterator iter = ansSet.begin();
    FILE *dS = fopen("ansSet.txt", "w");
    for (; iter != ansSet.end(); iter++) {
        fprintf(dS, "%s\n", iter->c_str());
    }
    fclose(dS);
}
#endif
int main(int argc, char *argv[])
{
    clock_t start, end;
    printf("Start loading\n");
    start = clock();
    if (argc < 2)
        init(NULL);
    else
        init(argv[1]);
#ifdef DUMPINFO
    dumpDicSet();
    dumpAnsHash();
#endif
    printf("Loading done\n");
    end = clock();
    printf("Load dictionary consumes %lf ms!\n", (double)(end-start));
    printf("Load dictionary consumes %lf s!\n", (double)(end-start)/CLOCKS_PER_SEC);
    start = clock();
    //btSimple();
#ifdef StrFirst
    btExtend(3);
#else
    btNumber(100);
#endif
    end = clock();
    printf("Running time consumes %lf ms!\n", (double)(end-start));
    printf("Running time consumes %lf s!\n", (double)(end-start)/CLOCKS_PER_SEC);
    dump();
    return 0;
}

void btSimple() {
    string currentStr;
    set<string>::iterator iter = dicSet.begin();
    uint8_t result[16];
    for (;iter != dicSet.end(); iter++) {
        guessedNumber++;
        md5((uint8_t *)(iter->c_str()), iter->length(), result);
        for (int i = 0; i < 16; i++) {
            char ss[10];
            sprintf(ss, "%2.2x", result[i]);
            currentStr += ss;
        }
        if (ansSet.find(currentStr) != ansSet.end()) {
            hitNumber++;
            hitsMap[guessedNumber] = currentStr;
        }
    }
}
void btNumber(int numLimit) {
    char frontStr[20], backStr[20];
    string currentStr, fStr, bStr;
    uint8_t result[16];
    set<string>::iterator iter;
    for (int front = 0; front < numLimit; front++) {
        for (int back = 0; back < numLimit; back++) {
            if (totalNumber <= 0) {
                return ;
            }
            sprintf(frontStr, "%d", front);
            sprintf(backStr, "%d", back);
            fStr = frontStr;
            bStr = backStr;

            iter = dicSetSimple.begin();
            for (;iter != dicSetSimple.end(); iter++) {
                guessedNumber++;
                md5((uint8_t *)((fStr + *iter).c_str()), (fStr + *iter).length(), result);
                for (int i = 0; i < 16; i++) {
                    char ss[10];
                    sprintf(ss, "%2.2x", result[i]);
                    currentStr += ss;
                }
                if (ansSet.find(currentStr) != ansSet.end()) {
                    totalNumber--;
                    hitNumber++;
                    hitsMap[guessedNumber] = currentStr;
                }
                guessedNumber++;
                md5((uint8_t *)((*iter + bStr).c_str()), (*iter + bStr).length(), result);
                for (int i = 0; i < 16; i++) {
                    char ss[10];
                    sprintf(ss, "%2.2x", result[i]);
                    currentStr += ss;
                }
                if (ansSet.find(currentStr) != ansSet.end()) {
                    totalNumber--;
                    hitNumber++;
                    hitsMap[guessedNumber] = currentStr;
                }
                guessedNumber++;
                md5((uint8_t *)((fStr + *iter + bStr).c_str()), (fStr + *iter + bStr).length(), result);
                for (int i = 0; i < 16; i++) {
                    char ss[10];
                    sprintf(ss, "%2.2x", result[i]);
                    currentStr += ss;
                }
                if (ansSet.find(currentStr) != ansSet.end()) {
                    totalNumber--;
                    hitNumber++;
                    hitsMap[guessedNumber] = currentStr;
                }
            }
        }
    }
}
void btExtend(int numLimit) {
    char frontStr[20], backStr[20];
    string currentStr, fStr, bStr;
    uint8_t result[16];
    set<string>::iterator iter;
    for (int front = 0; front < numLimit; front++) {
        for (int back = 0; back < numLimit; back++) {
            if (totalNumber <= 0) {
                return ;
            }
            sprintf(frontStr, "%d", front);
            sprintf(backStr, "%d", back);
            fStr = frontStr;
            bStr = backStr;

            iter = dicSet.begin();
            for (;iter != dicSet.end(); iter++) {
                guessedNumber++;
                md5((uint8_t *)((fStr + *iter).c_str()), (fStr + *iter).length(), result);
                for (int i = 0; i < 16; i++) {
                    char ss[10];
                    sprintf(ss, "%2.2x", result[i]);
                    currentStr += ss;
                }
                if (ansSet.find(currentStr) != ansSet.end()) {
                    totalNumber--;
                    hitNumber++;
                    hitsMap[guessedNumber] = currentStr;
                }
                guessedNumber++;
                md5((uint8_t *)((*iter + bStr).c_str()), (*iter + bStr).length(), result);
                for (int i = 0; i < 16; i++) {
                    char ss[10];
                    sprintf(ss, "%2.2x", result[i]);
                    currentStr += ss;
                }
                if (ansSet.find(currentStr) != ansSet.end()) {
                    totalNumber--;
                    hitNumber++;
                    hitsMap[guessedNumber] = currentStr;
                }
                guessedNumber++;
                md5((uint8_t *)((fStr + *iter + bStr).c_str()), (fStr + *iter + bStr).length(), result);
                for (int i = 0; i < 16; i++) {
                    char ss[10];
                    sprintf(ss, "%2.2x", result[i]);
                    currentStr += ss;
                }
                if (ansSet.find(currentStr) != ansSet.end()) {
                    totalNumber--;
                    hitNumber++;
                    hitsMap[guessedNumber] = currentStr;
                }
            }
        }
    }
}
// initialize
void init(char *hashPath) {
    loadDic();
    loadAnsHash(hashPath);
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
    fclose(ans);
}
// loading dictionary files
void loadDic() {
    FILE *dic = fopen("dictionary.txt", "r");
    char tmpstr[1000];
    while(~fscanf(dic, "%s", tmpstr)) {
        int len = strlen(tmpstr);
        for (int i = 0; i < len; i++) tmpstr[i] = toupper(tmpstr[i]);
        dicSetSimple.insert(tmpstr);
        /*
        dicdfs(tmpstr, 0, len);
        for (int i = 0; i < tmpDicV.size(); i++) {
            dicSet.insert(tmpDicV[i]);
        }
        */
    }
    fclose(dic);
}
// make upper and lower choice
void dicdfs(char *str, int lv, int len) {
    if (lv == 0) while(!tmpDicV.empty()) tmpDicV.pop_back();
    if (lv == len) {
        tmpDicV.push_back((string)str);
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
    fprintf(outputFile, "breaking rate : %lf %%\n", (double)guessedNumber/(double)hitNumber);
    fclose(outputFile);
}
