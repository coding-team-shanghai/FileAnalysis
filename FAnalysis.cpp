#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

struct fileinfo {
	char filename[MAX_PATH*10];
	DWORD filesize;
};

fileinfo files[600000];
int ptr = 0;
int jumped = 0;

bool cmp(fileinfo f1, fileinfo f2) {
	if (f1.filesize != f2.filesize) {
		return f1.filesize > f2.filesize;
	} else {
		return strcmp(f1.filename, f2.filename);
	}
}

string db_to_str(double num) {
	char tmp[100];
	sprintf(tmp, "%.2lf", num);
	string rt;
	rt.clear();
	rt+=tmp;
	return rt;
}

string i_to_str(DWORD num) {
	char tmp[100];
	sprintf(tmp, "%llld", num);
	string rt;
	rt.clear();
	rt+=tmp;
	return rt;
}

string processname(DWORD fs) {
	string rt;
	rt.clear();
	if (fs < 2048) {
		rt += i_to_str(fs);
		rt += "B";
		return rt;
	}
	if (2048 <= fs && fs <= 2097152) {
		rt += db_to_str(fs/1024.00);
		rt += "KB";
		return rt;
	}
	if (1/*2048 <= fs && fs <= 2097152*/) {
		rt += db_to_str(fs/1048576.00);
		rt += "MB";
		return rt;
	}
}

int viewfiles(const char *directory)
//遍历文件函数，获取文件路径
{
    WIN32_FIND_DATA fdFindData;
    HANDLE hFile;
    HANDLE hFind;
    char *filename;
    int count=0;
    BOOL done;
    filename=new char[strlen(directory)+5];
    strcpy(filename,directory);
    strcat(filename,"\\*.*");
    hFind=FindFirstFile(filename,&fdFindData);
    delete[] filename;
    done=hFind!=INVALID_HANDLE_VALUE;
    while(done) {
        if (strcmp(fdFindData.cFileName,".") && strcmp(fdFindData.cFileName,"..")) {
            filename=new char[strlen(directory)+strlen(fdFindData.cFileName)+2];
            strcpy(filename,directory);
            strcat(filename,"\\");
            strcat(filename,fdFindData.cFileName);
            if(!(fdFindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
            	if (ptr % 100 == 0) {
            		//printf("\rProcessing the %dth file...", ptr);
				}
				if (ptr % 20000 == 0) {
					//putchar('\n');
				}
            	hFile = CreateFileA(filename, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
               	//cout << filename << " Filesize = " << GetFileSize(hFile, NULL) << endl;
               	sprintf(files[ptr].filename, "%s", filename);
               	LARGE_INTEGER lint;
               	WINBOOL getsizersp = GetFileSizeEx(hFile, &lint);
               	if (getsizersp == 0) {
               		printf("\n%dth File: Error.<getsizeex_rsp> Filename: %s\n", ptr, filename);//MessageBox(NULL, "Error", "", MB_OK);
				}
				if (lint.u.HighPart < 0) {
               		printf("\n%dth File: Error<highpart>. Filename: %s\n", ptr, filename);//MessageBox(NULL, "Error", "", MB_OK);
				}
				if (lint.QuadPart < 10240) {
					jumped++;
            		delete[] filename;
        			done=FindNextFile(hFind,&fdFindData);
					continue;
				}
				if (lint.u.HighPart == 0) {
               		files[ptr].filesize = lint.u.LowPart;
				} else {
					files[ptr].filesize = lint.QuadPart;//processbigsize(lint.u.LowPart, lint.u.HighPart);
				}
               	ptr++;
			} else {
         		viewfiles(filename);
			}
            delete[] filename;
        }
        done=FindNextFile(hFind,&fdFindData);
    }
    FindClose(hFind);
    return(count);
}

int main() {
	cout << "输入想分析的目录：";
	string directory;
	cin>>directory; 
	clock_t clock1, clock2, clock3;
	clock1 = clock();
	printf("Viewing Files...\n\n\n");
	//viewfiles("D:\\Github");
	viewfiles(directory.c_str());
	clock2 = clock();
	printf("\n\n\nSorting...\n\n\n");
	sort(files, files+ptr+1, cmp);
	printf("\n\n\nOutputing Response....\n\n\n");
	clock3 = clock();
	Sleep(1000);
	for (int i = 0; i < (ptr > 1000 ? 1000 : ptr); i++) {
		printf("Size: %s\t\tFileName: %s\n",processname(files[i].filesize).c_str(), files[i].filename);
	}
	printf("\n%d Files Processed\n", ptr);
	printf("\n%d Files Skipped (<10KB)\n", jumped);
	printf("\nviewfiles: %dms used.\nsortfiles: %dms used\n", clock2-clock1, clock3-clock2);
	cin.clear();
	system("pause");
	return 0;
}
