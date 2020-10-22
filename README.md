
# 1.实验要求

1.  设计和实现一个模拟文件系统，要求包括目录、普通文件和文件的存储。。

2.  文件系统的目录结构采用类似Linux的树状结构。

3.  要求模拟的操作包括：

-   目录的添加、删除、重命名；

-   目录的显示（列表）；

-   文件的添加、删除、重命名；

-   文件和目录的拷贝；

-   文件的读写操作。

1.  用户进入时显示可用命令列表；用户输入help时显示所有命令的帮助文档；
    输入某个命令+？时显示该条命令的使用说明。

2.  用户输入exit时退出该系统。

3.  实验实现基于LINUX平台。

4.  实验开发语言必须选用C/C++，不能选用JAVA。

# 2.实验环境

-   调试环境：

-   操作系统：Ubuntu 16.04 TLS；

-   内存：3.5GiB

-   处理器：AMD E-350 Processor×2

-   图形：AMD PALM（DRM 2.50.0/4.15.0-45-generic,LLVM 6.6.6）

-   操作系统类型：64位

-   磁盘：30.4GB

-   开发环境：

    -   开发工具：Visual Studio 2017；

    -   操作系统：Windows 10 家庭中文版；

    -   处理器：Inter(R) Core(TM) i7-8565u CPU \@ 1.8GHz 1.99 GHz；

    -   内存：8.00 GB；

    -   系统类型：64位操作系统，基于x64的处理器。

# 3.实验设计

## 3.1系统流程

整体系统操作模拟Ubuntu命令行，具体流程如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201016112506877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


## 3.2文件结构

整体系统采用属性结构组织文件，具体图示如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201016111224166.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


## 3.3实现的命令


| 命令   | 用法                            | 说明                                   | 选项                                                                                                       |
|--------|---------------------------------|----------------------------------------|------------------------------------------------------------------------------------------------------------|
| cd     | cd [dir]                        | 显示当前目录名或改变当前目录           | 无                                                                                                         |
| ls     | ls [dir]                        | 显示当前或指定路径下所有文件和目录     | 无                                                                                                         |
| mkdir  | mkdir dir                       | 在当前目录下建立一个新目录             | 无                                                                                                         |
| touch  | touch file                      | 在当前目录下新建一个新文件             | 无                                                                                                         |
| gedit  | gedit file                      | 读写指定的文件                         | 无                                                                                                         |
| rm     | rm -d\|-f file\|dir             | 删除指定的目录或文件                   | \-d：删除目录 -f：删除文件                                                                                 |
| cp     | cp -d\|-f\|-cd\|-cf SOURSE DEST | 从原路径复制一个文件或目录到目的路径下 | \-d：复制目录 -f：复制文件 -cd：复制目录，但不在原路径下保留原目录 -cf：复制文件，但不在原路径下保留原文件 |
| rename | rename -d\|-f oldname newname   | 更改指定文件或目录的名字               | \-d：重命名目录 -f：重命名文件                                                                             |
| su     | su                              | 更改当前用户                           | 无                                                                                                         |
| cls    | cls                             | 清屏                                   | 无                                                                                                         |
| exit   | exit                            | 退出文件系统                           | 无                                                                                                         |
| help   | help                            | 显示帮助文档                           | 无                                                                                                         |

# 4.数据结构

为简化代码结构，系统未采用面向对象编程的思想。将用户、文件、目录分别封装成一个struct结构体。

## 4.1 用户数据结构

```cpp
 struct user

{

string name;//用户名

string password;//密码

};
```

说明：用户结构体中包含用户的用户名以及密码。

## 4.2 文件数据结构

```cpp
struct file

{

string name;//文件名

vector<string> content;//文件内容

user owner;//文件所有者

};
```

说明：文件结构体包含文件名、文件内容以及文件所有者。需要特别指出的是，目前系统实现的多用户权限可以概括为：文件创建者为文件所有者，非文件所有者可以知道该文件的存在，但不能对该文件执行读写、复制、删除等操作。

## 4.3 目录数据结构

```cpp
struct dir {

string name;//目录名

dir* pre;//父目录

map<string, file*> files;//所包含的文件

map<string, dir*> next;//子目录

};
```

说明：目录结构体包含目录名、父目录、当前目录下的文件以及直接子目录。其中，为方便代码编写，后两个成员使用map容器包装，其中map的first为文件名（或目录名），second为对应的文件指针（或目录指针）。

# 5.模块详解

## 5.1 用户指令

以下按3.3节展开。

### 5.1.1 cd

-   说明：显示当前目录的绝对路径或改变当前目录。

-   流程图：

![](https://img-blog.csdnimg.cn/20201016111852850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


-   关键代码：

```cpp
//cd  
void cd(string name) {  
    if (name == "") {  
        //显示当前目录的绝对路径  
    }  
    else {  
        dir* tmp = pathTrans(name);//解析路径  
        if (tmp == NULL) {  
            cout << "No Such Directory.\n";  
        }  
        else {  
            curdir = tmp;//进入用户输入的路径  
        }  
    }  
}  
```


### 5.1.2 ls

-   说明：显示当前目录下或指定路径下所有文件和目录。

-   流程图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201016112055214.png#pic_center)



-   关键代码：

```cpp
//ls  
void ls(string path) {  
    dir *tmp = curdir;  
    if (path != "") {  
        curdir = pathTrans(path);//解析路径  
        if (curdir == NULL) {  
            //输出错误提示  
        }  
    }  
    //遍历输出文件和目录信息  
    for (auto it = curdir->files.begin(); it != curdir->files.end(); it++) {  
    }  
    for (auto it = curdir->next.begin(); it != curdir->next.end(); it++) {  
    }  
    curdir = tmp;  
}  
```

### 5.1.3 mkdir

-   说明：在当前目录下建立一个新目录。

-   用法：mkdir dir，其中dir表示新建目录的目录名。

-   流程图：

![](https://img-blog.csdnimg.cn/20201021134154805.png#pic_center)


-   关键代码：

```cpp
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
```

### 5.1.4 touch

-   说明：在当前目录下新建一个新文件。

-   用法：touch file，其中file表示新建文件的文件名

-   流程图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174101297.png#pic_center)


-   关键代码：

```cpp
//建立文件  
void touch(string name) {  
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
        //建立对应的新文件  
    }  
}  
```


### 5.1.5 gedit

-   说明：读写指定的文件。

-   用法：gedit file，其中file表示需要读写的文件，可以用绝对路径或相对路径指定。

-   流程图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174122888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)




-   关键代码：

```cpp
//编辑文件  
void gedit(string name) {  
    dir *t = curdir;  
    if (name.find_last_of('/') != name.npos) {  
        //解析路径  
    }  
    //是否存在目标文件  
    if (curdir->files.find(name) == curdir->files.end()) {}  
    //目标文件是否为当前用户所拥有  
    else if (curdir->files[name]->owner.name != curuser.name) {}  
    else {  
        ofstream out("tmp.dat");  
        //将文件当前内容输入临时文件  
        for (int i = 0; i < curdir->files[name]->content.size(); i++) {}  
        out.close();  
        //用gedit打开临时文件  
        system("gedit tmp.dat");  
        //读取临时文件中的内容，存入文件  
        ifstream in("tmp.dat");  
        while (getline(in, t)) {  
            //读取临时文件内容  
        }  
    }  
    curdir = t;  
}  
```


-   代码解释：

    因本文件系统完全运行在内存中，创立的文件与目录并未存在实际磁盘中。因此，为提高用户读写文件的体验，在此引入了一个临时文件机制：将存储在内存中的文件暂时存储到临时文件中，然后用gedit打开这个临时文件。在用户编辑完成后，再将临时文件中的内容转存到内存中。

    Gedit是一个GNOME桌面环境下兼容UTF-8的文本编辑器。它使用GTK+编写而成，因此它十分的简单易用，有良好的语法高亮，对中文支持很好，支持包括gb2312、gbk在内的多种字符编码。

    虽然这样的操作效率不高，但是借助gedit强大的功能，能给用户带来一种编辑真实文件的体验。

### 5.1.6 rm

-   说明：删除指定的目录或文件。

-   用法：rm -d\|-f
    file\|dir，其中file与dir代表需要删除的文件或目录，目录（或文件）均可用绝对路径或者相对路径表示。

-   选项：

    -   \-d：删除目录；

    -   \-f：删除文件。

-   流程图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174134523.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



-   关键代码：

```cpp
//删除  
void rm(string tmp) {  
    //选项解析，路径解析  
    //删除目录  
    if (option == "-d") {  
        //是否存在目标目录  
        if (curdir->next.find(name) == curdir->next.end()) {}  
        else {  
            deletedir(curdir->next[name]);//递归删除  
        }  
    }  
    else if (option == "-f") {  
        //是否存在目标目录  
        if (curdir->files.find(name) == curdir->files.end()) {}  
        //是否为文件所有者  
        else if (curdir->files[name]->owner.name != curuser.name) {}  
        else {  
            delete(curdir->files[name]);  
        }  
    }  
}  

```

-   代码解释：因本系统采用树形的目录结构，因此在删除目录时需要用到递归，如deletedir函数所示。

```cpp
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

```

### 5.1.7 cp

-   说明：复制一个文件或目录到指定路径下。

-   用法：cp -d\|-f\|-cd\|-cf SOURCE
    DEST，其中SOURCE为需要复制的文件或目录，DEST为需要复制到的路径。SOURCE与DEST均可用绝对路径或者相对路径表示。

-   选项：

    -   \-d：复制目录

    -   \-f：复制文件

    -   \-cd：复制目录，但不在原路径下保留原目录

    -   \-cf：复制文件，但不在原路径下保留原文件

-   流程图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174146208.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



-   关键代码：

```cpp
//复制  
void cp(string tmp) {  
    //解析选项与路径  
  
    if (option == "-f") {  
        //是否有同名文件     
        if (den->files.find(name) != den->files.end()) {}  
        //是否存在目标文件  
        else if (sou->files.find(name) == sou->files.end()) {}  
        //是否为当前用户所拥有  
        else if (curdir->files[name]->owner.name != curuser.name) {}  
        else {  
            file *tmp = new file(*(sou->files[name]));//复制文件  
            den->files[name] = tmp;//放入目的目录下  
        }  
    }  
    else if (option == "-d") {  
        //是否有同名目录  
        if (den->next.find(name) != den->next.end()) {}  
        //是否存在目标目录  
        else if (sou->next.find(name) == sou->next.end()) {}  
        else {  
            dir *tmp = cpDir(sou->next[name]);//递归复制目录  
             tmp->pre = den;
            den->next[name] = tmp;//放入到目的目录下  
        }  
    }  
    else if (option == "-cf") {  
        //是否有同名文件  
        if (den->files.find(name) != den->files.end()){}  
         //是否存在目标文件  
        else if (sou->files.find(name) == sou->files.end()){}  
        //是否为当前用户所拥有  
        else if (curdir->files[name]->owner.name != curuser.name){}  
        else  
        {  
            den->files[name] = sou->files[name];//放入到目的目录下  
            sou->files.erase(name);  
        }  
    }  
      
    else if (option == "-cd") {  
        //是否有同名目录  
        if (den->next.find(name) != den->next.end()){}  
        //是否存在目标目录  
        else if (sou->next.find(name) == sou->next.end()){}  
        else  
        {  
            den->next[name] = sou->next[name];//放入到目的目录下  
            sou->next.erase(name);  
        }  
    }  
}  

```

-   代码解释：跟删除类似，在复制目录时需要用到递归复制，如cpDir所示。

```cpp
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

```



### 5.1.8 rename

-   说明：更改指定文件或目录的名字。

-   用法：rename -d\|-f oldname
    newname，oldname代表需要重命名的目录或文件，newname代表重命名后的名字。oldname可以使用绝对路径或相对路径。

-   选项：

    -   \-d：重命名目录；

    -   \-f：重命名文件。

-   流程图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174201976.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



-   关键代码：

```cpp
void rename(string tmp) {  
    //解析路径与选项  
  
    if (!judgeName(newname)) {}//新名字中是否有非法字符  
    if (option == "-d") {  
        if (curdir->next.find(old) == curdir->next.end()) {}//是否存在目标目录  
        else if (curdir->next.find(newname) != curdir->next.end()) {}//是否存在与新名字重名的目录  
        else {  
            //重命名  
        }  
    }  
    else if (option == "-f") {  
        if (curdir->files.find(old) == curdir->files.end()) {}//是否存在对应文件  
        else if (curdir->files.find(newname) != curdir->files.end()) {}//是否存在与新名字重名的文件  
        else if (curdir->files[old]->owner.name != curuser.name) {}//需重命名文件是否为当前用户拥有  
        else {  
            //重命名  
        }  
    }  
}  

```

### 5.1.9 su

-   说明：更改当前用户（调用login函数，详见下文）。

### 5.1.10 cls

-   说明：清屏。

### 5.1.11 exit

-   说明：退出文件系统（主要调用save函数，详见下文）。

### 5.1.12 help

-   说明：显示帮助文档。

## 5.2 其他系统操作 

### 5.2.1 登录

-   流程图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174213369.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



-   关键代码：

```cpp
void login() {  
    bool flag = 1;  
    map<string, string> users;//所有注册用户  
    //打印欢迎语  
  
    ifstream in("user.dat");  
    string tname, tpass;  
    while (in >> tname >> tpass) {}//读入所有注册用户的信息  
    //输入用户名  
  
    while (flag) {  
        if (users.find(tname) == users.end()) {}//注册用户中是否有输入用户  
        else {  
            if (users[tname] == tpass) {  
                //核对用户名与密码是否匹配  
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
            //注册  
        }  
        else {  
            login();  
        }  
    }  
    //重新存回  
    ofstream out("user.dat");  
    for (auto it = users.begin(); it != users.end(); it++) {}  
    return;  
}  
```

### 5.2.2 保存系统状态

为提高用户体验，每次用户使用exit命令退出系统时，系统会保存退出前的系统状态。

保存系统状态的主要实现思想是：通过递归遍历系统的目录结构，以字符串向量的形式存储包括目录名、文件名、文件内容在内的所有信息。最后将字符串向量存入record.dat文件。字符串向量内部的结构类似xml文件。具体流程图与代码如下：

-   流程图：

    -   exit函数：

![在这里插入图片描述](https://img-blog.csdnimg.cn/202010221742443.png#pic_center)



-   save函数：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174250555.png#pic_center)



-   具体代码：
```cpp
//退出系统  
void exit() {  
    records.clear();  
    save(root);  
    ofstream outr("record.dat");  
    for (int i = 0; i < records.size(); i++) {  
        outr << records[i] << endl;  
    }  
}  
```
```cpp
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

```


为方便调试，record.dat文件以明文存储，文件内容如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174310507.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



### 5.2.3 恢复系统状态

为提高用户体验，每次打开系统时，系统会根据record.dat文件存储的系统状态恢复上一次退出前的系统状态。

恢复系统状态的实现思想与保存系统状态（5.2.2）类似，根据record.dat的数据重建目录结构。
具体流程图与代码如下：

-   流程图：

    -   init函数：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174317526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


-   creat函数：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174324907.png#pic_center)


-   具体代码：

```cpp
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
```
```cpp
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
```

### 5.2.4 路径解析

为提高用户体验，系统引入了路径解析的机制，可以让包括cd、ls、gedit在内的多种操作同时支持相对路径与绝对路径。绝对路径与相对路径的表示方式与Ubuntu系统相同。

路径解析输入的参数为绝对路径或相对路径，返回的结果是目标目录的指针。具体流程图与代码如下：

-   流程图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174350812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)

-   关键代码：

```cpp
dir* pathTrans(string path) {  
    string tmp = path;  
    //绝对路径  
    if (path[0] == '~' || path[0] == '/') {  
        dir *cur = root;  
        if (path[0] == '/')path = "~" + path;  
        vector<string> tmp = split(path);//按照/分割路径  
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
        vector<string> tmp = split(path);//按照/分割路径  
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
```

# 6.实验演示

## 6.1登录

-   欢迎界面

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174516365.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


-   当用户名与密码不匹配，系统会让用户反复输入密码，直到正确为止：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174610430.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



-   当输入用户名与不在已注册用户中，若用户同意创建用户，系统会根据先前输入的用户名与密码创建用户：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174636103.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



## 6.2帮助文档与使用说明

-   用户输入help时显示所有命令的帮助文档：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174643946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


-   输入某个命令+？时显示该条命令的使用说明：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174659624.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



## 6.3 cd命令
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174708643.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



说明：红框为目前根节点下拥有的目录；黄框展示使用相对路径、绝对路径更改目录，以及回到上一级目录的情况；蓝框展示遇到错误路径的情况。

## 6.4 ls命令

![在这里插入图片描述](https://img-blog.csdnimg.cn/2020102217471766.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


说明：黄框展示有参及无参ls命令的使用情况；蓝框展示遇到错误路径的情况。

## 6.5 mkdir命令

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174725761.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174733122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174742918.png#pic_center)



说明：红框展示目前根节点下拥有的目录；黄框展示新建一个paly目录的情况；蓝框展示遇到重名目录的情况；第三张图展示新建目录名中存在非法字符的情况。

## 6.6 touch命令

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174823428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



说明：红框展示\~/study/college目录下拥有的文件；黄框展示新建一个test2.txt文件的情况；蓝框展示遇到重名文件的情况；展示新建文件名中存在非法字符的情况。

## 6.7 gedit命令

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174845852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



说明：红框展示\~/study/college目录下拥有的文件；黄框展示读写test2.txt文件的情况，截图下半部为gedit打开临时文件；蓝框展示遇到缺少文件名及文件非当前用户拥有的情况。

## 6.8 rm命令

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174856914.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


说明：红框展示根目录下拥有的文件与目录；黄框展示删除work目录的情况；绿框展示删除test.txt文件的情况；蓝框展示遇到不存在目录或文件的情况。

## 6.9 cp命令
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201022174932863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


说明：红框展示\~/study目录与\~/work目录下拥有的文件与目录；黄框展示复制college目录到\~/work目录的情况。

![](https://img-blog.csdnimg.cn/20201022174939527.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)



说明：红框展示\~/work与\~/study/college目录下拥有的文件与目录；黄框展示复制test2.txt文件到\~/work目录的情况。

![在这里插入图片描述](https://img-blog.csdnimg.cn/2020102217510360.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)




说明：红框展示\~/work与\~/study/college目录下拥有的文件与目录；黄框展示剪贴test2.txt文件到\~/work目录的情况。

![](https://img-blog.csdnimg.cn/20201022174949589.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)


说明：红框展示\~/work与\~/study目录下拥有的文件与目录；黄框展示剪贴college目录到\~/work目录的情况。

## 6.10 用户权限

系统实现的多用户权限可以概括为：文件创建者为文件所有者，非文件所有者可以知道该文件的存在，但不能对该文件执行读写、复制、删除等操作。下图展示了非文件所有者不能对文件执行gedit、rm、rename、cp命令的情况：

![在这里插入图片描述](https://img-blog.csdnimg.cn/202010221751176.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDMxODE5Mg==,size_16,color_FFFFFF,t_70#pic_center)

