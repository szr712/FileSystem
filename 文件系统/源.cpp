//+?的说明还没写 错误字符 cd的路径
#include<iostream>
#include<algorithm>
#include<string.h>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cmath>
#include<stack>
#include<queue>
#include<stdio.h>
//#include<Windows.h>
#include<fstream>
#include<iomanip>
using namespace std;



vector<string> records;
int reco = 0;

struct user
{
	string name;//用户名
	string password;//密码
};

struct file
{
	string name;//文件名
	vector<string> content;//文件内容
	user owner;//文件所有者
};

struct dir {
	string name;//目录名
	dir* pre;//父目录
	map<string, file*> files;//所包含的文件
	map<string, dir*> next;//子目录
};

user curuser;//当前用户
dir* curdir;//当前目录
dir* root;//根目录

void initDir() {
	curdir = new dir();
	curdir->name = "~";
	curdir->pre = NULL;
	root = curdir;
}

void displayPath() {
	dir* tmp = curdir;
	vector<string> path;
	while (tmp != NULL) {
		path.push_back(tmp->name);
		tmp = tmp->pre;
	}
	cout << "~";
	for (int i = path.size() - 2; i >= 0; i--) {
		cout << "/" << path[i];
	}
	cout << "$ ";
}

//按照‘/’拆分字符串
vector<string> split(string str) {
	vector<string> ans;
	string tmp = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] != '/') {
			tmp = tmp + str[i];
		}
		else {
			ans.push_back(tmp);
			tmp = "";
		}
	}
	if (tmp != "")ans.push_back(tmp);
	return ans;
}

dir* pathTrans(string path) {
	string tmp = path;
	//绝对路径
	if (path[0] == '~' || path[0] == '/') {
		dir *cur = root;
		if (path[0] == '/')path = "~" + path;
		vector<string> tmp = split(path);
		for (int i = 1; i < tmp.size(); i++) {
			if (cur->next.find(tmp[i]) == cur->next.end()) {
				return NULL;
			}
			cur = cur->next[tmp[i]];
		}
		return cur;
	}
	//相对路径
	else {
		dir *cur = curdir;
		vector<string> tmp = split(path);
		for (int i = 0; i < tmp.size(); i++) {
			if (tmp[i] == ".") {

			}
			else if (tmp[i] == "..") {
				if (cur == root) {//不能再往上走了
					return NULL;
				}
				else {
					cur = cur->pre;
				}
			}
			else if (cur->next.find(tmp[i]) == cur->next.end()) {
				return NULL;
			}
			else if (cur->next.find(tmp[i]) != cur->next.end()) {
				cur = cur->next[tmp[i]];
			}
		}
		return cur;
	}
	return NULL;
}

bool judgeName(string name) {
	if (name.find("\\") != name.npos || name.find("/") != name.npos || name.find(":") != name.npos || name.find("*") != name.npos || name.find("?") != name.npos
		|| name.find("<") != name.npos || name.find(">") != name.npos || name.find("|") != name.npos || name.find("\"") != name.npos || name.find("..") != name.npos || name.find(" ") != name.npos) return false;
	return true;
}

void login() {
	bool flag = 1;
	map<string, string> users;
	system("cls");
	cout << "\n\n\n";
	cout << "\t\t            操作系统课程设计-模拟文件系统" << endl << endl << endl << endl << endl << endl;
	cout << "\t\t             作者：Alex" << endl << endl << endl;
	ifstream in("user.dat");
	if (!in) {
		cout << "打开文件出错！即将退出！" << endl;
		system("Pause");
		exit(1);
	}
	string tname, tpass;
	while (in >> tname >> tpass) {
		users[tname] = tpass;
	}
	printf("username:");
	cin >> tname;
	while (flag) {

		printf("password:");
		cin >> tpass;
		if (users.find(tname) == users.end()) {
			flag = 0;
		}
		else {
			if (users[tname] == tpass) {
				system("cls");
				curuser.name = tname;
				curuser.password = tpass;
				break;
			}
			else {
				printf("password is incorrect!\n");
			}
		}
	}
	if (!flag) {
		printf("This user is not exist.\nDo you want to creat a new user?(y/n):");
		char choice;
		cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			users[tname] = tpass;
			curuser.name = tname;
			curuser.password = tpass;
			system("cls");
		}
		else {
			login();
		}
	}
	//重新存回
	ofstream out("user.dat");
	for (auto it = users.begin(); it != users.end(); it++) {
		out << it->first << " " << it->second << endl;
	}
	getchar();
	return;
}

void help() {
	printf("command: \n\
	help   ---  show help menu \n\
	cd     ---  change the shell working directory \n\
	cls    ---  clear the screen \n\
	ls     ---  show all the files and directories in particular directory \n\
	mkdir  ---  create the directory,if it does not already exist.   \n\
	touch  ---  create a new file \n\
	gedit  ---  read and write something to a particular file \n\
	rm     ---  delete a exist file or a directory\n\
	cp     ---  copy SOURSE to DEST \n\
	rename ---  rename a exist file or a directory \n\
	su     ---  switch current user \n\
	exit   ---  exit this system \n");
}

void help2(string command) {
	if (command == "cd?") printf("cd: cd [dir]\nchange the shell working directory.\n");
	else if (command == "ls?")printf("ls: ls [dir]\nShow all the files and directories in particular directory.\n");
	else if (command == "mkdir?") printf("mkdir: mkdir dir\nCreate the directory,if it does not already exist.\n");
	else if (command == "touch?")printf("touch: touch file\nCreate a new file,if it does not already exist.\n");
	else if (command == "gedit?")printf("gedit: gedit file\nRead and write something to a particular file.\n");
	else if (command == "rm?")printf("rm: rm -d|-f file|dir\nDelete a exist file or a directory.\noption:\n-d:delete a directory\n-f:delete a file\n");
	else if (command == "cp?")printf("cp: cp -d|-f|-cd|-cf SOURSE DEST\nCopy SOURSE to DEST\noption:\n-d:copy a directory\n-f:copy a file\n-cd:cut a directory\n-cf:cut a file\n");
	else if (command == "rename?")printf("rename: rename -d|-f oldname newname\nRename a exist file or a directory.\noption:\n-d:rename a directory\n-f:rename a file\n");
	else if (command == "su?")printf("Switch current user.\n");
	else if (command == "cls?")printf("Clear the screen.\n");
	else if (command == "exit?")printf("Exit this system.\n");
	else printf("Wrong Command!\n");

}

//创建目录
void mkdir(string name) {
	if (name == "") {
		cout << "Require Parameters" << endl;
	}
	else if (curdir->next.find(name) != curdir->next.end()) {
		cout << "There is a directory having same name.\n";
	}
	else if (!judgeName(name)) {
		cout << "Name has at least a illegal character.\n";
	}
	else {
		dir *tmp = new dir();//一定要这样创建，否则字符串后面就不能读取
		tmp->name = name;
		tmp->pre = curdir;
		curdir->next[name] = tmp;
	}
}

//cd
void cd(string name) {
	if (name == "") {
		dir* tmp = curdir;
		vector<string> path;
		while (tmp != NULL) {
			path.push_back(tmp->name);
			tmp = tmp->pre;
		}
		cout << "~";
		for (int i = path.size() - 2; i >= 0; i--) {
			cout << "/" << path[i];
		}
		cout << endl;
	}
	else {
		dir* tmp = pathTrans(name);
		if (tmp == NULL) {
			cout << "No Such Directory.\n";
		}
		else {
			curdir = tmp;
		}
	}
}

//建立文件
void touch(string name) {
	//string name;//文件名
	//cin >> name;
	if (name == "") {
		cout << "Require Parameters" << endl;
	}
	else if (curdir->files.find(name) != curdir->files.end()) {
		cout << "There is a same file.\n";
	}
	else if (!judgeName(name)) {
		cout << "Name has at least a illegal character.\n";
	}
	else
	{
		file *tmp = new file();
		tmp->name = name;
		tmp->owner = curuser;
		tmp->content.push_back("");
		curdir->files[name] = tmp;
	}
}

//ls
void ls(string path) {
	dir *tmp = curdir;
	if (path != "") {
		curdir = pathTrans(path);
		if (curdir == NULL) {
			curdir = tmp;
			printf("Wrong Path!\n");
			return;
		}
	}
	for (auto it = curdir->files.begin(); it != curdir->files.end(); it++) {
		cout << it->first << setw(25 - it->first.length()) << "<FILE>" << "  owner:" << it->second->owner.name << endl;
	}
	for (auto it = curdir->next.begin(); it != curdir->next.end(); it++) {
		cout << it->first << setw(26 - it->first.length()) << "<DIR>  " << endl;
	}
	curdir = tmp;
}

//编辑文件
void gedit(string name) {
	dir *t = curdir;
	if (name == "") {
		cout << "Require Parameters" << endl;
		return;
	}
	else if (name.find_last_of('/') != name.npos) {
		string tmp = name;
		if (tmp[0] == '/') tmp = "~" + tmp;
		tmp = tmp.substr(0, tmp.find_last_of('/'));
		name = name.substr(name.find_last_of('/') + 1, name.length() - name.find_last_of('/'));
		curdir = pathTrans(tmp);
		if (curdir == NULL) {
			curdir = t;
			printf("Wrong Path!\n");
			return;
		}
	}
	if (curdir->files.find(name) == curdir->files.end()) {
		cout << "No Such File.\n";
	}
	else if (curdir->files[name]->owner.name != curuser.name) {
		cout << "This file is not yours!\n";
	}
	else {
		ofstream out("tmp.dat");
		for (int i = 0; i < curdir->files[name]->content.size(); i++) {
			out << curdir->files[name]->content[i] << endl;
		}
		out.close();
		system("notepad tmp.dat");
		ifstream in("tmp.dat");
		string t;
		curdir->files[name]->content.clear();//清除原来数组内的内容
		while (getline(in, t)) {
			curdir->files[name]->content.push_back(t);
		}
	}
	curdir = t;
}

//重命名
void rename(string tmp) {
	dir *t = curdir;
	string option = "";
	if (tmp.find_first_of('-') != tmp.npos&&tmp.length() >= 2) {
		option = tmp.substr(tmp.find_first_of('-'), 2);
	}
	else {
		cout << "Wrong Option!\n";
		return;
	}
	string old = "", newname = "";
	old = tmp.substr(tmp.find_first_of('-') + 3, tmp.find_last_of(' ') - tmp.find_first_of('-') - 3);
	newname = tmp.substr(tmp.find_last_of(' ') + 1, tmp.length() - tmp.find_last_of(' '));
	if (!judgeName(newname)) {
		cout << "Newname has at least a illegal character.\n";
		return;
	}
	if (newname == "" || old == "" || newname == old) {
		cout << "Require Parameters" << endl;
		return;
	}
	if (old.find_last_of('/') != old.npos) {
		string tmp = old;
		if (tmp[0] == '/') tmp = "~" + tmp;
		tmp = tmp.substr(0, tmp.find_last_of('/'));
		old = old.substr(old.find_last_of('/') + 1, old.length() - old.find_last_of('/'));
		curdir = pathTrans(tmp);
		if (curdir == NULL) {
			curdir = t;
			printf("Wrong Path!\n");
			return;
		}
	}
	if (option == "-d") {
		if (curdir->next.find(old) == curdir->next.end()) {
			cout << "No Such Directory.\n";
		}
		else if (curdir->next.find(newname) != curdir->next.end()) {
			cout << "There is a directory having same name.\n";
		}
		else {
			dir *tmp = curdir->next[old];
			curdir->next.erase(old);
			tmp->name = newname;
			curdir->next[newname] = tmp;
		}
	}
	else if (option == "-f") {
		if (curdir->files.find(old) == curdir->files.end()) {
			cout << "No Such File.\n";
		}
		else if (curdir->files.find(newname) != curdir->files.end()) {
			cout << "There is a same file.\n";
		}
		else if (curdir->files[old]->owner.name != curuser.name) {
			cout << "This file is not yours!\n";
		}
		else {
			file *tmp = curdir->files[old];
			curdir->files.erase(old);
			tmp->name = newname;
			curdir->files[newname] = tmp;
		}
	}
	else {
		cout << "Wrong Option!\n";
	}
	curdir = t;
}

//递归删除目录
void deletedir(dir *cur) {
	//先删文件
	for (auto it = cur->files.begin(); it != cur->files.end(); it++) {
		delete(it->second);
	}
	cur->files.clear();
	//再删目录，要嵌套删除
	for (auto it = cur->next.begin(); it != cur->next.end(); it++) {
		deletedir(it->second);
	}
	cur->next.clear();
	delete(cur);
}

//删除
void rm(string tmp) {
	string option = "";
	dir *t = curdir;
	if (tmp.find_first_of('-') != tmp.npos&&tmp.length() >= 2) {
		option = tmp.substr(tmp.find_first_of('-'), 2);
	}
	else {
		cout << "Wrong Option!\n";
		return;
	}
	string name = "";
	name = tmp.substr(tmp.find_first_of('-') + 3, tmp.length() - tmp.find_first_of('-') - 3);
	if (name.find_last_of('/') != name.npos) {
		string tmp = name;
		if (tmp[0] == '/') tmp = "~" + tmp;
		tmp = tmp.substr(0, tmp.find_last_of('/'));
		name = name.substr(name.find_last_of('/') + 1, name.length() - name.find_last_of('/'));
		curdir = pathTrans(tmp);
		if (curdir == NULL) {
			curdir = t;
			printf("Wrong Path!\n");
			return;
		}
	}
	if (name == "") {
		cout << "Require Parameters" << endl;
	}
	else if (option == "-d") {
		if (curdir->next.find(name) == curdir->next.end()) {
			cout << "No Such Directory.\n";
		}
		else {
			deletedir(curdir->next[name]);
			curdir->next.erase(name);
		}
	}
	else if (option == "-f") {
		if (curdir->files.find(name) == curdir->files.end()) {
			cout << "No Such File.\n";
		}
		else if (curdir->files[name]->owner.name != curuser.name) {
			cout << "This file is not yours!\n";
		}
		else {
			delete(curdir->files[name]);
			curdir->files.erase(name);
		}
	}
	else {
		cout << "Wrong Option!\n";
	}
}

//递归复制目录
dir* cpDir(dir *tmp) {
	dir *goal = new dir(*tmp);
	//清除原来的内容
	goal->next.clear();
	goal->files.clear();
	//把文件重建
	for (auto it = tmp->files.begin(); it != tmp->files.end(); it++) {
		file *f = new file(*(it->second));
		goal->files[it->first] = f;
	}
	//重建目录
	for (auto it = tmp->next.begin(); it != tmp->next.end(); it++) {
		dir *d = cpDir(it->second);
		d->pre = goal;
		goal->next[it->first] = d;
	}
	return goal;
}

//复制
void cp(string tmp) {
	string option = "";
	if (tmp.find_first_of('-') != tmp.npos&&tmp.length() >= 3) {
		option = tmp.substr(tmp.find_first_of('-'), tmp.find_first_of(' ', tmp.find_first_of('-')) - tmp.find_first_of('-'));
	}
	else {
		cout << "Wrong Option!\n";
		return;
	}
	string source = "", dest = "", name = "";
	source = tmp.substr(tmp.find_first_of(' ', tmp.find_first_of('-')) + 1, tmp.find_last_of(' ') - tmp.find_first_of(' ', tmp.find_first_of('-')) - 1);
	dest = tmp.substr(tmp.find_last_of(' ') + 1, tmp.length() - tmp.find_last_of(' '));
	dir *sou = NULL, *den = NULL;
	if (source.find_first_of("/") == source.npos) {
		sou = curdir;
		name = source;
	}
	else {
		if (source[0] == '/') source = "~" + source;
		name = source.substr(source.find_last_of('/') + 1, source.length() - source.find_last_of('/'));
		source = source.substr(0, source.find_last_of('/'));
		sou = pathTrans(source);
	}
	den = pathTrans(dest);
	if (den == NULL) {
		printf("Wrong Path!\n");
	}
	else if (option == "-f") {
		if (sou == NULL) {
			printf("No Such File.\n");
			return;
		}
		else {
			if (den->files.find(name) != den->files.end()) {
				cout << "Destination has a same file.\n";
				return;
			}
			else if (sou->files.find(name) == sou->files.end()) {
				printf("No Such File.\n");
				return;
			}
			else if (curdir->files[name]->owner.name != curuser.name) {
				cout << "This file is not yours!\n";
			}
			else {
				file *tmp = new file(*(sou->files[name]));
				den->files[name] = tmp;
			}
		}
	}
	else if (option == "-d") {
		if (sou == NULL) {
			printf("No Such Directory.\n");
		}
		else {
			if (den->next.find(name) != den->next.end()) {
				cout << "Destination has a same directory.\n";
				return;
			}
			else if (sou->next.find(name) == sou->next.end()) {
				printf("No Such Directory.\n");
				return;
			}
			else {
				dir *tmp = cpDir(sou->next[name]);
				tmp->pre = den;
				den->next[name] = tmp;
			}
		}
	}
	else if (option == "-cf") {
		if (sou == NULL) {
			printf("No Such File.\n");
			return;
		}
		else {
			if (den->files.find(name) != den->files.end()) {
				cout << "Destination has a same file.\n";
				return;
			}
			else if (sou->files.find(name) == sou->files.end()) {
				printf("No Such File.\n");
				return;
			}
			else if (curdir->files[name]->owner.name != curuser.name) {
				cout << "This file is not yours!\n";
			}
			else {
				den->files[name] = sou->files[name];
				sou->files.erase(name);
			}
		}
	}
	else if (option == "-cd") {
		if (sou == NULL) {
			printf("No Such Directory.\n");
		}
		else {
			if (den->next.find(name) != den->next.end()) {
				cout << "Destination has a same directory.\n";
				return;
			}
			else if (sou->next.find(name) == sou->next.end()) {
				printf("No Such Directory.\n");
				return;
			}
			else {
				den->next[name] = sou->next[name];
				sou->next.erase(name);
			}
		}
	}
	else {
		cout << "Wrong Option!\n";
	}

}

//清屏
void cls() {
	system("cls");
}

//切换用户
void su() {
	login();
}

//存储目前情况
void save(dir *tmp) {
	records.push_back(tmp->name);//目录名
	records.push_back(to_string(tmp->files.size()));//文件数
	for (auto it = tmp->files.begin(); it != tmp->files.end(); it++) {
		records.push_back(it->second->name);//文件名
		for (int i = 0; i < it->second->content.size(); i++) {
			records.push_back( it->second->content[i]);//文件内容
		}
		records.push_back("content");//文件内容结束符
		records.push_back(it->second->owner.name);//所有者用户名
		records.push_back(it->second->owner.password);//所有者密码
	}
	records.push_back(to_string(tmp->next.size()));//子目录数
	for (auto it = tmp->next.begin(); it != tmp->next.end(); it++) {
		records.push_back(it->second->name);//子目录名
		save(it->second);//递归子目录
	}
}

//还原上一次系统关闭状态
dir* creat(dir *last) {
	dir *tmp = new dir();//新建目录
	tmp->name=records[reco++];//读取目录名
	tmp->pre = last;//设置父指针
	string t;
	t= records[reco++];//读取文件数
	for (int i = 0; i < stoi(t); i++) {
		file *tfile = new file();//新建文件
		tfile->name= records[reco++];//读取文件名
		while (1) {
			string ts;
			ts= records[reco++];//读取文件内容
			if (ts != "content") {//不是关键字就持续读入内容
				tfile->content.push_back(ts);
			}
			else {
				break;
			}
		}
		user a;
		a.name = records[reco++];//读取用户名
		a.password= records[reco++];//密码
		tfile->owner = a;
		tmp->files[tfile->name] = tfile;//将新建的文件加入目录
	}
	t= records[reco++];//子目录数
	for (int i = 0; i < stoi(t); i++) {
		string name;
		name= records[reco++];//子目录名
		tmp->next[name] = creat(tmp);//递归新建子目录
	}
	return tmp;
}

void init() {
	ifstream inr("record.dat");
	string tmp;
	if (!inr) {
		initDir();
	}
	while (inr >> tmp)
		records.push_back(tmp);
	if (records.size() >= 1) {
		root = curdir = creat(NULL);
	}
	else {
		initDir();
	}
}


//退出系统
void exit() {
	records.clear();
	save(root);
	ofstream outr("record.dat");
	for (int i = 0; i < records.size(); i++) {
		outr << records[i] << endl;
	}
}


int main() {
	init();
	login();
	string command;
	while (1) {
		cout << curuser.name << "@" << curuser.name << "-FileSystem:";
		displayPath();
		getline(cin, command);
		string tmp = command;
		if (command.find_first_of(' ') != command.npos) {
			command = command.substr(0, command.find_first_of(' '));
			tmp = tmp.substr(tmp.find_first_of(' ') + 1, tmp.length() - tmp.find_first_of(' '));
		}
		else tmp = "";
		if (command == "help") help();
		else if (command == "mkdir") mkdir(tmp);
		else if (command == "cd") cd(tmp);
		else if (command == "touch") touch(tmp);
		else if (command == "ls") ls(tmp);
		else if (command == "gedit") gedit(tmp);
		else if (command == "rename") rename(tmp);
		else if (command == "rm") rm(tmp);
		else if (command == "cp") cp(tmp);
		else if (command == "cls") cls();
		else if (command == "su") su();
		else if (command == "exit") {
			exit();
			return 0;
		}
		else help2(command);
	}
	system("pause");
}

