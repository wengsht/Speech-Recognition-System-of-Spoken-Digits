#ifndef __PRO3__EDITDISTANCE__PATH__
#define __PRO3__EDITDISTANCE__PATH__
enum DIR{
    DELETE,INSERT,SAME,CHANGE
};
struct Path{
    int len;
    // 右删词+1
    // 对角无消耗
    // 上加词+1
    // 对角消耗+1
    DIR * dir;
    int * val;

	Path(){
		dir = NULL;
		val = NULL;
	};

    Path(int m){
        len = 0;
        dir = new DIR[m];
		val = NULL;
    }

    void print(){
        for(int i = len-1;i>=0;i--){
            if(dir[i] == INSERT)printf("insert-");
            else if(dir[i] == DELETE)printf("delete-");
            else if(dir[i] == SAME)printf("same-");
            else if(dir[i] == CHANGE)printf("change-");
        }puts("");
    }
	
    void setMaxLen(int m){
        len = 0;
        if(dir!=NULL)delete dir;
        dir = new DIR[m];
		if(val!=NULL)delete val;
		val = NULL;
	}
    void add(DIR t){
        dir[len++] = t;
    }
    ~Path(){
       if(dir) delete dir;
		if(val)delete val;
	}
};
struct Count{
	int del;
	int ins;
	int same;
	int change;
	void print(){
		printf("\nDelete letter : %d\n",del);
		printf("Inserted letter : %d\n",ins);
		printf("Change letter : %d\n",change);
		printf("Same letter : %d\n",same);
	}
	void addPath(const Path& p){
		for(int i = 0;i<p.len;i++){
			if(p.dir[i] == DELETE) del++;
			if(p.dir[i] == INSERT) ins++;
			if(p.dir[i] == CHANGE) change++;
			if(p.dir[i] == SAME) same++;
		}
	}
	Count(){
		del = ins = same = change = 0;
	}
};



#endif

