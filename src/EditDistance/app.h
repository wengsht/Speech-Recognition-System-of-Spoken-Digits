#ifndef __PRO3__EDITDISTANCE__APP__
#define __PRO3__EDITDISTANCE__APP__
#include <string>
#include <map>
#include <vector>
#include "Path.h"
#include "tool.h"
// !
using namespace std;
const int thread_num = 4;

struct cmp_task_info
{
	int index;
	const char * w;
	vector<string> * dict;
	vector<int> * result;
	int type;
	Path * p;
};

class APP {
	//
	WRITE_ONLY_DECLARE(bool ,fromFile,FromFile);
	
protected:
	WRITE_ONLY_DECLARE(char *, dictFile,Dict);// 只在单词中使用
	WRITE_ONLY_DECLARE(char *, betterStoryFile,BStory);//只在单词中使用

	WRITE_ONLY_DECLARE(char *, storyFile,Story);// 单词 句子 文章
	WRITE_ONLY_DECLARE(char *, cStoryFile,CStory);// 单词 句子 文章

	//单词模式，string存单个单词 //支持多个比较
	//句子模式，string存单个句子// !!单个分析
	//文章模式，string存一行，换行符隔开 //单个分析
	vector<string> story;
	vector<string> cStory;

	vector<Path> paths;	
	
	//在单词模式，去除重复单词，并将故事存成索引形式
	//其他模式由于没有字典并没有用到的必要
	vector<int> storyIndex; //index[i] = -1，则单词未出现过，否则与index[i]相同
	//
	void calcIndex(const vector<string>& org,
					vector<int>& index
	);

	static void cmp_task(void * in);
	
	//字典 只在单词模式
	vector<string> dic;
	//纠正结果，是字典的索引，只在单词模式
	vector<int> better;
	
	// 1 普通编辑距离
	// 2 阈值
	// 3 beam
	// 4 kb 树 // 只支持单词
	int alg;
	
	// 1 单词模式
	// 2 句子模式
	// 3 文章模式
	int mode;

	//文件输入（批量单词）
	bool fromFileJob();
	int calcAllWithThread();
	int calcOne();
	int calcOneWithDic();	
	//手动输入
	bool inputJob();
	//根据模式读文件
	void readFile();
	void printInfo();
	
	// line = true 则在一行内输出
	void print(const vector<string>& v,bool line = true)const;
	void print(const vector<int>& index,
					const vector<string> &v,
					bool line = true)const;
public:	
	APP();
	~APP();
	bool setMode(int t);
	bool setAlg(int t);
	bool run();
};

#endif
