#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <vector>
//#define DUMPINFO
using namespace std;
extern void md5(const uint8_t *, size_t, uint8_t *);
void init();
void loadDic();
void dicdfs(char *, int, int);
void dump();    //dump infomation to output file
void loadAnsHash();
void btSimple(); // only test dictionary string
void btExtend(); // test with numbers
void btFront(int);
void btBack(int);
void btFAB(int);
long long int guessedNumber = 0;
long long int hitNumber = 0;
set<string> dicSet;
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
    init();
#ifdef DUMPINFO
    dumpDicSet();
    dumpAnsHash();
#endif
    printf("Load done\n");
    btSimple();
    btExtend();
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
void btFront(int lv) {
    string currentStr;
    set<string>::iterator iter = dicSet.begin();
    uint8_t result[16];
    for (;iter != dicSet.end(); iter++) {
        string tmps = *iter;
        char OAO[4] = {0};
        for (int i = '0'; i <= '9'; i++)
            for (int j = '0'; j <= '9'; j++)
                for (int k = '0'; k <= '9'; k++) {
                    OAO[0] = i; OAO[1] = j; OAO[2] = k;
                    string QAQ = OAO;

                    guessedNumber++;
                    md5((uint8_t *)((QAQ+tmps).c_str()), (iter->length())+(size_t)3, result);
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
}
void btBack(int lv) {

}
void btFAB(int lv) {

}
void btExtend() {
    //for (int i = 1; i < 3; i++)
        btFront(3);
    for (int i = 1; i < 3; i++)
        btBack(i);
    for (int i = 1; i < 3; i++)
        for (int j = 1; j < 3; j++)
            btFAB(i);
}
// initialize
void init() {
    loadDic();
    loadAnsHash();
}
// loading hashed password
void loadAnsHash() {
    FILE *ans = fopen("HashedPassword.txt", "r");
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
        dicdfs(tmpstr, 0, len);
        for (int i = 0; i < tmpDicV.size(); i++) {
            dicSet.insert(tmpDicV[i]);
        }
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
